/*
 * ArcadeAmp - plugin wrapper (DISTRHO Plugin Framework, ISC) around the
 * DSP in ArcadeAmpDSP.hpp. All documentation of what the DSP does and what
 * is real data versus approximation lives in that header and in
 * ../README.md.
 *
 * Parameters: Preset (one-click recipes) plus Drive, Warmth, Sag, Hum, Hiss,
 * Hiss Tone, Whine, Mains 50Hz. Preset != Custom overrides all but Mains.
 *
 * License: GPL-2.0-or-later (see ../LICENSE.txt).
 */

#include "DistrhoPlugin.hpp"
#include "ArcadeAmpDSP.hpp"
#include "ArcadeAmpIds.hpp"

START_NAMESPACE_DISTRHO

static const char* const kPresetNames[ArcadeAmpDSP::kPresetCount] = {
    "Custom", "Light", "Medium", "Heavy", "TV early", "TV late"
};

class ArcadeAmpPlugin : public Plugin
{
public:
    ArcadeAmpPlugin()
        : Plugin(kParamCount, 0, 0)
    {
        fValues[kParamPreset] = 0.0f;
        fValues[kParamDrive] = 10.0f;
        fValues[kParamWarmth] = 0.0f;
        fValues[kParamSag] = 0.0f;
        fValues[kParamHum] = 0.0f;
        fValues[kParamHiss] = 0.0f;
        fValues[kParamHissTone] = 50.0f;
        fValues[kParamWhine] = 0.0f;
        fValues[kParamMains50] = 0.0f;
        fDsp.setSampleRate(getSampleRate());
        applyParameters();
        fDsp.reset();
    }

protected:
    const char* getLabel() const override       { return "ArcadeAmp"; }
    const char* getDescription() const override
    {
        return "Open-source amplifier/TV character stage for the Arcade Cabinet Acoustic Emulator for MAME: "
               "level-neutral saturation, asymmetry, supply sag, mains hum, hiss and CRT line-frequency whine. "
               "Generic approximations, not measured models of any specific amplifier.";
    }
    const char* getMaker() const override       { return "Arcade Cabinet Acoustic Emulator for MAME project"; }
    const char* getHomePage() const override    { return DISTRHO_PLUGIN_URI; }
    const char* getLicense() const override     { return "GPL-2.0-or-later"; }
    uint32_t getVersion() const override        { return d_version(0, 4, 0); }
    int64_t getUniqueId() const override        { return d_cconst('A','r','c','A'); }

    static void percent(Parameter& p, const char* name, const char* symbol, float def, const char* desc)
    {
        p.name = name; p.symbol = symbol; p.unit = "%";
        p.ranges.min = 0.0f; p.ranges.max = 100.0f; p.ranges.def = def;
        p.description = desc;
    }

    void initParameter(uint32_t index, Parameter& parameter) override
    {
        parameter.hints = kParameterIsAutomatable;
        switch (index)
        {
        case kParamPreset:
        {
            parameter.name = "Preset"; parameter.symbol = "preset";
            // NOTE: deliberately NOT kParameterIsInteger. DPF's VST2 wrapper reports integer
            // limits in a layout Equalizer APO reads as min=max=0, and its panel crashed on it.
            parameter.ranges.min = 0.0f; parameter.ranges.max = float(ArcadeAmpDSP::kPresetCount - 1);
            parameter.ranges.def = 0.0f;
            parameter.description = "One-click recipe. Custom = use the sliders below. Any other choice "
                                    "overrides every slider except Mains 50Hz. See presets/experimental-vst/README.md.";
            ParameterEnumerationValue* const v = new ParameterEnumerationValue[ArcadeAmpDSP::kPresetCount];
            for (int i = 0; i < ArcadeAmpDSP::kPresetCount; ++i) { v[i].label = kPresetNames[i]; v[i].value = float(i); }
            parameter.enumValues.count = ArcadeAmpDSP::kPresetCount;
            parameter.enumValues.restrictedMode = true;
            parameter.enumValues.values = v;
            break;
        }
        case kParamDrive:
            percent(parameter, "Drive", "drive", 10.0f,
                    "Saturation. 0 = off. Never makes quiet sound louder; only rounds off loud peaks.");
            break;
        case kParamWarmth:
            percent(parameter, "Warmth", "warmth", 0.0f,
                    "Asymmetry: adds even harmonics like a single-ended amplifier stage. Needs Drive above 0.");
            break;
        case kParamSag:
            percent(parameter, "Sag", "sag", 0.0f,
                    "Power-supply droop: loud passages briefly lose gain, then recover. Never louder than the input.");
            break;
        case kParamHum:
            percent(parameter, "Hum", "hum", 0.0f,
                    "Mains buzz (harmonics of 50/60 Hz, 2nd dominant). 20-30 is plainly audible in quiet moments.");
            break;
        case kParamHiss:
            percent(parameter, "Hiss", "hiss", 0.0f, "Broadband amplifier hiss.");
            break;
        case kParamHissTone:
            percent(parameter, "Hiss Tone", "hisstone", 50.0f, "0 = dark/rumbly hiss, 100 = bright hiss.");
            parameter.unit = "";
            break;
        case kParamWhine:
            percent(parameter, "Whine", "whine", 0.0f,
                    "CRT horizontal-scan whine at 15,734 Hz (NTSC/NTSC-J line rate). Very high pitched; "
                    "inaudible on small speakers and to many adults.");
            break;
        case kParamMains50:
            parameter.name = "Mains 50Hz"; parameter.symbol = "mains50";
            parameter.hints |= kParameterIsBoolean;
            parameter.ranges.min = 0.0f; parameter.ranges.max = 1.0f; parameter.ranges.def = 0.0f;
            parameter.description = "Off = 60 Hz mains (western Japan/Osaka, North America). "
                                    "On = 50 Hz mains (eastern Japan/Tokyo, Europe). Only affects Hum.";
            {
                ParameterEnumerationValue* const v = new ParameterEnumerationValue[2];
                v[0].label = "60 Hz"; v[0].value = 0.0f;
                v[1].label = "50 Hz"; v[1].value = 1.0f;
                parameter.enumValues.count = 2;
                parameter.enumValues.restrictedMode = true;
                parameter.enumValues.values = v;
            }
            break;
        }
    }

    float getParameterValue(uint32_t index) const override
    {
        return index < kParamCount ? fValues[index] : 0.0f;
    }

    void setParameterValue(uint32_t index, float value) override
    {
        if (index < kParamCount) fValues[index] = value;
        applyParameters();
    }

    void sampleRateChanged(double newSampleRate) override
    {
        fDsp.setSampleRate(newSampleRate);
        applyParameters();
    }

    void activate() override { fDsp.reset(); }

    void run(const float** inputs, float** outputs, uint32_t frames) override
    {
        fDsp.process(inputs, outputs, frames, DISTRHO_PLUGIN_NUM_OUTPUTS);
    }

private:
    void applyParameters()
    {
        ArcadeAmpParams c;
        c.drive = fValues[kParamDrive];
        c.warmth = fValues[kParamWarmth];
        c.sag = fValues[kParamSag];
        c.hum = fValues[kParamHum];
        c.hiss = fValues[kParamHiss];
        c.hissTone = fValues[kParamHissTone];
        c.whine = fValues[kParamWhine];
        c.mains50 = fValues[kParamMains50] >= 0.5f;
        int preset = int(fValues[kParamPreset] + 0.5f);
        if (preset < 0 || preset >= ArcadeAmpDSP::kPresetCount) preset = 0;
        fDsp.setParameters(ArcadeAmpDSP::presetParams(preset, c));
    }

    ArcadeAmpDSP fDsp;
    float fValues[kParamCount];

    DISTRHO_DECLARE_NON_COPYABLE(ArcadeAmpPlugin)
};

Plugin* createPlugin()
{
    return new ArcadeAmpPlugin();
}

END_NAMESPACE_DISTRHO
