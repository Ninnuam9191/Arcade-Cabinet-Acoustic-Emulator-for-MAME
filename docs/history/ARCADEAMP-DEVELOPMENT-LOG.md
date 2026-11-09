# ArcadeAmp development log

Every ArcadeAmp release was driven by a real field report. For each one this
log records what was reported, how the cause was found, what was changed, and
how it was checked. Numbers are from the recorded test outputs in
`vst-plugin/tests/`.

Evidence labels used below: **MEASURED** here means measured in this project's
test programs, never a measurement of a real cabinet.

## Why a custom plugin

Equalizer APO's filters are linear. A real amplifier also saturates, droops
under load, and carries hum and hiss. Two ready-made plugins were tried and
failed on stock Equalizer APO (see `docs/research/VST.md`): PurestDrive (no
reliably sourced VST2 build), PeakEater (CLAP/LV2/VST3 only). Stock Equalizer
APO hosts **VST2** only; Steinberg withdrew the VST2 SDK in 2018, which is why
few maintained free plugins still ship VST2.

Build choices: DISTRHO Plugin Framework (ISC licence); the VST2 build uses a
GPL-2.0-or-later header, so ArcadeAmp is GPL-2.0-or-later; cross-compiled from
Linux to Windows with mingw-w64; DSP in a framework-free header
(`source/ArcadeAmpDSP.hpp`) so the exact shipped code is what the tests run.

## v0.1: first plugin

Five controls: Drive, Mix, Noise Level, Noise Color, Mains Hz.

### Field report (project owner, Equalizer APO 1.4.2)

* The volume changed noticeably and the Preamplification row had to be turned
  down.
* Drive was audible, but it was not clear whether it was drive or clipping.
* No result from the mains buzz or the other colour options.
* The preset text listed several parameters while the editor row showed only
  Drive and Noise.
* The setup was "a bit complex for everyday users".

### Investigation

The v0.1 code was re-created and measured at **MAME-realistic levels** (a
0.3-peak tone, not a full-scale tone). The earlier unit tests had used full
scale on one channel, which is why they passed and still missed all of this.

| Reported | Cause found | v0.1 measured | After fix |
|---|---|---|---|
| Volume jumps up | "Gain compensation" `tanh(x*g)/tanh(g)` was normalised for a full-scale peak, so quiet audio was boosted | +2.2 dB at Drive 0 %, +5.5 dB at 12 %, +8.1 dB at 25 % | Drive 5 / 12 / 25 %: -0.05 / -0.27 / -1.09 dB. Never louder |
| Drive vs clipping | The boost pushed signals into clipping further down the chain | n/a | Output can never exceed input level |
| No buzz (right channel) | The hum oscillator only advanced during the left channel's pass, so the right channel's hum was frozen | Right channel: no hum | Frame-outer, channel-inner loop; one shared noise sample; left and right bit-identical |
| No buzz (left channel) | Hiss sat near -65 dBFS and the hum was a pure sine at 60 and 120 Hz, which the Neo Geo MVS profile's own 85 Hz high-pass cuts by 7 to 10 dB | Left -49.7 dBFS, right -65 dBFS | Harmonic-rich buzz; Hum 25 % is -46.5 dBFS and about -47.4 dBFS after the MVS filters |
| Too complex | Five controls, two overlapping | 5 | Fewer, then Presets (below) |
| Preset text disagreed with the editor | The earlier per-profile experimental preset files had drifted from the audited presets (different routing, different Preamp), which likely contributed to the volume change | n/a | Those files were deleted; settings are now a table applied to the real profile |

## v0.2: bug fixes

Fixed the above. Controls cut to three (Drive, Noise, Mains 50Hz). Tests
rewritten to run the real two-channel path at realistic levels. Later feedback:
removing features was not the intent, since the purpose of the project is to
reproduce cabinet and TV sound qualities.

## v0.3: features restored and extended

Added, each with its evidence label (see `vst-plugin/README.md`):

* **Warmth** (asymmetric clipping, even harmonics; INFERRED, amount EXPERIMENTAL)
* **Sag** (level-dependent gain from supply droop; INFERRED, constants EXPERIMENTAL)
* **Hum**, **Hiss**, **Hiss Tone** (EXPERIMENTAL levels; hum harmonic pattern INFERRED from full-wave rectifier ripple at twice the mains frequency; 50/60 Hz DOCUMENTED, with Japan split east/west)
* **Whine** at 15,734.26 Hz (NTSC / NTSC-J line rate; frequency DOCUMENTED, level EXPERIMENTAL)
* **Presets**: Custom, Light, Medium, Heavy, TV early, TV late, so ordinary users choose one button.

Calibration note: the first test run showed Heavy lowering a 0.3-peak tone by
2.5 dB, which is a volume change again, so the Sag depth was halved (now Light
-0.05 dB, Medium -0.70 dB, Heavy -1.81 dB, never louder).

Verified: 14 groups of DSP tests (65 checks), plus a host that loads the real
plugin wrapper through the VST2 API.

### Field report: Open panel crashed Equalizer APO

> "when I press open panel on the vst selection in eq apo it crashes"

Windows Event 1000 (Application Error), values supplied by the project owner:

| Field | Value |
|---|---|
| Faulting application and module | `Editor.exe`, version 1.4.2.0 (Equalizer APO's own Configuration Editor) |
| Exception code | `c0000005` (access violation) |
| Fault offset | `0x0000000000091315` |

The faulting module was Equalizer APO's editor, **not** the plugin. That
redirected the investigation to Equalizer APO's source code.

### Investigation of the crash

1. **First hypothesis (wrong as the main cause, but a real defect):** the new
   Preset control was flagged as an integer parameter. The plugin framework
   writes the integer limits in a layout hosts read as `min = 0, max = 0`
   (measured: flags say "integer limits" while the limits read 0..0).
   Removed, with a test that fails if it returns. The crash persisted for the
   project owner, so this was not the cause.
2. **Equalizer APO's open-source code** (GitHub mirror `mirror/equalizerapo`,
   files `Editor/guis/VSTPluginFilterGUI.cpp`,
   `helpers/VSTPluginInstance.cpp`, `filters/VSTPluginFilterFactory.cpp`) was
   read. Findings, all later relied on:
   * **Open panel always asks the plugin for an editor window**
     (`effEditGetRect`, `effEditOpen`, `effEditGetRect`) and uses the returned
     rectangle without checking that the plugin has an editor. A plugin with
     no window leaves that pointer unset, and `Editor.exe` dereferences it:
     access violation `c0000005`. This matches the crash log.
   * The `VSTPlugin:` row is `Library <path> <name> <value> ...` with values
     on the plugin's 0 to 1 scale; names with spaces are quoted. A text file
     therefore *can* carry plugin settings (an earlier note in this project said
     otherwise and was corrected).
   * The green **+** on a row inserts the new row **above** that row
     (`FilterTable::addLine(line, before)` inserts before the clicked item).
     The setup guide originally said "under"; corrected.
   * The Editor's dialog has **Apply**, **Cancel** and **OK** buttons and an
     "Apply automatically" tick box (default off).
   * Equalizer APO's installer creates `VSTPlugins` in its install folder;
     the Editor warns if its audio service cannot read a plugin elsewhere.
3. **Conclusion:** a plugin used with Equalizer APO's Open panel **must** have
   its own window. v0.3 had none.

## v0.4: plugin window

A real window (DPF NanoVG/OpenGL UI): six preset buttons, seven sliders, a
60/50 Hz switch. While a preset is active the sliders show its values;
touching a slider switches to Custom starting from those values, so there is
no "my slider does nothing" trap. The window uses the same preset table as the
audio.

Build detail worth recording: the DPF `pugl` submodule must be fetched
(`--recurse-submodules`); a shallow clone leaves it empty and the build stops
at `pugl/pugl.h`.

### How the window was verified without the project owner's machine

`vst-plugin/tests/winui_test.cpp`, a Windows program run under Wine with a
virtual display, does what Equalizer APO's Editor does: get rectangle, open the
editor into a parent window, get rectangle again, run the idle loop. It then
sends real mouse messages (click Heavy, drag Hum, click 50 Hz) and checks the
plugin's parameter values (Heavy = 0.60; after the drag the preset is Custom
with Heavy's values copied and Hum at 0.65; mains = 1.00), then closes it.
Screenshots from that run are in `vst-plugin/docs/`. Recorded output:
`vst-plugin/tests/winui_test-output.txt`.

### Field report: it works

Project owner, running the v0.4 plugin in Equalizer APO on Windows: it works,
the window is fine, and the sound changes. In their words the effect "adds to
the sounds the characteristics needed", and it is not perfect without a
further stage (read here as the deferred convolution phase, which is the
project's stated next step).

This is a user report. It is **not** a measurement and it is one listener.

## Lessons recorded

1. Test the way the thing is really used: realistic levels, both channels, the
   real binary, and the real host's calling sequence.
2. Passing tests are not proof; three real bugs passed the first test suite.
3. Read the host's source when its behaviour is the question. The crash
   could not be explained from the plugin side.
4. A crash log's *faulting module* decides where to look.
5. Fix the first plausible cause only if it is verified. The integer-flag fix
   was real but not the cause; shipping it as "the fix" would have been wrong.
6. Keep measured numbers next to every claim, and say what was not verified.
