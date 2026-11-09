// Standalone test of ArcadeAmpDSP.hpp - the exact code compiled into the plugin.
// Build:  g++ -O2 -Wall -I../source -o dsp_test dsp_test.cpp -lm && ./dsp_test
//
// Unlike the first version of this project's tests, this one:
//   * uses MAME-realistic signal levels (peaks ~0.3, not full scale);
//   * runs the REAL two-channel process() path, not a one-channel excerpt;
//   * measures noise in dBFS, per channel, and AFTER the preset's own
//     high-pass/low-pass filters (what actually reaches the speaker).
#include "ArcadeAmpDSP.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

static int g_fail = 0;
#define CHECK(cond, ...) do { if (cond) { printf("  PASS  "); } else { printf("  FAIL  "); ++g_fail; } printf(__VA_ARGS__); printf("\n"); } while (0)

static double rms(const std::vector<float>& v)
{
    double s = 0; for (float x : v) s += (double)x * x; return std::sqrt(s / v.size());
}
static double db(double lin) { return 20.0 * std::log10(lin + 1e-30); }
static double peak(const std::vector<float>& v)
{
    double p = 0; for (float x : v) p = std::max(p, (double)std::fabs(x)); return p;
}

// Goertzel: amplitude of a single frequency component.
static double goertzel(const std::vector<float>& x, double f, double sr)
{
    const double w = 2.0 * M_PI * f / sr, c = 2.0 * std::cos(w);
    double s1 = 0, s2 = 0;
    for (float v : x) { double s = v + c * s1 - s2; s2 = s1; s1 = s; }
    double re = s1 - s2 * std::cos(w), im = s2 * std::sin(w);
    return 2.0 * std::sqrt(re * re + im * im) / x.size();
}

// RBJ biquad (the same family Equalizer APO's HP/LP filters belong to).
struct Biquad {
    double b0, b1, b2, a1, a2, z1 = 0, z2 = 0;
    static Biquad hp(double fc, double q, double sr) {
        double w = 2 * M_PI * fc / sr, c = std::cos(w), a = std::sin(w) / (2 * q);
        double a0 = 1 + a; Biquad f;
        f.b0 = (1 + c) / 2 / a0; f.b1 = -(1 + c) / a0; f.b2 = (1 + c) / 2 / a0;
        f.a1 = -2 * c / a0; f.a2 = (1 - a) / a0; return f;
    }
    static Biquad lp(double fc, double q, double sr) {
        double w = 2 * M_PI * fc / sr, c = std::cos(w), a = std::sin(w) / (2 * q);
        double a0 = 1 + a; Biquad f;
        f.b0 = (1 - c) / 2 / a0; f.b1 = (1 - c) / a0; f.b2 = (1 - c) / 2 / a0;
        f.a1 = -2 * c / a0; f.a2 = (1 - a) / a0; return f;
    }
    float run(float x) {
        double y = b0 * x + z1; z1 = b1 * x - a1 * y + z2; z2 = b2 * x - a2 * y; return (float)y;
    }
};

static void runStereo(ArcadeAmpDSP& d, const std::vector<float>& inL, const std::vector<float>& inR,
                      std::vector<float>& outL, std::vector<float>& outR)
{
    outL.assign(inL.size(), 0.f); outR.assign(inR.size(), 0.f);
    const float* in[2]  = { inL.data(), inR.data() };
    float*       out[2] = { outL.data(), outR.data() };
    // feed in awkward block sizes, like a real host
    size_t pos = 0; const uint32_t blocks[] = { 64, 100, 480, 1, 333, 1024 }; int bi = 0;
    while (pos < inL.size()) {
        uint32_t n = std::min<size_t>(blocks[bi++ % 6], inL.size() - pos);
        const float* i2[2] = { in[0] + pos, in[1] + pos };
        float*       o2[2] = { out[0] + pos, out[1] + pos };
        d.process(i2, o2, n, 2); pos += n;
    }
}

static std::vector<float> tone(double amp, double f, double sr, size_t n)
{
    std::vector<float> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = (float)(amp * std::sin(2 * M_PI * f * i / sr));
    return v;
}


typedef ArcadeAmpParams P;
static P mk(float drive = 0, float warmth = 0, float sag = 0, float hum = 0, float hiss = 0, float tone = 50, float whine = 0, bool m50 = false)
{ P p; p.drive = drive; p.warmth = warmth; p.sag = sag; p.hum = hum; p.hiss = hiss; p.hissTone = tone; p.whine = whine; p.mains50 = m50; return p; }
static ArcadeAmpDSP* mkdsp(double sr, const P& p) { ArcadeAmpDSP* d = new ArcadeAmpDSP(); d->setSampleRate(sr); d->setParameters(p); d->reset(); return d; }

int main()
{
    const double sr = 48000.0; const size_t N = 96000; // 2 s
    std::vector<float> aL, aR;
    std::vector<float> z(N, 0.f);

    printf("== 1. Exact bypass when everything is 0 ==\n");
    {
        ArcadeAmpDSP* d = mkdsp(sr, mk());
        auto x = tone(0.3, 440, sr, N);
        runStereo(*d, x, x, aL, aR);
        double md = 0; for (size_t i = 0; i < N; ++i) md = std::max(md, (double)std::fabs(aL[i] - x[i]));
        CHECK(md == 0.0, "output identical to input (max difference %.3g)", md);
        runStereo(*d, z, z, aL, aR);
        CHECK(peak(aL) == 0.0 && peak(aR) == 0.0, "silence in -> silence out");
        delete d;
    }

    printf("\n== 2. Level neutrality (the old 'volume jump'): Drive, Warmth, Sag combined, MAME-realistic levels ==\n");
    printf("      (v0.1: +2.2 dB at Drive 0, +5.5 dB at 12 %%, +8.1 dB at 25 %%)\n");
    for (double amp : { 0.30, 0.01 }) {
        struct { float d, w, s; } c[] = { {5,0,0}, {12,0,0}, {25,0,0}, {100,0,0}, {12,15,10}, {25,25,20}, {25,100,100}, {100,100,100} };
        for (auto& t : c) {
            ArcadeAmpDSP* d = mkdsp(sr, mk(t.d, t.w, t.s));
            auto x = tone(amp, 440, sr, N);
            runStereo(*d, x, x, aL, aR);
            // skip first 0.25 s (sag/DC-blocker settling) for the RMS comparison
            std::vector<float> y(aL.begin() + 12000, aL.end()), xr(x.begin() + 12000, x.end());
            double change = db(rms(y)) - db(rms(xr));
            double pk = peak(y) / peak(xr);
            CHECK(change <= 0.05 && pk <= 1.03, "tone peak %.2f  Drive %3.0f Warmth %3.0f Sag %3.0f: RMS change %+6.2f dB, peak ratio %.3f", amp, t.d, t.w, t.s, change, pk);
            delete d;
        }
    }

    printf("\n== 3. Strictly never louder per sample when Warmth = 0 and DC-block inactive (hostile +-4.0 input) ==\n");
    {
        ArcadeAmpDSP* d = mkdsp(sr, mk(100, 0, 100));
        std::vector<float> x(N); uint32_t r = 12345;
        for (auto& v : x) { r = r * 1664525u + 1013904223u; v = ((r >> 8) / 8388608.0f - 1.0f) * 4.0f; }
        runStereo(*d, x, x, aL, aR);
        bool ok = true, fin = true;
        for (size_t i = 0; i < N; ++i) { if (std::fabs(aL[i]) > std::fabs(x[i]) + 1e-6f) ok = false; if (!std::isfinite(aL[i])) fin = false; }
        CHECK(ok, "Drive 100 + Sag 100: |out| <= |in| on every sample");
        CHECK(fin, "no NaN/Inf");
        delete d;
        d = mkdsp(sr, mk(100, 100, 100, 100, 100, 100, 100));
        runStereo(*d, x, x, aL, aR);
        fin = true; for (size_t i = 0; i < N; ++i) if (!std::isfinite(aL[i]) || !std::isfinite(aR[i])) fin = false;
        CHECK(fin && peak(aL) < 10.0, "everything at 100 on hostile input: finite, peak %.2f", peak(aL));
        delete d;
    }

    printf("\n== 4. Small-signal gain 0 dB (-60 dBFS tone) ==\n");
    for (int k = 0; k < 3; ++k) {
        P p = k == 0 ? mk(100) : k == 1 ? mk(25, 100) : mk(25, 25, 100);
        ArcadeAmpDSP* d = mkdsp(sr, p);
        auto x = tone(0.001, 1000, sr, N); runStereo(*d, x, x, aL, aR);
        std::vector<float> y(aL.begin() + 12000, aL.end()), xr(x.begin() + 12000, x.end());
        double ch = db(rms(y)) - db(rms(xr));
        CHECK(std::fabs(ch) < 0.05, "Drive %.0f Warmth %.0f Sag %.0f: gain %+.4f dB", p.drive, p.warmth, p.sag, ch);
        delete d;
    }

    printf("\n== 5. Warmth makes even harmonics; DC is removed; Warmth 0 makes none ==\n");
    {
        auto x = tone(0.5, 500, sr, N);
        for (float w : { 0.f, 50.f, 100.f }) {
            ArcadeAmpDSP* d = mkdsp(sr, mk(40, w)); runStereo(*d, x, x, aL, aR);
            std::vector<float> y(aL.begin() + 12000, aL.end());
            double h1 = goertzel(y, 500, sr), h2 = goertzel(y, 1000, sr), h3 = goertzel(y, 1500, sr);
            double mean = 0; for (float v : y) mean += v; mean /= y.size();
            if (w == 0.f) CHECK(h2 < 1e-4 * h1 && std::fabs(mean) < 1e-4, "Warmth   0: 2nd harmonic %.1f dB re fundamental (none), DC %+.5f", db(h2 / h1), mean);
            else          CHECK(h2 > 0.005 * h1 && std::fabs(mean) < 5e-3, "Warmth %3.0f: 2nd harmonic %.1f dB, 3rd %.1f dB re fundamental, DC %+.5f", w, db(h2 / h1), db(h3 / h1), mean);
            delete d;
        }
    }

    printf("\n== 6. Sag: level dependent, never louder, recovers ==\n");
    {
        // loud tone vs quiet tone at same Sag
        double g[2]; double amps[2] = { 0.05, 0.6 };
        for (int k = 0; k < 2; ++k) {
            ArcadeAmpDSP* d = mkdsp(sr, mk(0, 0, 50)); auto x = tone(amps[k], 300, sr, N); runStereo(*d, x, x, aL, aR);
            std::vector<float> y(aL.begin() + 24000, aL.end()), xr(x.begin() + 24000, x.end());
            g[k] = db(rms(y)) - db(rms(xr)); delete d;
        }
        CHECK(g[1] < g[0] - 0.5 && g[0] <= 0.0 && g[1] <= 0.0, "Sag 50: peak 0.05 -> %+.2f dB, peak 0.6 -> %+.2f dB (loud loses more, neither louder)", g[0], g[1]);
        // burst then quiet: gain recovers
        std::vector<float> x(sr * 2, 0.f); for (size_t i = 0; i < 24000; ++i) x[i] = 0.7f * (float)std::sin(2 * M_PI * 300 * i / sr);
        for (size_t i = 24000; i < x.size(); ++i) x[i] = 0.05f * (float)std::sin(2 * M_PI * 300 * i / sr);
        ArcadeAmpDSP* d = mkdsp(sr, mk(0, 0, 100)); runStereo(*d, x, x, aL, aR);
        auto seg = [&](size_t a, size_t b) { std::vector<float> y(aL.begin() + a, aL.begin() + b), xr(x.begin() + a, x.begin() + b); return db(rms(y)) - db(rms(xr)); };
        double early = seg(24000, 26400), late = seg(70000, 96000);
        // steady-state gain of the quiet tone alone, for comparison
        ArcadeAmpDSP* q = mkdsp(sr, mk(0, 0, 100)); auto xq = tone(0.05, 300, sr, N); std::vector<float> ql, qr; runStereo(*q, xq, xq, ql, qr);
        std::vector<float> qy(ql.begin() + 70000, ql.end()), qx(xq.begin() + 70000, xq.end()); double steady = db(rms(qy)) - db(rms(qx)); delete q;
        CHECK(early < late - 3.0 && std::fabs(late - steady) < 0.3, "after a 0.7-peak burst the 0.05 tone is %+.2f dB at +50 ms, back to %+.2f dB later (its own steady state: %+.2f dB)", early, late, steady);
        delete d;
    }

    printf("\n== 7. Hum / Hiss levels, stereo identity, DC ==\n");
    for (int which = 0; which < 2; ++which)
        for (float nz : { 5.f, 25.f, 50.f, 100.f }) {
            ArcadeAmpDSP* d = mkdsp(sr, which == 0 ? mk(0, 0, 0, nz) : mk(0, 0, 0, 0, nz));
            runStereo(*d, z, z, aL, aR);
            double md = 0, mean = 0; for (size_t i = 0; i < N; ++i) { md = std::max(md, (double)std::fabs(aL[i] - aR[i])); mean += aL[i]; } mean /= N;
            double expected = -54.0 + 0.30 * nz, L = db(rms(aL)), R = db(rms(aR));
            CHECK(std::fabs(L - expected) < 0.7 && std::fabs(R - expected) < 0.7 && md == 0.0 && std::fabs(mean) < 5e-3,
                  "%s %5.1f%%: L %6.1f R %6.1f dBFS (target %.1f), L==R, DC %+.5f", which == 0 ? "Hum " : "Hiss", nz, L, R, expected, mean);
            delete d;
        }

    printf("\n== 8. Hiss Tone: same loudness, different colour ==\n");
    {
        double lvl[3], hf[3], lf[3]; float tones[3] = { 0, 50, 100 };
        for (int k = 0; k < 3; ++k) {
            ArcadeAmpDSP* d = mkdsp(sr, mk(0, 0, 0, 0, 50, tones[k])); runStereo(*d, z, z, aL, aR);
            lvl[k] = db(rms(aL));
            // crude band energies: highpass 6 kHz vs lowpass 1 kHz
            Biquad h = Biquad::hp(6000, 0.71, sr), l = Biquad::lp(1000, 0.71, sr);
            std::vector<float> a(N), b(N); for (size_t i = 0; i < N; ++i) { a[i] = h.run(aL[i]); b[i] = l.run(aL[i]); }
            hf[k] = db(rms(a)); lf[k] = db(rms(b)); delete d;
        }
        CHECK(std::fabs(lvl[0] - lvl[2]) < 1.0 && std::fabs(lvl[1] - lvl[2]) < 1.0, "overall level at tone 0/50/100: %.1f / %.1f / %.1f dBFS", lvl[0], lvl[1], lvl[2]);
        CHECK(hf[2] > hf[0] + 6.0 && lf[0] > lf[2] + 3.0, "tone 0 -> 100: energy above 6 kHz rises %.1f dB, below 1 kHz falls %.1f dB", hf[2] - hf[0], lf[0] - lf[2]);
    }

    printf("\n== 9. Whine: frequency, level, silent on low sample rates ==\n");
    {
        for (double s : { 44100.0, 48000.0, 96000.0 }) {
            ArcadeAmpDSP* d = mkdsp(s, mk(0, 0, 0, 0, 0, 50, 100)); std::vector<float> zz((size_t)(2 * s), 0.f), l, r; runStereo(*d, zz, zz, l, r);
            double at = goertzel(l, ArcadeAmpDSP::kWhineHz, s), lo = goertzel(l, ArcadeAmpDSP::kWhineHz - 400, s), hi = goertzel(l, ArcadeAmpDSP::kWhineHz + 400, s);
            double expected = -30.0;
            double L = db(rms(l));
            CHECK(at > 100 * lo && at > 100 * hi && std::fabs(L - expected) < 0.5, "%6.0f Hz: 15734 Hz bin %.4f vs +-400 Hz %.6f/%.6f, level %.1f dBFS (target %.1f)", s, at, lo, hi, L, expected);
            delete d;
        }
        ArcadeAmpDSP* d = mkdsp(32000.0, mk(0, 0, 0, 0, 0, 50, 100)); std::vector<float> zz(64000, 0.f), l, r; runStereo(*d, zz, zz, l, r);
        CHECK(peak(l) == 0.0, "32 kHz sample rate (Nyquist 16 kHz): whine stays silent instead of aliasing");
        delete d;
    }

    printf("\n== 10. Hum spectrum: 2nd harmonic dominant, 60 and 50 Hz, other rates ==\n");
    for (bool m50 : { false, true }) {
        ArcadeAmpDSP* d = mkdsp(sr, mk(0, 0, 0, 100, 0, 50, 0, m50)); runStereo(*d, z, z, aL, aR);
        double f0 = m50 ? 50.0 : 60.0;
        double a1 = goertzel(aL, f0, sr), a2 = goertzel(aL, 2 * f0, sr), a3 = goertzel(aL, 3 * f0, sr), a4 = goertzel(aL, 4 * f0, sr);
        CHECK(a2 > a1 && a2 > a3 && a2 > a4 && a2 > 10.0 * goertzel(aL, 1.5 * f0, sr), "%2.0f Hz mains: 1st %.4f 2nd %.4f 3rd %.4f 4th %.4f -> 2nd dominates, energy on harmonics", f0, a1, a2, a3, a4);
        delete d;
    }
    for (double s : { 44100.0, 96000.0 }) {
        ArcadeAmpDSP* d = mkdsp(s, mk(0, 0, 0, 100)); std::vector<float> zz((size_t)(2 * s), 0.f), l, r; runStereo(*d, zz, zz, l, r);
        double a = goertzel(l, 120.0, s); CHECK(a > 10 * goertzel(l, 117.0, s) && a > 10 * goertzel(l, 123.0, s), "%6.0f Hz: 120 Hz bin %.4f, neighbours low", s, a);
        delete d;
    }

    printf("\n== 11. What reaches the speaker: Hum 25 after the MVS preset's HP 85 Hz + LP 7 kHz ==\n");
    {
        ArcadeAmpDSP* d = mkdsp(sr, mk(0, 0, 0, 25)); runStereo(*d, z, z, aL, aR);
        Biquad hp = Biquad::hp(85, 0.71, sr), lp = Biquad::lp(7000, 0.71, sr);
        std::vector<float> f(N); for (size_t i = 0; i < N; ++i) f[i] = lp.run(hp.run(aL[i]));
        double before = db(rms(aL)), after = db(rms(f));
        CHECK(after > -52.0, "Hum 25: %.1f dBFS before, %.1f dBFS after the MVS filters (v0.1 sine hum lost 7-10 dB here)", before, after);
        delete d;
    }

    printf("\n== 12. Presets ==\n");
    {
        P c = mk(77, 77, 77, 77, 77, 77, 77, true);
        P cu = ArcadeAmpDSP::presetParams(ArcadeAmpDSP::kCustom, c);
        CHECK(cu.drive == 77 && cu.warmth == 77 && cu.whine == 77 && cu.mains50, "Custom passes the sliders through untouched");
        const char* names[] = { "Custom", "Light", "Medium", "Heavy", "TV early", "TV late" };
        for (int p = 1; p < ArcadeAmpDSP::kPresetCount; ++p) {
            P q = ArcadeAmpDSP::presetParams(p, c);
            printf("  INFO  %-9s Drive %2.0f Warmth %2.0f Sag %2.0f Hum %2.0f Hiss %2.0f Tone %2.0f Whine %2.0f\n", names[p], q.drive, q.warmth, q.sag, q.hum, q.hiss, q.hissTone, q.whine);
            CHECK(q.mains50 == true, "%s keeps the user's Mains setting", names[p]);
            // level neutrality of the processing part (noise off)
            P nn = q; nn.hum = nn.hiss = nn.whine = 0;
            ArcadeAmpDSP* d = mkdsp(sr, nn); auto x = tone(0.3, 440, sr, N); runStereo(*d, x, x, aL, aR);
            std::vector<float> y(aL.begin() + 12000, aL.end()), xr(x.begin() + 12000, x.end());
            double ch = db(rms(y)) - db(rms(xr));
            CHECK(ch <= 0.05 && ch > -3.0, "%s: processing changes a 0.3-peak tone by %+.2f dB", names[p], ch);
            delete d;
        }
    }

    printf("\n== 13. Runtime changes: smooth, finite, mains switch on the same instance ==\n");
    {
        ArcadeAmpDSP* d = mkdsp(sr, mk(0, 0, 0, 100)); std::vector<float> l1, r1, l2, r2; auto x = tone(0.3, 440, sr, N / 2);
        runStereo(*d, x, x, l1, r1);
        d->setParameters(mk(100, 100, 100, 100, 100, 100, 100, true)); runStereo(*d, x, x, l2, r2);
        bool fin = true; double mj = 0; for (size_t i = 1; i < l2.size(); ++i) { if (!std::isfinite(l2[i])) fin = false; mj = std::max(mj, (double)std::fabs(l2[i] - l2[i - 1])); }
        CHECK(fin, "no NaN/Inf after slamming every control");
        CHECK(mj < 1.0, "no giant discontinuity (max sample-to-sample jump %.3f)", mj);
        std::vector<float> q1, q2, qa, qb; ArcadeAmpDSP* q = mkdsp(sr, mk(0, 0, 0, 100, 0, 50, 0, false));
        runStereo(*q, z, z, q1, qa); q->setParameters(mk(0, 0, 0, 100, 0, 50, 0, true)); runStereo(*q, z, z, q2, qb);
        CHECK(goertzel(q1, 120.0, sr) > 5 * goertzel(q1, 100.0, sr), "60 Hz mains: 120 Hz buzz, no 100 Hz");
        CHECK(goertzel(q2, 100.0, sr) > 5 * goertzel(q2, 120.0, sr), "flipped to 50 Hz on the same instance: 100 Hz buzz, no 120 Hz");
        delete q; delete d;
    }

    printf("\n== 14. In-place processing equals out-of-place ==\n");
    {
        P p = mk(25, 25, 20, 25, 20, 35, 25);
        ArcadeAmpDSP* d1 = mkdsp(sr, p); ArcadeAmpDSP* d2 = mkdsp(sr, p);
        auto x = tone(0.3, 440, sr, 4800); std::vector<float> inL = x, inR = x, oL(4800), oR(4800);
        const float* in[2] = { inL.data(), inR.data() }; float* out[2] = { oL.data(), oR.data() }; d1->process(in, out, 4800, 2);
        std::vector<float> pL = x, pR = x; float* pp[2] = { pL.data(), pR.data() }; d2->process(pp, pp, 4800, 2);
        double md = 0; for (size_t i = 0; i < 4800; ++i) md = std::max(md, (double)std::fabs(oL[i] - pL[i]));
        CHECK(md == 0.0, "identical (max difference %.3g)", md);
        delete d1; delete d2;
    }

    printf("\n%s (%d failure%s)\n", g_fail ? "TESTS FAILED" : "ALL TESTS PASSED", g_fail, g_fail == 1 ? "" : "s");
    return g_fail ? 1 : 0;
}
