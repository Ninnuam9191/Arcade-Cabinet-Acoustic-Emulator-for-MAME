/*
 * ArcadeAmp - the plugin window.
 *
 * Why this exists: Equalizer APO's "Open panel" button assumes the plugin has
 * its own editor window (its Editor.exe asks the plugin for the window size
 * and crashes if there is none). So ArcadeAmp must have one. It is kept
 * deliberately plain: six preset buttons, seven sliders, one 60/50 Hz switch.
 *
 * Behaviour that matters:
 *  - While a preset is active the sliders show that preset's values. Touching
 *    a slider switches to Custom, starting from those values, so there is no
 *    "my slider does nothing" trap.
 *  - Everything is drawn from the same ArcadeAmpDSP::presetParams() table the
 *    audio uses, so what you see is what you hear.
 *
 * License: GPL-2.0-or-later (see ../LICENSE.txt).
 */

#include "DistrhoUI.hpp"
#include "ArcadeAmpDSP.hpp"
#include "ArcadeAmpIds.hpp"

#include <cmath>
#include <cstdio>

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

namespace {

const char* const kPresetLabels[ArcadeAmpDSP::kPresetCount] = { "Custom", "Light", "Medium", "Heavy", "TV early", "TV late" };

struct SliderDef { uint32_t param; const char* label; const char* note; };
const SliderDef kSliders[] = {
    { kParamDrive,    "Drive",     "round off loud peaks" },
    { kParamWarmth,   "Warmth",    "lopsided clipping" },
    { kParamSag,      "Sag",       "supply droop" },
    { kParamHum,      "Hum",       "mains buzz" },
    { kParamHiss,     "Hiss",      "amplifier hiss" },
    { kParamHissTone, "Hiss Tone", "dark to bright" },
    { kParamWhine,    "Whine",     "15.7 kHz CRT whine" },
};
const int kNumSliders = 7;

// layout
const float kMargin = 24.0f;
const float kPresetY = 112.0f, kPresetH = 34.0f, kPresetGap = 8.0f;
const float kSliderY0 = 206.0f, kSliderDy = 34.0f;
const float kTrackX0 = 150.0f, kTrackX1 = 380.0f;
const float kMainsY = 206.0f + 7 * 34.0f + 8.0f;

} // namespace

class ArcadeAmpUI : public UI
{
public:
    ArcadeAmpUI()
        : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT)
    {
        loadSharedResources();
        fVal[kParamPreset] = 0.0f;
        fVal[kParamDrive] = 10.0f;
        fVal[kParamWarmth] = 0.0f;
        fVal[kParamSag] = 0.0f;
        fVal[kParamHum] = 0.0f;
        fVal[kParamHiss] = 0.0f;
        fVal[kParamHissTone] = 50.0f;
        fVal[kParamWhine] = 0.0f;
        fVal[kParamMains50] = 0.0f;
    }

protected:
    void parameterChanged(uint32_t index, float value) override
    {
        if (index < kParamCount)
        {
            fVal[index] = value;
            repaint();
        }
    }

    // ---- helpers ----
    int presetIndex() const
    {
        int p = int(fVal[kParamPreset] + 0.5f);
        return (p < 0 || p >= ArcadeAmpDSP::kPresetCount) ? 0 : p;
    }

    ArcadeAmpParams custom() const
    {
        ArcadeAmpParams c;
        c.drive = fVal[kParamDrive]; c.warmth = fVal[kParamWarmth]; c.sag = fVal[kParamSag];
        c.hum = fVal[kParamHum]; c.hiss = fVal[kParamHiss]; c.hissTone = fVal[kParamHissTone];
        c.whine = fVal[kParamWhine]; c.mains50 = fVal[kParamMains50] >= 0.5f;
        return c;
    }

    // values the sliders should display right now
    float effective(uint32_t param) const
    {
        const ArcadeAmpParams p = ArcadeAmpDSP::presetParams(presetIndex(), custom());
        switch (param)
        {
        case kParamDrive: return p.drive;
        case kParamWarmth: return p.warmth;
        case kParamSag: return p.sag;
        case kParamHum: return p.hum;
        case kParamHiss: return p.hiss;
        case kParamHissTone: return p.hissTone;
        case kParamWhine: return p.whine;
        }
        return 0.0f;
    }

    void put(uint32_t index, float value)
    {
        fVal[index] = value;
        setParameterValue(index, value);
    }

    // Leaving a preset: copy what it showed into the sliders, then switch to Custom.
    void leavePreset()
    {
        if (presetIndex() == 0) return;
        for (int i = 0; i < kNumSliders; ++i)
            put(kSliders[i].param, effective(kSliders[i].param));
        put(kParamPreset, 0.0f);
    }

    static bool inside(double x, double y, float rx, float ry, float rw, float rh)
    {
        return x >= rx && x <= rx + rw && y >= ry && y <= ry + rh;
    }

    float presetW() const { return (float(getWidth()) - 2 * kMargin - 5 * kPresetGap) / 6.0f; }

    void setFromX(int slider, double x)
    {
        double t = (x - kTrackX0) / (kTrackX1 - kTrackX0);
        if (t < 0) t = 0; if (t > 1) t = 1;
        put(kSliders[slider].param, std::round(float(t) * 100.0f));
        repaint();
    }

    // ---- input ----
    bool onMouse(const MouseEvent& ev) override
    {
        if (ev.button != 1) return false;
        const double x = ev.pos.getX(), y = ev.pos.getY();

        if (!ev.press)
        {
            if (fDrag >= 0) { editParameter(kSliders[fDrag].param, false); fDrag = -1; return true; }
            return false;
        }

        for (int i = 0; i < ArcadeAmpDSP::kPresetCount; ++i)
        {
            const float rx = kMargin + i * (presetW() + kPresetGap);
            if (inside(x, y, rx, kPresetY, presetW(), kPresetH))
            {
                editParameter(kParamPreset, true);
                put(kParamPreset, float(i));
                editParameter(kParamPreset, false);
                repaint();
                return true;
            }
        }

        for (int i = 0; i < kNumSliders; ++i)
        {
            const float ry = kSliderY0 + i * kSliderDy;
            if (inside(x, y, kTrackX0 - 10, ry - 12, (kTrackX1 - kTrackX0) + 20, 24))
            {
                leavePreset();
                fDrag = i;
                editParameter(kSliders[i].param, true);
                setFromX(i, x);
                return true;
            }
        }

        for (int k = 0; k < 2; ++k)
        {
            const float rx = kTrackX0 + k * 88.0f;
            if (inside(x, y, rx, kMainsY, 80, 30))
            {
                editParameter(kParamMains50, true);
                put(kParamMains50, k == 1 ? 1.0f : 0.0f);
                editParameter(kParamMains50, false);
                repaint();
                return true;
            }
        }
        return false;
    }

    bool onMotion(const MotionEvent& ev) override
    {
        if (fDrag < 0) return false;
        setFromX(fDrag, ev.pos.getX());
        return true;
    }

    // ---- drawing ----
    void text(float x, float y, const char* s, float size, Color c, int align = ALIGN_LEFT | ALIGN_MIDDLE)
    {
        fontSize(size);
        fillColor(c);
        textAlign(align);
        NanoVG::text(x, y, s, nullptr);
    }

    void onNanoDisplay() override
    {
        const float W = float(getWidth()), H = float(getHeight());
        const Color bg(24, 26, 30), panel(36, 39, 46), accent(255, 176, 46), accentDim(120, 88, 30);
        const Color fg(235, 235, 235), dim(150, 155, 165), track(62, 66, 76);

        beginPath(); rect(0, 0, W, H); fillColor(bg); fill();

        fontFaceId(0); // shared DejaVu Sans loaded in the constructor
        text(kMargin, 36, "ArcadeAmp", 26, fg);
        text(kMargin, 62, "ArcadeAmp 0.4 - amplifier and CRT-TV character for the arcade emulator project", 12, dim);

        text(kMargin, 96, "Preset", 13, dim);
        const int cur = presetIndex();
        for (int i = 0; i < ArcadeAmpDSP::kPresetCount; ++i)
        {
            const float rx = kMargin + i * (presetW() + kPresetGap);
            beginPath(); roundedRect(rx, kPresetY, presetW(), kPresetH, 6);
            fillColor(i == cur ? accent : panel); fill();
            text(rx + presetW() / 2, kPresetY + kPresetH / 2, kPresetLabels[i], 13,
                 i == cur ? Color(20, 20, 20) : fg, ALIGN_CENTER | ALIGN_MIDDLE);
        }

        text(kMargin, 168, cur == 0 ? "Custom: drag the sliders." : "Preset active: sliders show its values. Drag one to switch to Custom.", 12, dim);
        text(kMargin, 186, "Sliders", 13, dim);

        for (int i = 0; i < kNumSliders; ++i)
        {
            const float ry = kSliderY0 + i * kSliderDy;
            const float v = effective(kSliders[i].param);
            const float t = v / 100.0f;
            text(kMargin, ry, kSliders[i].label, 14, fg);
            beginPath(); roundedRect(kTrackX0, ry - 3, kTrackX1 - kTrackX0, 6, 3); fillColor(track); fill();
            beginPath(); roundedRect(kTrackX0, ry - 3, (kTrackX1 - kTrackX0) * t, 6, 3);
            fillColor(cur == 0 ? accent : accentDim); fill();
            beginPath(); circle(kTrackX0 + (kTrackX1 - kTrackX0) * t, ry, 8);
            fillColor(cur == 0 ? fg : Color(190, 190, 195)); fill();
            char buf[16]; std::snprintf(buf, sizeof(buf), "%d", int(std::round(v)));
            text(kTrackX1 + 20, ry, buf, 14, fg);
            text(kTrackX1 + 54, ry, kSliders[i].note, 11, dim);
        }

        text(kMargin, kMainsY + 15, "Mains hum", 14, fg);
        for (int k = 0; k < 2; ++k)
        {
            const float rx = kTrackX0 + k * 88.0f;
            const bool on = (fVal[kParamMains50] >= 0.5f) == (k == 1);
            beginPath(); roundedRect(rx, kMainsY, 80, 30, 6); fillColor(on ? accent : panel); fill();
            text(rx + 40, kMainsY + 15, k == 0 ? "60 Hz" : "50 Hz", 13, on ? Color(20, 20, 20) : fg, ALIGN_CENTER | ALIGN_MIDDLE);
        }
        text(kTrackX0, kMainsY + 48, "50 Hz: Tokyo, Europe.   60 Hz: Osaka, North America.", 11, dim);

        text(kMargin, H - 14, "Generic effect, not a measured model of any cabinet or TV. Levels are starting points.", 11, dim);
    }

private:
    float fVal[kParamCount];
    int   fDrag = -1;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArcadeAmpUI)
};

UI* createUI()
{
    return new ArcadeAmpUI();
}

END_NAMESPACE_DISTRHO
