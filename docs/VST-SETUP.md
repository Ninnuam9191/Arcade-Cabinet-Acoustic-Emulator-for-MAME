# Optional: ArcadeAmp (amplifier and TV character)

Optional. The normal presets work fine without it.

**What it adds:** Equalizer APO's filters can only reshape tone. Real
amplifiers and TVs also round off loud peaks, sag when the power supply is
loaded, hum, hiss, and (on CRT TVs) whine at the scan frequency. ArcadeAmp
adds those. It is a generic effect, **not** a measured model of any
particular cabinet or TV.

## Quick start

A longer version with pictures is in `SETUP-GUIDE.md` (Part B). Short form:

1. **Install Equalizer APO 1.4.2 (stock)** and make sure your normal preset
   already works.
2. **Copy `vst-plugin/dist/ArcadeAmp.dll`** to
   `C:\Program Files\EqualizerAPO\VSTPlugins\` (the installer creates this
   folder; Equalizer APO's audio service can always read it).
3. Open the **Configuration Editor** and open a copy of your normal preset
   (for example a copy of `presets/arcade/neogeo-mvs.txt`).
4. Find the **Preamplification** row. Click the green **+** on the row
   **directly below it** (the **+** inserts the new row **above** the row you
   click it on), open **Plugins**, choose **VST plugin**, and pick
   `ArcadeAmp.dll` with the small file button. The row must sit **directly
   below Preamplification and above the GraphicEQ/filter rows**, because the
   amplifier comes before the speaker. (Picture: `images/02-editor-vst-row.png`.)
5. Click **Open panel** on that row. The ArcadeAmp window opens (see
   `images/03-plugin-window.png`). Click the **Preset** button listed for your
   profile in `presets/experimental-vst/README.md`, then click **OK** under it
   (Equalizer APO's own button; **Cancel** throws the change away). Done.

If you tried an earlier ArcadeAmp, **delete its row and add the plugin
again**; a saved row keeps the old control layout.

## The window

Six preset buttons, seven sliders, and a 60/50 Hz switch. While a preset is
active the sliders show that preset's values (dimmed). Drag any slider and the
window switches to **Custom**, starting from those values, so you can tweak a
preset. Equalizer APO adds its own buttons under the window: **OK** stores
your choice in the row, **Apply** stores it and keeps the window open,
**Cancel** discards it. Tick **Apply automatically** if you want every click
stored at once.

## The controls (in the window)

| Control | Plain meaning | Evidence |
|---|---|---|
| **Preset** | Custom = use the sliders below. Light / Medium / Heavy / TV early / TV late = a ready-made recipe that **overrides every slider except Mains 50Hz**. | Recipes are EXPERIMENTAL starting points |
| **Drive** | Rounds off loud peaks like an amplifier running out of headroom. 0 = off. Never makes quiet sound louder. | Textbook tanh soft clipper; amount EXPERIMENTAL |
| **Warmth** | Makes the rounding lopsided (adds even harmonics, like a single-ended amplifier stage). Needs Drive above 0. | INFERRED from amplifier theory; amount EXPERIMENTAL |
| **Sag** | Power-supply droop: loud passages briefly lose a little gain, then recover over about a tenth of a second. Never louder than the input. | INFERRED physics; time constants EXPERIMENTAL |
| **Hum** | Mains buzz. Starts clearly audible around 20 to 30. | Harmonic pattern INFERRED (rectifier ripple at 2x mains); level EXPERIMENTAL |
| **Hiss** | Broadband amplifier hiss. | Level EXPERIMENTAL |
| **Hiss Tone** | 0 = dark, rumbly hiss; 100 = bright, airy hiss (same loudness). | EXPERIMENTAL |
| **Whine** | A very high-pitched tone at 15,734 Hz, the line rate of NTSC / Japanese NTSC-J TVs and standard 15 kHz arcade monitors. Many adults cannot hear it and small speakers do not play it. | Frequency DOCUMENTED (4.5 MHz / 286); level EXPERIMENTAL |
| **Mains 50Hz** | Off = 60 Hz hum, On = 50 Hz hum. Eastern Japan (Tokyo) and Europe are 50 Hz; western Japan (Osaka) and North America are 60 Hz. Only matters for Hum. | DOCUMENTED grid frequencies |

Nothing here is measured from a real cabinet or TV. Levels are chosen so the
effect is audible but not harsh.

## Presets and what they set

| Preset | Drive | Warmth | Sag | Hum | Hiss | Hiss Tone | Whine | Typical use |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| Light | 5 | 0 | 0 | 0 | 0 | 50 | 0 | Better-than-standard chains |
| Medium | 12 | 15 | 10 | 15 | 10 | 40 | 0 | Most cabinets |
| Heavy | 25 | 25 | 20 | 25 | 20 | 35 | 0 | Single small speaker cabinets |
| TV early | 25 | 25 | 20 | 25 | 25 | 30 | 25 | 1970s to 1980s CRT TVs |
| TV late | 12 | 15 | 10 | 12 | 10 | 45 | 12 | 1990s CRT TVs |

Which preset suits which profile is in `presets/experimental-vst/README.md`.
It follows one rule: cheap single-speaker cabinets and early TVs lean heavy,
better-than-standard chains lean light. That is reasoning from documented
hardware class, **not a measurement**.

## What to expect (so you know it's working)

* **Volume should not jump up.** ArcadeAmp never makes the signal louder
  than it came in. The processing slightly *lowers* loud peaks: on a typical
  MAME level (peaks around 0.3), Medium is about 0.7 dB quieter and Heavy
  about 1.8 dB quieter. If you want the old loudness back, raise the Preamp
  row by that amount. If you ever hear it get *louder*, that is a bug: tell
  us.
* **Drive, Warmth and Sag are subtle on purpose.** For a quick sanity check
  set Preset to Custom, Drive to 100 once: loud sounds should obviously get
  squashed and buzzy. Then come back down.
* **Hum, Hiss and Whine are easiest to hear in silent moments** (title
  screens, the gap between songs). Raise them until you can just hear them
  there, then back off a notch. On small laptop speakers you will hear the
  upper part of the hum, not the deep 60/120 Hz.
* **Whine** is at 15.7 kHz. If you hear nothing at Whine 50 it is probably
  your speakers or your ears, not a fault. Try headphones.
* **Don't use the Analysis panel to judge it.** That graph shows the
  frequency response of the filters. Saturation, sag and noise are not a
  frequency response, so the curve is not a reliable way to see what the
  plugin does. Listen instead.

## Troubleshooting

* **Equalizer APO crashes when you click Open panel:** you have an older ArcadeAmp (v0.3 or earlier), which had no window of its own; Equalizer APO's panel needs one. Use the `ArcadeAmp.dll` from the newest zip (v0.4) and delete the old row first.
* **Plugin doesn't appear / won't load:** use `ArcadeAmp.dll` (VST2) with
  stock Equalizer APO, not the `.vst3` folder. The `.vst3` is only for the
  community VST3 build.
* **Changed a value in the text line and nothing happened:** Preset is not 0 (Custom). Any preset other than Custom overrides the other values, see the next section.
* **No effect at all:** check the VST row's power button is on, and that it
  is below Preamp and above the filters.
* **Too loud or distorted anywhere:** set Preset to Custom with Drive 0 and
  everything else 0 (the plugin is then an exact bypass), and tell us what
  you heard.
* **Sounds worse than without it:** remove the row. The core preset never
  depends on the plugin.

## Advanced: the text line

If you prefer, or the window will not open, the whole setting lives on one
line of the preset file (Equalizer APO's own format, confirmed from its
source code). The values are the plugin's 0 to 1 scale. For example, Medium
on 60 Hz mains:

```text
VSTPlugin: Library C:\Program Files\EqualizerAPO\VSTPlugins\ArcadeAmp.dll Preset 0.4 Drive 0.1 Warmth 0 Sag 0 Hum 0 Hiss 0 "Hiss Tone" 0.5 Whine 0 "Mains 50Hz" 0
```

| Value | Meaning |
|---|---|
| `Preset` | 0 Custom, 0.2 Light, 0.4 Medium, 0.6 Heavy, 0.8 TV early, 1 TV late |
| `Drive`, `Warmth`, `Sag`, `Hum`, `Hiss`, `"Hiss Tone"`, `Whine` | percent divided by 100 (0.25 means 25). Ignored unless `Preset` is 0 |
| `"Mains 50Hz"` | 0 = 60 Hz, 1 = 50 Hz |

Put the line directly below the `Preamp:` line and above the filter lines. Names with
a space need quotes, as shown. Use the window to create the line the first
time if unsure; it writes exactly this.

## Order of processing

```text
MAME > Preamp > ArcadeAmp (sag, saturation, noise) > routing and speaker/cabinet model > output
```

The noise goes through the speaker model too, like a real amplifier's hum
does. That is why a small-speaker profile makes the deep part of the hum
quieter and leaves the buzzy harmonics.

If you later add a measured full-chain impulse response, do not assume the
plugin should stay on top of it; see `docs/research/VST.md`.
