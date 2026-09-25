# Arcade-Cabinet-Acoustic-Emulator-for-MAME

A simple Equalizer APO preset designed to approximate the tonal character of a classic arcade cabinet when playing MAME on modern PC audio hardware.

## TL;DR

MAME produces digital audio without reproducing the acoustic coloration introduced by a physical arcade cabinet, its speakers, amplifier, cabinet construction, and listening environment.

This project uses a small number of EQ filters to **approximate some of that coloration**: stronger low-frequency weight, reduced lower-midrange "boxiness", and a modest amount of high-frequency presence.

It is **not a measurement-derived model of one specific arcade cabinet**. The preset is an intentionally broad approximation of the kind of tonal character associated with many arcade machines from the 1990s and 2000s.

---

## Audio Comparison

Listen to the difference between the unadulterated MAME output and the arcade cabinet simulation:

**1. Flat / Raw MAME Audio:**
<audio controls src="./flat-mame.mp3"></audio>

**2. Arcade Cabinet Emulation (With Preset Applied):**
<audio controls src="./arcade-boosted.mp3"></audio>

---

## Preset Configuration

Preamp: -2.5 dB
Filter 1: LS f=95 Hz gain=11 dB Q=0.71
Filter 2: PK f=315 Hz gain=-4 dB Q=1.41
Filter 3: HS f=7000 Hz gain=2 dB Q=0.71

### Equalizer APO

Preamp: -2.5 dB
Filter 1: ON LS Fc 95 Hz Gain 11 dB Q 0.71
Filter 2: ON PK Fc 315 Hz Gain -4 dB Q 1.41
Filter 3: ON HS Fc 7000 Hz Gain 2 dB Q 0.71

> Note: The exact syntax may vary depending on how the filters are entered through Equalizer APO or Peace. The frequency, gain, and Q values above are the important parameters.

---

## What This Emulates

A real arcade machine is not an acoustically transparent playback system. The sound reaching the player is influenced by the speaker drivers, amplifier, cabinet geometry, cabinet materials, speaker mounting, and listening environment.

This preset attempts to reproduce some of the **broad tonal effects** that can contribute to the familiar arcade sound.

### 1. Low-Frequency Weight

**95 Hz Low Shelf, +11 dB**

The low shelf provides substantially more low-frequency energy than a typical flat PC playback chain.

The intention is to approximate the physical weight and punch that can be perceived from speakers mounted in an arcade cabinet, particularly with explosions, impacts, engine sounds, and other low-frequency effects.

Baffle geometry can affect low-frequency radiation through the well-known baffle-step phenomenon. However, the exact amount of compensation depends heavily on the physical dimensions and construction of the particular speaker enclosure.

Therefore, the +11 dB value should be considered an **empirical tonal choice**, rather than a universal correction for arcade cabinets.

### 2. Lower-Midrange "Boxiness"

**315 Hz Peaking Cut, -4 dB**

The 315 Hz cut reduces some of the lower-midrange energy that can make small or inexpensive speaker systems sound thick, congested, or "boxy."

This is intended to create more separation between effects, voices, and music while reducing the sense of a heavily colored enclosure.

Actual cabinet resonances vary considerably with cabinet dimensions, speaker placement, construction, and internal volume, so this filter is an approximation rather than a model of a specific cavity resonance.

### 3. High-Frequency Presence

**7,000 Hz High Shelf, +2 dB**

The high shelf adds a small amount of upper-frequency presence.

The goal is not to reproduce a particular arcade sound board or speaker driver, but to counterbalance the strong low-frequency emphasis and lower-midrange reduction while retaining some of the energetic character associated with arcade playback.

The actual high-frequency response of an arcade machine varies substantially between hardware generations, speaker systems, and cabinet designs.

---

## What This Preset Is — and Isn't

### It Is

- A simple tonal approximation of classic arcade playback.
- Designed for modern PC audio systems running MAME.
- Intended to work with Equalizer APO and, optionally, Peace.
- A deliberately small EQ curve rather than a complicated multi-band simulation.
- Something that can be adjusted to personal preference and different games.

### It Isn't

- A measurement of a specific arcade cabinet.
- A physically accurate simulation of cabinet acoustics.
- A replacement for an actual arcade speaker/amplifier/cabinet system.
- Guaranteed to reproduce the response of every arcade machine.
- A claim that every arcade cabinet had the same frequency response.

The goal is simply to get **closer to the character of arcade playback than a completely flat PC audio chain**.

---

## Managing Volume Differences Between Games

Arcade hardware and games were produced across many different generations and used different audio hardware, mixing levels, and gain structures. Consequently, games can have noticeably different perceived volume levels.

The **Preamp** control can be used to compensate for these differences.

### Quieter Games

For games that have a relatively low output level, you can raise the Preamp value.

For example:
Preamp: 0 dB
or slightly higher if necessary.

### Louder Games

For games with particularly strong peaks, retaining a negative Preamp value can provide additional headroom:
Preamp: -2.5 dB
or lower if required.

Because the low shelf adds substantial gain below 95 Hz, some reduction in Preamp is useful for maintaining headroom.

> Note: The Preamp value is primarily a gain/headroom control. It does not change the tonal shape of the preset.

---

## Installation Guide

### 1. Install Equalizer APO

Download Equalizer APO from the official SourceForge project:
https://sourceforge.net/projects/equalizerapo/

During installation:
1. Select the Windows playback device you want MAME to use.
2. Complete the installation.
3. Reboot Windows if requested.

### 2. Optional: Install Peace

Peace provides a graphical interface for Equalizer APO and makes creating and adjusting presets easier.
https://sourceforge.net/projects/peace-equalizer-apo-extension/

### 3. Apply the Preset

Enter the following parameters into Equalizer APO or Peace:

Preamp: -2.5 dB

Low Shelf:
Frequency: 95 Hz
Gain: +11 dB
Q: 0.71

Peaking:
Frequency: 315 Hz
Gain: -4 dB
Q: 1.41

High Shelf:
Frequency: 7000 Hz
Gain: +2 dB
Q: 0.71

Start with the default values before making personal adjustments.

---

## Recommended Use

The preset is intended primarily for:
- MAME arcade emulation
- Desktop speakers
- Arcade cabinet PC builds
- Full-range PC audio systems
- Headphones when a more arcade-like tonal balance is desired

Results will depend heavily on the playback hardware.

---

## Tuning the Preset

The three filters are intentionally broad, making them relatively easy to tune.

### More Bass
Increase the LS 95 Hz gain.

### Less "Boxiness"
Increase the depth of the PK 315 Hz cut (e.g., -5 dB instead of -4 dB).

### More Treble Presence
Increase the HS 7000 Hz gain (e.g., +3 dB instead of +2 dB).

---

## Future Improvements

Potential future work includes:
- Measuring frequency responses from multiple arcade cabinets.
- Comparing different cabinet sizes and speaker configurations.
- Recording the same game through different arcade audio systems.
- Using impulse-response measurements instead of EQ alone.

---

## Scientific References & Literature

- Olson, Harry F. — Direct Radiator Loudspeaker Enclosures. Journal of the Audio Engineering Society.
- Dickason, Vance — The Loudspeaker Design Cookbook. Audio Amateur Press.
- Beranek, Leo L. — Acoustics. McGraw-Hill.

---

## Disclaimer

This project is an **audio experimentation and emulation project**, not a scientifically validated model of arcade cabinet acoustics.

> **Take clean digital MAME audio and give it some of the tonal character associated with playing through a classic arcade cabinet.**
