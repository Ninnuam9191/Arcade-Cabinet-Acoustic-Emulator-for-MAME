# Changelog

## v2.2: ArcadeAmp release, illustrated setup guide, research and test history

Merged on top of the existing repository; no earlier document was removed.

- **Illustrated beginner guide**: `docs/SETUP-GUIDE.md` with pictures in `docs/images/` (a real cropped Equalizer APO screenshot with markers, the real plugin window, two explanatory drawings). Lists which steps still lack real screenshots.
- **ArcadeAmp v0.4** (`vst-plugin/`): saturation, Warmth, Sag, Hum, Hiss, Hiss Tone, CRT Whine, six presets, and a plugin window. Works in Equalizer APO 1.4.2 on Windows per the project owner's report.
- **New documentation of the work itself**:
  - `docs/history/PROJECT-HISTORY.md`: phase-by-phase history and standing rules.
  - `docs/history/ARCADEAMP-DEVELOPMENT-LOG.md`: v0.1 to v0.4, each field report with cause, fix and measurements, including the Open panel crash investigation.
  - `docs/testing/TEST-REPORT.md`: test layers, 65 DSP checks, host tests, field reports, facts about Equalizer APO established from its source, and an explicit "not verified" list.
- **Corrections to earlier instructions** (found by reading Equalizer APO's source):
  - The green **+** adds a row **above** the row it is clicked on (the guide earlier said "under").
  - A `VSTPlugin:` row **can** carry settings as text (`Library <path> name value ...`); an earlier note said it could not.
  - Recommended plugin location is `C:\Program Files\EqualizerAPO\VSTPlugins\` (was an arbitrary `C:\VSTPlugins`).
  - Equalizer APO's **Open panel** needs the plugin to have its own window.
- README, USER-GUIDE, VST-SETUP, PROFILE-CATALOG, PACKAGE-MANIFEST updated to point at the above.
- **Original files untouched**: `arcade-boost.txt`, `arcade-boost.peace`, both MP3s and the original README (moved to `docs/history/ORIGINAL-README-v1.md`). `arcade-boost-apo.txt` is the same preset in the documented Equalizer APO syntax, now used by `config.txt`.
- Everything below this entry is unchanged historical record, including the earlier PurestDrive-based VST notes, which are superseded by ArcadeAmp (see `docs/research/VST.md`).

## ArcadeAmp v0.4.0 (plugin window, fixes the Open panel crash)

- **Cause of the crash found.** Clicking **Open panel** in Equalizer APO crashed it (Windows log: `Editor.exe`, `c0000005`). Equalizer APO's source shows the panel always asks the plugin for an editor window and reads the rectangle without checking; ArcadeAmp had none. Documented in `docs/research/VST.md` and `vst-plugin/README.md`.
- **New plugin window**: six preset buttons, seven sliders, a 60/50 Hz switch. Sliders show the active preset's values; touching one switches to Custom starting from those values. Screenshots in `vst-plugin/docs/`.
- Preset no longer flagged as an integer parameter (hosts misread its limits as 0..0); `host_test` fails on that now.
- New tests: `winhost_test` (shipped Windows DLL under Wine, panel-style queries) and `winui_test` (opens the window like Equalizer APO, clicks Heavy, drags a slider, clicks 50 Hz, closes).
- Corrected an earlier claim: the `VSTPlugin:` row **does** carry settings as text (`Library <path> name value ...`, values on a 0 to 1 scale), so `docs/VST-SETUP.md` now has an "Advanced: the text line" section.
- The DLL is now about 1.5 MB (was 0.2 MB) because it contains the window code; it still imports only Windows system DLLs.
- Sound unchanged from v0.3.
- Not verified: the window inside the real Equalizer APO on Windows, and listening.

## ArcadeAmp v0.3 (features restored and extended)

Feedback on v0.2: the setup is too complex for everyday users, and removing features was not the idea, since the project exists to reproduce cabinet and TV sound qualities. v0.3 puts the character back, on top of the fixed v0.2 base.

- **New controls**: Warmth (asymmetric clipping, even harmonics), Sag (level-dependent supply droop), separate Hum and Hiss, Hiss Tone (dark to bright at constant loudness), Whine (15,734 Hz NTSC/NTSC-J line rate, a documented frequency).
- **Preset selector** (Custom / Light / Medium / Heavy / TV early / TV late): one control sets everything except Mains 50Hz. `presets/experimental-vst/README.md` now assigns one preset per profile with the same hardware-class rule as before.
- Level behaviour kept: nothing can be louder than its input; the processing lowers a 0.3-peak tone by 0.05 dB (Light), 0.7 dB (Medium), 1.8 dB (Heavy). Sag depth was halved after the first test run showed Heavy at -2.5 dB.
- **Tests**: 14 groups (was 10) in `vst-plugin/tests/dsp_test.cpp`, plus `host_test.cpp`, which loads the real plugin wrapper through the VST2 API and checks the 9 parameters, Preset selection and levels.
- Docs rewritten: `docs/VST-SETUP.md` (Preset-first quick start, evidence column per control), `vst-plugin/README.md` (version history, DSP per control), `docs/research/VST.md`.
- Old ArcadeAmp rows must be deleted and re-added (control layout changed).
- (Superseded in v0.4.0: v0.3 shipped without a window, which Equalizer APO's Open panel cannot handle.)
- Not verified: v0.3 loaded and listened to inside real Equalizer APO.

## ArcadeAmp v0.2 (bug fixes)

Reported from real use of v0.1: volume changed a lot, drive hard to tell from clipping, no audible mains buzz. Causes were reproduced and fixed.

- **Volume jump fixed**: gain compensation boosted quiet audio (+2.2 / +5.5 / +8.1 dB at Drive 0 / 12 / 25 % on a 0.3-peak tone). Now `tanh(k*x)/k`: 0 dB small-signal gain, never louder than input, exact bypass at 0.
- **Buzz now audible**: right-channel hum was frozen, hiss was about -65 dBFS and the pure sine hum was cut by the MVS 85 Hz high-pass. Now a shared harmonic-rich buzz plus hiss.
- Tests now run the real two-channel path at MAME-realistic levels against the shipping header.
- The six per-profile `.txt` copies in `presets/experimental-vst/` were removed: they had drifted from the audited presets (different routing and Preamp), which likely contributed to the volume change.
- VST syntax corrected: the form is `VSTPlugin: Library "path"`, added via the Configuration Editor.

## Consolidated project update

This update restructures the research package into a cleaner repository layout while preserving the original Arcade Boost baseline.

### Repository cleanup

- Removed duplicate copies of research documents.
- Removed duplicate `presets/arcade/` tree; `presets/arcade/` is now canonical.
- Removed duplicate `presets/crt-tv/` tree; `presets/crt-tv/` is now canonical.
- Removed the duplicate `config.txt`; `config.txt` is the single selector.
- Removed the obsolete `neogeo-mvs-original-v6.txt` clone from the active preset tree.
- Consolidated historical research documents into `docs/research/`.
- Consolidated VST research and implementation guidance into `docs/research/VST.md` and `optional/vst/`.
- Replaced multiple version-numbered changelogs with this single project changelog.

### VST/DSP clarification

- Equalizer APO remains the core linear DSP engine.
- VST processing is explicitly optional and experimental.
- Airwindows PurestDrive is documented as a generic nonlinear stage, not an amplifier emulation.
- The previous per-profile VST snippets were removed because their Heavy/Medium/Light labels did not correspond to different encoded plugin parameters.
- Generic noise generation remains outside the core presets because no target-specific noise measurement supports a fixed setting.
- Convolution remains a separate measurement-driven path.

### Research integrity

- Hardware/routing evidence remains separated from acoustic assumptions.
- Nominal speaker specifications are not treated as frequency-response measurements.
- No fabricated arcade-cabinet IR is included.
- The repository structure now makes it easier to replace experimental profiles with measured data later.

## Experimental VST layer

Added a dedicated `presets/experimental-vst/` family and `docs/VST-SETUP.md`.

- Keeps nonlinear processing separate from the evidence-based core profiles.
- Adds conservative Light/Medium/Heavy PurestDrive starting points for MVS.
- Adds Light experimental profiles for CPS1, CPS2 and Sega Model 2.
- Documents the limitation that VST parameter state is not a portable plain-text standard.
- Provides an end-user Configuration Editor workflow instead of shipping third-party VST binaries.
- Keeps noise generation separate from evidence-backed cabinet modelling.
