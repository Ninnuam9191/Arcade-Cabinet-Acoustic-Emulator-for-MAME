# Test report

What has been tested, how, with what result, and what has **not** been
verified. Raw outputs are kept in `vst-plugin/tests/` and are reproducible.

Scope: the optional ArcadeAmp plugin and how it is hosted by Equalizer APO.
The core EQ profiles are research-based (`docs/research/`) and are judged by
listening; they have no automated tests because there is nothing measurable to
test them against (no measured cabinet data exists in this project; see
`docs/research/MEASUREMENTS-AND-IR.md`).

## 1. Test layers

| Layer | Program | Runs on | What it proves | Recorded output |
|---|---|---|---|---|
| DSP maths | `vst-plugin/tests/dsp_test.cpp` | Any machine with a C++ compiler | The signal processing behaves as documented, using the same header the plugin compiles | `dsp_test-output.txt` |
| Plugin wrapper through the VST2 API | `host_test.cpp` | Linux (native build of the same wrapper) | The nine parameters, Preset selection by value, level behaviour, and that no parameter reports integer limits a host would misread | `host_test-output.txt` |
| Shipped Windows DLL, host queries | `winhost_test.cpp` | Windows program run under Wine | The actual `ArcadeAmp.dll` answers the queries a host's parameter panel makes, sweeps every parameter, processes audio and closes cleanly | `winhost_test-output.txt` |
| Shipped Windows DLL, editor window | `winui_test.cpp` | Windows program under Wine, virtual display | The window opens through Equalizer APO's call sequence; real mouse messages reach the plugin | `winui_test-output.txt`, screenshots in `vst-plugin/docs/` |
| Real use | The project owner, Equalizer APO 1.4.2 on Windows | Real machine | It loads, the window opens, the sound changes | Field reports, section 4 |

## 2. DSP test groups (65 checks, 0 failures)

Recorded in `vst-plugin/tests/dsp_test-output.txt`; re-run result matches the
recorded file exactly.

| # | What is checked | Key result |
|---|---|---|
| 1 | Exact bypass when everything is 0; silence in gives silence out | Output identical to input |
| 2 | **Level neutrality** (the old volume jump) for Drive, Warmth, Sag combinations at 0.3 and 0.01 peak | Never louder; Drive 12 % about -0.27 dB (v0.1 was +5.5 dB) |
| 3 | Hostile +-4.0 input with Drive 100 and Sag 100; everything at 100 | Strictly never louder per sample (Warmth 0); no NaN or Inf |
| 4 | Small-signal gain at -60 dBFS | 0 dB |
| 5 | Warmth creates even harmonics, removes DC; Warmth 0 creates none | 2nd harmonic -16.8 dB (Warmth 50) and -11.8 dB (Warmth 100) re the fundamental at Drive 40 |
| 6 | Sag is level dependent, never louder, recovers | Sag 50: 0.05 peak -0.38 dB, 0.6 peak -3.75 dB; recovers to its own steady state after a burst |
| 7 | Hum and Hiss levels vs the documented mapping; left equals right; DC | Within 0.7 dB; identical channels |
| 8 | Hiss Tone: same loudness, different colour | Level within 1 dB; high energy rises, low falls |
| 9 | Whine frequency and level at 44.1, 48, 96 kHz; silent at 32 kHz | 15,734 Hz bin dominates; level -30 dBFS at 100 % |
| 10 | Hum spectrum: 2nd harmonic dominant for 60 and 50 Hz at several sample rates | Harmonics sit on multiples of the mains frequency |
| 11 | Hum after the Neo Geo MVS profile's HP 85 Hz and LP 7 kHz | -46.5 dBFS before, -47.4 after (v0.1 lost 7 to 10 dB here) |
| 12 | Presets (values; Mains is never overridden; level effect) | Light -0.05, Medium -0.70, Heavy -1.81 dB on a 0.3-peak tone |
| 13 | Slamming every control mid-stream; live 60 to 50 Hz switch on one instance | Finite, no giant jumps, buzz moves from 120 to 100 Hz |
| 14 | In-place equals out-of-place processing | Identical |

How to re-run: see `vst-plugin/BUILD.md`. One line:
`g++ -O2 -Wall -I../source -o dsp_test dsp_test.cpp -lm && ./dsp_test`.

## 3. Host-integration tests

### 3.1 Wrapper through the VST2 API (`host_test`)

Lists the nine parameters as a host sees them, sets values through
`setParameter` (normalised 0 to 1), processes audio and prints the levels.
Excerpt of recorded behaviour: Custom Hum 25 only, silence in: -46.5 dBFS on
both channels; preset TV early on silence: -42.9 dBFS (hum + hiss + whine
combined). It also **fails** if any parameter reports integer limits that
a host would read as `min >= max`.

### 3.2 Shipped DLL under Wine (`winhost_test`)

Output shows `flags 0x11` (has editor), plugin name, nine parameters with
names, labels and display strings (for example `Preset` displays `Custom`,
`Mains 50Hz` displays `60 Hz`), automation flags, and `closed ok`.

### 3.3 Editor window under Wine (`winui_test`)

Reproduces Equalizer APO's call sequence from its source code and then uses
real mouse messages. Recorded: has editor; rectangle available before open
(this is exactly the condition that crashed v0.3); `editOpen=1`, rectangle 560
by 540; clicking Heavy sets Preset 0.60 and sends one automation call;
dragging Hum switches to Custom (Preset 0.00, Drive 0.25 copied from Heavy,
Hum 0.65); clicking 50 Hz sets Mains 1.00; `editClose ok`.
(The two stray lines at the end of that file are Wine/DPF log noise.)

## 4. Field reports

All reports are from one listener, the project owner, on Windows with
Equalizer APO 1.4.2. They are **observations, not measurements**.

| When | Version | Report | Outcome |
|---|---|---|---|
| Early in plugin work (screenshot dated 2026-10-07) | v0.1 | Plugin row visible in the editor; volume changed so the Preamplification row had to be lowered; drive audible; mains buzz and colour options inaudible; setup complex | Root causes found and fixed in v0.2 (development log) |
| 2026-10-09 | v0.1 feedback repeated | Same, plus: removing features was not the intent | v0.3 restores and extends the features |
| 2026-10-11 | v0.3 | **Open panel crashes the editor.** Event 1000: `Editor.exe` 1.4.2.0, `c0000005`, offset `0x91315` | Cause: no plugin window. v0.4 adds one |
| 2026-11-09 | v0.4 | Works; the window is fine; the sound changes; not perfect without a further stage | Accepted as the baseline; merged into the repository |

## 5. Facts about Equalizer APO established from its source

Verified by reading the open-source code (GitHub mirror `mirror/equalizerapo`)
during the crash investigation. They are the basis of the setup guide.

| Fact | Where |
|---|---|
| **Open panel** calls the plugin's editor functions without checking it has an editor | `Editor/guis/VSTPluginFilterGUI.cpp`, `helpers/VSTPluginInstance.cpp` |
| The row syntax is `VSTPlugin: Library <path> <name> <value> ...`; values are on the 0 to 1 scale | `filters/VSTPluginFilterFactory.cpp`, `Editor/guis/VSTPluginFilterGUI.cpp` |
| The **+** on a row inserts the new row above it | `Editor/FilterTable.cpp` (`addLine(line, before)`) |
| The entry is under the **Plugins** menu as **VST plugin** | `Editor/guis/VSTPluginFilterGUIFactory.cpp` |
| The file button opens a "Select VST plugin" dialog for `*.dll` | `Editor/guis/VSTPluginFilterGUI.cpp` |
| The installer creates `VSTPlugins`; the Editor warns if the audio service cannot read a plugin elsewhere | `Setup/Setup.nsi`, `helpers/VSTPluginLibrary.cpp`, `VSTPluginFilterGUI.cpp` |
| The panel dialog has OK, Apply, Cancel and an "Apply automatically" box | `Editor/guis/VSTPluginFilterGUIDialog.ui` |
| A plugin that crashes during processing is logged by the audio engine | `filters/VSTPluginFilter.cpp` |

## 6. Not verified

* ArcadeAmp **measured against any real cabinet or TV.** It is a generic
  effect. All levels and time constants other than the documented frequencies
  are usability choices.
* The setup guide's steps A1, A3, A4, B3 (menu path) and B4 (OK button) were
  written from Equalizer APO's behaviour and source, not from fresh
  screenshots, and have not been followed step by step by a beginner. Please
  report anything that does not match what you see.
* Listening tests with more than one listener, or blind comparison.
* Other Windows versions, other sample rates in real use, and other audio
  devices than the project owner's.
* The `.vst3` and `.clap` builds were built and format-checked only; they were
  not loaded in any host.
* Convolution (deferred).

## 7. Reproducing everything

1. `vst-plugin/BUILD.md`: build the plugin (needs DPF with submodules).
2. Run `dsp_test` (any platform).
3. Optional: `host_test` (Linux) and, with Wine and a virtual display,
   `winhost_test` and `winui_test` (commands in `vst-plugin/BUILD.md`).
