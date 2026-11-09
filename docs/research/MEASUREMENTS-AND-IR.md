# Measurement → IR pipeline — v7.0

This is the measurement protocol to use when an original cabinet becomes available.

## 1. Document the hardware

Photograph and record:

- cabinet model/revision;
- PCB/model and revision;
- amplifier PCB/model;
- speaker label/part number;
- speaker impedance;
- speaker mounting location and orientation;
- grille/mesh/cover;
- cabinet dimensions;
- volume control position;
- power supply model.

## 2. Electrical capture

Where safe and electrically appropriate, capture a line-level signal before the power amplifier. Do **not** connect a normal audio interface directly to a power-amplifier speaker output.

If an electrical transfer measurement is possible, record the interface, attenuation/probe arrangement, sample rate, and reference level.

## 3. Acoustic sweep

Use an exponential/logarithmic sine sweep. A practical default is 20 Hz–20 kHz at 48 kHz or 96 kHz. Keep the level safely below obvious mechanical or electrical stress.

Record:

- raw sweep file;
- microphone calibration information;
- microphone position;
- cabinet volume setting;
- ambient-noise condition.

For stereo cabinets, measure each side separately and retain the channel identity.

## 4. Deconvolution

Generate the impulse response using an ESS/reverse-filter method. The open-source IR Maker project documents this workflow and requires the same sample rate for the sweep, recorded response and resulting IR for clean results.

Source:
https://github.com/JuanPabloZed/IR_Maker

## 5. Derive two project assets

### A. Reference IR

Keep the original, minimally processed IR for convolution. Document whether it contains:

- speaker only;
- speaker + cabinet;
- amplifier + speaker + cabinet;
- room/listener position.

Do not stack an IR containing the complete acoustic chain on top of an EQ intended to model the same chain.

### B. Frequency-domain approximation

Generate a GraphicEQ approximation only as a convenience for users who do not want convolution. Mark it **MEASURED-DERIVED**, not simply “measured,” because the EQ is an approximation of the measured transfer function.

## 6. Nonlinearity/noise

If multiple-level captures exist, compare them for harmonic distortion, compression, and noise. Keep those components separate from the linear IR. A generic hiss generator is not an acceptable substitute for a measured cabinet noise floor.

## 7. Release requirements

A measurement should not enter `impulse-responses/` as a canonical cabinet IR until the repository contains:

- source/owner permission or compatible license;
- hardware identification;
- measurement metadata;
- sample rate/bit depth/channel count;
- microphone position;
- processing/deconvolution method;
- checksum of the distributed WAV.


---

# Real arcade impulse-response sources — v7.0

## Plogue / chipcrusher

Plogue publicly documents the capture of arcade-cabinet impulse responses and lists full-size Galaxian Arcade Cabinet and Nintendo VS cabinet responses in chipcrusher's Cabinet section.

- Capture documentation: https://ploguechipsounds.blogspot.com/2013/06/making-arcade-cabinet-impulse-responses.html
- Product documentation: https://www.plogue.com/products/chipcrusher.html
- EULA: https://www.plogue.com/chipcrusher/eula-text-chipcrusher.html

### Redistribution status

The IRs are **not suitable for inclusion in this open-source project**. Plogue's EULA states that the enclosed impulse responses and noise recordings cannot be reused in another commercial sample library or competitive product and prohibits redistribution/upload/download/trade of the contents.

Therefore this repository contains no Plogue IR files.

### Why this source still matters

It establishes that the correct methodology—feeding a sweep through an actual arcade cabinet and capturing the resulting response—is practical and has already been used professionally. It also identifies two concrete arcade targets (Galaxian and Nintendo VS) for future independently captured, freely licensed measurements.

## Openly redistributable generic IRs

Generic loudspeaker/cabinet IR repositories exist, including Creative Commons/public-domain examples on Freesound and open-source IR collections. These are **not arcade-cabinet measurements** and therefore are not substituted for an arcade IR in this project.

Example generic IR source:
- https://freesound.org/people/jesterdyne/sounds/129398/

Such files may be useful for testing the convolution pipeline, but using one as an MVS/NAC IR would be misleading.


---

# Acoustic-data gaps and measurement targets — v7.0 consolidation

The project now has several real hardware anchors, but the central missing dataset remains the same: **calibrated electro-acoustic transfer measurements of original arcade cabinets**.

## Highest-value measurements

1. Original speaker, mounted in original cabinet, microphone at normal player/listener position.
2. Log sweep from 20 Hz to 20 kHz at moderate level, with cabinet volume control documented.
3. Impulse response and minimum-phase/linear-phase export where possible.
4. Separate left/right captures for stereo cabinets.
5. Electrical line-level capture before the power amplifier, when safe and practical.
6. Speaker impedance sweep if the original driver can be isolated.
7. Cabinet dimensions, speaker mounting position, grille/cover, and amplifier model photographed/documented.
8. Repeat measurement at two or more volume levels to expose amplifier/speaker compression or clipping.

## Why this matters

A nominal speaker rating such as 4 ohm / 10 W gives no information about the frequency-response shape. Likewise, a service manual's stereo wiring diagram does not provide the cabinet's acoustic transfer function.

The project therefore must not turn specifications into invented EQ numbers.

## Current evidence hierarchy

- **Measured:** actual frequency/sound-pressure/impulse data from an original cabinet.
- **Documented:** service manuals, parts lists, schematics, PCB routing, named speaker models.
- **Empirical secondary:** restoration measurements/listening tests performed on real cabinets, when methodology is described.
- **Inferred:** acoustic consequences derived from documented construction.
- **Experimental:** project EQ/Modal values not backed by a measurement.

v7.0 consolidation promotes no experimental EQ values to “measured.”
