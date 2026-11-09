# VST and Nonlinear Processing Research

## Purpose

Equalizer APO is very strong for deterministic **linear** processing: gain, filters, frequency-domain shaping, channel routing, delay, modal-style resonances, and convolution. Its native processing cannot by itself reproduce nonlinear behaviour such as harmonic generation, soft clipping, or saturation, nor add a noise floor.

That distinction matters for this project because a real arcade/TV chain contains nonlinear stages in its amplifier and speaker system, and a real amplifier has a noise floor.

The VST layer therefore exists to extend the model without contaminating the core evidence-based presets.

## What Equalizer APO can do directly

The official configuration reference documents:

- `Preamp:`
- `Filter:` (including `Modal` with a T60 decay)
- `GraphicEQ:`
- `Copy:`
- `Delay:`
- `Convolution:`
- `Include:`
- `Device:` / `Channel:` / `Stage:`
- `If` / `Eval` expressions

Reference: https://sourceforge.net/p/equalizerapo/wiki/Configuration%20reference/

That page does **not** document a `VSTPlugin:` command at all. VST hosting is documented only in the project's discussion archive (below), which is one reason the project treats VST use as experimental.

## VST loading

Equalizer APO can load VST plugins with a `VSTPlugin:` command. The official project's discussion archive shows the command and shows plugin parameters being serialized into the configuration text.

Example of the form shown there:

```text
VSTPlugin: Library "C:\VSTPlugins\Plugin.dll"
```

Add and configure plugins through the Configuration Editor rather than by typing the line: the editor writes the correct parameter representation for the installed plugin. (An earlier revision of this project's draft snippets used a bare `VSTPlugin: C:\...dll` form without `Library`; that form is not what the archive shows and should not be used.)

Reference examples:
- https://sourceforge.net/p/equalizerapo/discussion/general/thread/fc7e51287e/
- https://sourceforge.net/p/equalizerapo/discussion/general/thread/611ebfe897/

## VST2 versus VST3

Stock Equalizer APO hosts **VST2** plugins. A separate community build provides VST3 and experimental out-of-process VST support. That build is not required by this project and is not part of the core dependency chain.

Reference: https://sourceforge.net/projects/eqapo64-with-vst3-support/

**Repository policy:** core profiles must remain usable without a community VST3 build.

## Two off-the-shelf plugins were tried, and neither worked

1. **Airwindows PurestDrive** (open source, smooth saturation) was recommended first. In practice a reliably sourced current **VST2** build could not be obtained.
2. **PeakEater** (open source clipper) was then tried directly. Its Windows release ships **CLAP, LV2 and VST3 only — no VST2** — so stock Equalizer APO cannot load it.

Both point at the same cause: Steinberg withdrew the VST2 SDK in 2018 and asked developers not to make new VST2 plugins, so most actively maintained free plugins no longer ship VST2.

## Resolution: ArcadeAmp, a small plugin written for this project

`../../vst-plugin/` contains **ArcadeAmp**, open source (GPL-2.0-or-later; built on the ISC-licensed DISTRHO Plugin Framework), compiled to VST2, VST3 and CLAP, with its tests and build instructions. See `../../vst-plugin/README.md` for the DSP, `../VST-SETUP.md` for setup. It has nine controls, but a **Preset** selector (Light / Medium / Heavy / TV early / TV late) means most users touch only one.

### What it adds, and the evidence behind each piece

| Piece | Why a linear EQ cannot do it | Evidence |
|---|---|---|
| **Drive**: `tanh(k*x)/k` soft clipper, 0 dB small-signal gain, never louder than input | Saturation is nonlinear | Textbook form (J. O. Smith); amount EXPERIMENTAL |
| **Warmth**: different k for positive and negative half-waves, DC removed by a 5 Hz high-pass | Even harmonics need asymmetric nonlinearity | INFERRED from amplifier theory; amount EXPERIMENTAL |
| **Sag**: gain `1/(1+s*env)`, peak follower attack 8 ms / release 120 ms | Level-dependent gain is not time-invariant | INFERRED (unregulated supply droop); constants EXPERIMENTAL |
| **Hum**: mains harmonics, 2nd dominant, 50 or 60 Hz | A noise floor is an added signal | Pattern INFERRED (full-wave rectifier ripple at 2x mains); weights and level EXPERIMENTAL; grid frequencies DOCUMENTED (Japan: east 50 Hz, west 60 Hz) |
| **Hiss** and **Hiss Tone**: filtered white noise, corner 1 to 16 kHz | Same | EXPERIMENTAL |
| **Whine**: 15,734.26 Hz tone | Same | Frequency DOCUMENTED (NTSC/NTSC-J line rate 4.5 MHz / 286); whether and how loudly it reached the audio of a given set is EXPERIMENTAL |

### What it does not do

- It does not model a particular Sega/SNK/Capcom/CRT amplifier.
- It does not prove the saturation curve of an original cabinet, or that the cabinet's amplifier was ever driven into that region.
- The buzz harmonic weights, the sag constants and every level mapping are approximations / usability choices, **not measurements**.
- PAL line rate (15,625 Hz) is not modelled.
- It does not replace a measured cabinet IR.

The project therefore labels it **EXPERIMENTAL generic coloration**, not "authentic amplifier emulation."

### Lesson recorded from the first releases (v0.1, v0.2)

v0.1 passed its own tests and still failed in real use, because those tests exercised the maths at full-scale levels on one channel. Real use showed: a volume jump, no audible buzz, and a silent right-channel hum. Re-creating the v0.1 code and measuring it at MAME-realistic levels reproduced all of it (details and numbers in `../../vst-plugin/README.md`). The tests since v0.2 run the real two-channel code path at realistic levels, and the DSP lives in a header shared by the plugin and the tests so that what is tested is what ships. v0.2 then fixed the bugs but cut the controls to three, which removed the character the project exists to add; v0.3 restores and extends it, and `vst-plugin/tests/host_test.cpp` additionally drives the real plugin wrapper through the VST2 API.

## Why the old per-profile VST snippets were removed (still true)

An earlier package contained many `*.vst-addon.txt` files that differed only in comments such as Heavy/Medium/Light while loading the same plugin with no encoded parameters. That was not reproducible modelling. The current approach is one plugin, documented controls with a Preset selector, and one settings table per profile (`../../presets/experimental-vst/README.md`).

The `presets/experimental-vst/` folder no longer contains per-profile `.txt` copies of the core presets: they duplicated the audited presets' filter lines and had already drifted from them. (An earlier note here said a text file cannot carry plugin settings. That was wrong: Equalizer APO's source shows the `VSTPlugin:` row stores parameter values as `name value` pairs on the same line. See `../VST-SETUP.md`, "Advanced: the text line".) Settings are applied to the real core preset, in the plugin window or on that one line.

## How Equalizer APO hosts a plugin (from its open-source code)

Read from the `mirror/equalizerapo` copy of the source (`Editor/guis/VSTPluginFilterGUI.cpp`, `helpers/VSTPluginInstance.cpp`, `filters/VSTPluginFilterFactory.cpp`):

- The config row is `VSTPlugin: Library <path> <name> <value> <name> <value> ...`. Names containing spaces are quoted; values are the VST2 normalised floats 0 to 1. Plugins that expose chunks are stored as a `ChunkData` blob instead.
- **Open panel always asks the plugin for an editor window** (`effEditGetRect`, `effEditOpen`, `effEditGetRect`) and reads the rectangle it gets back, without checking whether the plugin has an editor. A plugin with no window leaves that pointer unset and `Editor.exe` crashes with an access violation. This is what happened with ArcadeAmp v0.3 (crash log: faulting module `Editor.exe`, exception `c0000005`). **A plugin used with Equalizer APO must have its own window.** ArcadeAmp v0.4 has one.

## Noise is not a measured noise floor

A generic noise generator is not equivalent to a measured cabinet noise floor. Hum, Hiss and Whine default to 0, the Light preset leaves them at 0, and the project's MVS noise documentation (`MVS.md`) remains a research constraint: turning noise on is an artistic choice, not hardware reproduction.

## Convolution is a separate problem

Nonlinearity and convolution solve different modelling problems:

- **VST saturation** → nonlinear amplitude-dependent behaviour.
- **IR convolution** → linear time/frequency response of the measured signal chain.

They should not be treated as interchangeable. If a future measured IR already contains the cabinet/speaker response, do not stack another synthetic full-chain EQ on top of it without validating the combined response.

## Recommended signal-chain architecture

```text
MAME audio
  ↓
Preamp (headroom)
  ↓
optional ArcadeAmp (amplifier saturation + noise floor)
  ↓
documented channel/routing model
  ↓
documented or measured speaker/cabinet response
  ↓
optional measured cabinet IR (future)
  ↓
output
```

The amplifier comes before the speaker, so its noise and saturation products are shaped by the speaker model too, as in a real unit. The exact order of nonlinear processing and a measured IR must follow the measurement setup: a close-mic speaker IR and a room/listener IR are not equivalent.

## VST safety rule

A VST is optional experimental DSP, not a prerequisite for the project.

If the VST is unavailable, the core profile must still load and function.

If a VST misbehaves, set Preset to Custom and every control to 0 (exact bypass) or remove the row, and return to the core profile. Do not debug an unstable plugin by changing the acoustic EQ at the same time.
