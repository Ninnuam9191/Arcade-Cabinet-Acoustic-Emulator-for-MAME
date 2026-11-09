/*
 * ArcadeAmpDSP.hpp - the actual signal processing of ArcadeAmp (v0.3), with
 * no plugin-framework dependency, so the exact same code is (a) compiled into
 * the VST and (b) compiled into a standalone test program that checks it
 * (see ../tests/dsp_test.cpp). What is tested is what ships.
 *
 * Signal path, per sample (one amplifier, one power supply, so the sag
 * detector and every noise source are shared by all channels):
 *
 *   in -> SAG (supply droop) -> DRIVE/WARMTH (saturation) -> DC block
 *      -> + HUM + HISS + WHINE -> out
 *
 * EVIDENCE LABELS (project convention):
 *   DOCUMENTED  - the number/behaviour comes from a standard or textbook
 *   INFERRED    - follows from documented physics, constants are chosen
 *   EXPERIMENTAL- usability choice; not measured from any cabinet or TV
 *
 *  DRIVE   tanh soft clipper out = tanh(k*x)/k, k = Drive*0.1.
 *          Textbook DSP (J. O. Smith, "Physical Audio Signal Processing").
 *          Small-signal gain exactly 0 dB; |out| <= |in|; Drive 0 = bit-exact
 *          bypass.  Generic curve: EXPERIMENTAL, not any specific amplifier.
 *
 *  WARMTH  Asymmetry. Positive and negative half-waves get different k:
 *          kPos = k(1+w), kNeg = k(1-w), w = Warmth*0.8/100. A symmetric
 *          clipper makes odd harmonics only; a single-ended / biased stage
 *          makes even ones too (INFERRED from amplifier theory). The amount
 *          is EXPERIMENTAL. Small-signal gain stays 0 dB and each half is
 *          still <= |in|. The DC this creates is removed by a 5 Hz
 *          high-pass (real amplifiers are AC-coupled). Needs Drive > 0.
 *
 *  SAG     Level-dependent gain loss: g = 1/(1 + s*env), env = peak follower
 *          (attack 8 ms, release 120 ms) of the loudest channel,
 *          s = Sag*0.02. An unregulated supply's rail droops under load and
 *          recovers over tens to hundreds of ms (INFERRED); the time
 *          constants and depth are EXPERIMENTAL. g <= 1 always (never
 *          louder); g -> 1 for quiet signals. A linear EQ cannot do this.
 *
 *  HUM     Mains-synchronous buzz from the harmonic series of the mains
 *          frequency, 2nd harmonic dominant (full-wave rectifier ripple at
 *          2x mains - INFERRED). Weights are an approximation, not
 *          measured. 50/60 Hz choice is real (Japan: east 50, west 60 -
 *          DOCUMENTED).
 *
 *  HISS    White noise through a one-pole low-pass whose corner is set by
 *          Hiss Tone (1 kHz dark ... 16 kHz bright). EXPERIMENTAL colour.
 *
 *  WHINE   Pure tone at 15,734.26 Hz = the NTSC / NTSC-J horizontal scan
 *          rate (4.5 MHz / 286 - DOCUMENTED standard, used by Japanese TVs
 *          and by standard-resolution 15 kHz arcade monitors). Real sets
 *          emit it acoustically from the flyback transformer, and it can
 *          leak into the audio; whether and how loudly it did in a given
 *          set is EXPERIMENTAL. PAL is 15,625 Hz (109 Hz lower), not
 *          modelled. Silent if the sample rate is below 35 kHz.
 *
 *  Noise level mapping (usability choice, NOT a measured noise floor; none
 *  exists in this project, see docs/research/MVS.md):
 *      Hum, Hiss : dBFS RMS = -54 + 0.30 * %   (100 % = -24, 25 % = -46.5)
 *      Whine     : dBFS RMS = -60 + 0.30 * %   (100 % = -30)
 *
 *  PRESET selects a ready-made recipe for all of the above except Mains
 *  (see presetParams()). Recipes are EXPERIMENTAL starting points chosen by
 *  hardware class, not measurements.
 *
 * License: GPL-2.0-or-later (see ../LICENSE.txt).
 */

#pragma once

#include <cmath>
#include <cstdint>

struct ArcadeAmpParams
{
    float drive    = 10.0f;  // 0-100
    float warmth   = 0.0f;   // 0-100
    float sag      = 0.0f;   // 0-100
    float hum      = 0.0f;   // 0-100
    float hiss     = 0.0f;   // 0-100
    float hissTone = 50.0f;  // 0 dark .. 100 bright
    float whine    = 0.0f;   // 0-100
    bool  mains50  = false;
};

class ArcadeAmpDSP
{
public:
    enum Preset { kCustom = 0, kLight, kMedium, kHeavy, kTVEarly, kTVLate, kPresetCount };

    static constexpr float kNoiseFloorDbAt0 = -54.0f;
    static constexpr float kNoiseDbPerPercent = 0.30f;
    static constexpr float kWhineFloorDbAt0 = -60.0f;
    static constexpr float kFadeInPct = 2.0f;
    static constexpr double kWhineHz = 15734.264; // 4.5e6 / 286
    static constexpr float kSagPerPercent = 0.02f;
    static constexpr float kWarmthMax = 0.8f;

    // Resolve a preset into concrete parameters. Mains is never changed by a
    // preset (it depends on where the machine lived); Custom returns `custom`.
    static ArcadeAmpParams presetParams(int preset, const ArcadeAmpParams& custom)
    {
        ArcadeAmpParams p = custom;
        switch (preset)
        {
        //                    drive warmth sag hum hiss tone whine
        case kLight:   set(p,   5,    0,    0,   0,   0,  50,   0); break;
        case kMedium:  set(p,  12,   15,   10,  15,  10,  40,   0); break;
        case kHeavy:   set(p,  25,   25,   20,  25,  20,  35,   0); break;
        case kTVEarly: set(p,  25,   25,   20,  25,  25,  30,  25); break; // 1970s-80s
        case kTVLate:  set(p,  12,   15,   10,  12,  10,  45,  12); break; // 1990s
        default: break;
        }
        return p;
    }

    ArcadeAmpDSP()
    {
        buildBuzzTable();
        setSampleRate(48000.0);
        setParameters(ArcadeAmpParams());
        reset();
    }

    void setSampleRate(double sr)
    {
        if (sr < 8000.0) sr = 8000.0;
        fSampleRate = sr;
        fSmooth = static_cast<float>(1.0 - std::exp(-1.0 / (0.010 * sr)));
        fAtk = static_cast<float>(1.0 - std::exp(-1.0 / (0.008 * sr)));
        fRel = static_cast<float>(1.0 - std::exp(-1.0 / (0.120 * sr)));
        fDcR = static_cast<float>(1.0 - 2.0 * M_PI * 5.0 / sr);
        fWhineOn = sr >= 35000.0;
        fWhineInc = kWhineHz / sr;
        updateMainsIncrement();
        updateHiss();
    }

    void reset()
    {
        fK = fKTarget; fW = fWTarget; fS = fSTarget;
        fHum = fHumTarget; fHissLvl = fHissTarget; fWhine = fWhineTarget;
        fBuzzPhase = 0.0f; fWhinePhase = 0.0;
        fHissState = 0.0f; fRng = 0x1234567u;
        fEnv = 0.0f;
        for (int c = 0; c < kMaxCh; ++c) { fDcX[c] = 0.0f; fDcY[c] = 0.0f; }
    }

    void setParameters(const ArcadeAmpParams& in)
    {
        fKTarget = clamp(in.drive) * 0.1f;
        fWTarget = clamp(in.warmth) * 0.01f * kWarmthMax;
        fSTarget = clamp(in.sag) * kSagPerPercent;
        fHumTarget = noiseAmp(clamp(in.hum), kNoiseFloorDbAt0);
        fHissTarget = noiseAmp(clamp(in.hiss), kNoiseFloorDbAt0);
        fWhineTarget = noiseAmp(clamp(in.whine), kWhineFloorDbAt0) * 1.41421356f; // sine: peak = RMS*sqrt2
        fHissTone = clamp(in.hissTone);
        fMains50 = in.mains50;
        updateMainsIncrement();
        updateHiss();
    }

    // `in` and `out` may alias. Up to kMaxCh channels.
    void process(const float* const* in, float* const* out, uint32_t frames, uint32_t channels)
    {
        if (channels > kMaxCh) channels = kMaxCh;
        for (uint32_t i = 0; i < frames; ++i)
        {
            fK += (fKTarget - fK) * fSmooth;
            fW += (fWTarget - fW) * fSmooth;
            fS += (fSTarget - fS) * fSmooth;
            fHum += (fHumTarget - fHum) * fSmooth;
            fHissLvl += (fHissTarget - fHissLvl) * fSmooth;
            fWhine += (fWhineTarget - fWhine) * fSmooth;

            // ---- shared sag detector (one power supply) ----
            float gain = 1.0f;
            if (fS > 1.0e-5f || fEnv > 1.0e-6f)
            {
                float pk = 0.0f;
                for (uint32_t ch = 0; ch < channels; ++ch)
                {
                    const float a = std::fabs(in[ch][i]);
                    if (a > pk) pk = a;
                }
                fEnv += (pk - fEnv) * (pk > fEnv ? fAtk : fRel);
                gain = 1.0f / (1.0f + fS * fEnv);
            }

            // ---- shared noise ----
            float noise = 0.0f;
            if (fHum > 1.0e-7f)     noise += fHum * nextBuzz();
            else                    nextBuzz();
            const float hiss = nextHiss();
            if (fHissLvl > 1.0e-7f) noise += fHissLvl * hiss;
            if (fWhine > 1.0e-7f && fWhineOn)
            {
                noise += fWhine * static_cast<float>(std::sin(2.0 * M_PI * fWhinePhase));
            }
            fWhinePhase += fWhineInc;
            if (fWhinePhase >= 1.0) fWhinePhase -= 1.0;

            const bool sat = fK >= 1.0e-3f;
            const bool asym = sat && fW > 1.0e-4f;

            for (uint32_t ch = 0; ch < channels; ++ch)
            {
                float x = in[ch][i] * gain;
                float y = x;
                if (sat)
                {
                    const float k = (asym && x < 0.0f) ? fK * (1.0f - fW)
                                  : (asym ? fK * (1.0f + fW) : fK);
                    y = std::tanh(k * x) / k;
                }
                if (asym)
                {
                    // 5 Hz DC blocker (AC coupling)
                    const float d = y - fDcX[ch] + fDcR * fDcY[ch];
                    fDcX[ch] = y; fDcY[ch] = d; y = d;
                }
                else { fDcX[ch] = y; fDcY[ch] = 0.0f; }
                out[ch][i] = y + noise;
            }
        }
    }

    float currentK() const { return fK; }

private:
    static constexpr int kTableSize = 4096;
    static constexpr int kMaxCh = 8;

    static void set(ArcadeAmpParams& p, float d, float w, float s, float h, float hs, float t, float wh)
    { p.drive = d; p.warmth = w; p.sag = s; p.hum = h; p.hiss = hs; p.hissTone = t; p.whine = wh; }

    static float clamp(float v) { return v < 0.0f ? 0.0f : (v > 100.0f ? 100.0f : v); }

    static float noiseAmp(float pct, float floorDbAt0)
    {
        if (pct <= 0.0f) return 0.0f;
        float amp = std::pow(10.0f, (floorDbAt0 + kNoiseDbPerPercent * pct) / 20.0f);
        if (pct < kFadeInPct) amp *= pct / kFadeInPct;
        return amp;
    }

    void updateMainsIncrement()
    {
        fBuzzInc = static_cast<float>((fMains50 ? 50.0 : 60.0) / fSampleRate);
    }

    void updateHiss()
    {
        // corner 1 kHz (Tone 0) .. 16 kHz (Tone 100), exponential
        double fc = 1000.0 * std::pow(16.0, fHissTone / 100.0);
        if (fc > fSampleRate * 0.45) fc = fSampleRate * 0.45;
        fHissA = static_cast<float>(std::exp(-2.0 * M_PI * fc / fSampleRate));
        const double a = fHissA;
        // uniform[-1,1] has variance 1/3; one-pole y=a*y+(1-a)*x scales variance by (1-a)/(1+a)
        fHissNorm = static_cast<float>(std::sqrt(3.0) / std::sqrt((1.0 - a) / (1.0 + a)));
    }

    void buildBuzzTable()
    {
        static const double w[] = { 0.0, 0.30, 1.00, 0.20, 0.50, 0.12, 0.30, 0.08, 0.18, 0.05, 0.10 };
        const int nh = static_cast<int>(sizeof(w) / sizeof(w[0])) - 1;
        double sumSq = 0.0;
        for (int h = 1; h <= nh; ++h) sumSq += w[h] * w[h] * 0.5;
        const double norm = 1.0 / std::sqrt(sumSq);
        for (int n = 0; n < kTableSize; ++n)
        {
            double s = 0.0;
            for (int h = 1; h <= nh; ++h) s += w[h] * std::sin(2.0 * M_PI * h * n / kTableSize);
            fBuzzTable[n] = static_cast<float>(s * norm);
        }
    }

    inline float nextBuzz()
    {
        const float pos = fBuzzPhase * kTableSize;
        int i0 = static_cast<int>(pos);
        const float frac = pos - static_cast<float>(i0);
        i0 &= (kTableSize - 1);
        const int i1 = (i0 + 1) & (kTableSize - 1);
        const float v = fBuzzTable[i0] + (fBuzzTable[i1] - fBuzzTable[i0]) * frac;
        fBuzzPhase += fBuzzInc;
        if (fBuzzPhase >= 1.0f) fBuzzPhase -= 1.0f;
        return v;
    }

    inline float nextHiss()
    {
        fRng ^= fRng << 13; fRng ^= fRng >> 17; fRng ^= fRng << 5;
        const float white = (static_cast<float>(fRng) / 4294967295.0f) * 2.0f - 1.0f;
        fHissState = fHissState * fHissA + white * (1.0f - fHissA);
        return fHissState * fHissNorm;
    }

    double fSampleRate = 48000.0;
    float fSmooth = 0.001f, fAtk = 0.01f, fRel = 0.001f, fDcR = 0.999f;

    float fKTarget = 1.0f, fK = 1.0f;
    float fWTarget = 0.0f, fW = 0.0f;
    float fSTarget = 0.0f, fS = 0.0f;
    float fHumTarget = 0.0f, fHum = 0.0f;
    float fHissTarget = 0.0f, fHissLvl = 0.0f;
    float fWhineTarget = 0.0f, fWhine = 0.0f;
    float fHissTone = 50.0f;
    bool  fMains50 = false;
    bool  fWhineOn = true;

    float fEnv = 0.0f;
    float fDcX[kMaxCh] = {}, fDcY[kMaxCh] = {};

    float fBuzzTable[kTableSize];
    float fBuzzPhase = 0.0f;
    float fBuzzInc = 60.0f / 48000.0f;

    double fWhinePhase = 0.0, fWhineInc = kWhineHz / 48000.0;

    float fHissState = 0.0f, fHissA = 0.5f, fHissNorm = 1.0f;
    uint32_t fRng = 0x1234567u;
};
