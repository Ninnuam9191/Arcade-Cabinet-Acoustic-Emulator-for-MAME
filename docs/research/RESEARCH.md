# Research Master

This release consolidates the acoustic/hardware research accumulated through v6.4 and the subsequent international source audit. It is intentionally a **research consolidation**, not another small preset tweak.

## Evidence policy

The project distinguishes five evidence classes:

1. **MEASURED** — numerical electro-acoustic or electrical measurement from the actual target cabinet/device.
2. **DOCUMENTED** — service manual, parts list, schematic, PCB documentation, or manufacturer documentation identifying hardware/topology.
3. **EMPIRICAL** — test/restoration work performed on the real cabinet where methodology is described, but numerical transfer data is unavailable.
4. **INFERRED** — acoustic consequence logically derived from documented construction.
5. **EXPERIMENTAL** — project-created EQ/Modal parameters that have not been measured.

A speaker impedance/power rating is never treated as a frequency-response measurement. A subjective listening description is never converted into numerical EQ without an independent measurement.

## Major real-data findings

### SNK Neo Geo MVS

The SNK MVS-4-25 parts documentation identifies a real **4-inch, 8-ohm, 5-W speaker**. The MV-1F service documentation separately documents left/right speaker connections and stereo/monaural switching, while the NeoGeo Development Wiki documents multiple amplifier implementations: MV1B TA8201AK mono (~7 W/4 ohm), MV1C TDA7240 mono (20 W/4 ohm), and MV1F/MV1FZS HA13001 stereo (2×5 W/4 ohm). These facts establish genuine hardware variation but do not provide an MVS cabinet transfer function.

Sources:
- https://www.videogameconsolelibrary.com/images/Manuals/91_Neo_Geo_MVS_US-Manual.pdf
- https://manualzz.com/doc/6299009/snk-neo-geo-mv-1f-arcade-console-service-manual
- https://wiki.neogeodev.org/index.php/Power_amp

**Project consequence:** keep MVS acoustic EQ experimental; expose mono/stereo as installation-dependent variants; do not fabricate an MVS IR.

### Sega New Astro City

The New Astro City service manual identifies the original speaker assembly as **Sega 130-5124, SPEAKER MINI BOX, 4 OHM 10W** and separately lists speaker wiring/ground hardware. A French restoration test identifies the original driver as **130-5124 / F77A95-1** and compares it against replacement drivers while mounted in an actual New Astro City cabinet. The published article does not provide a numerical response trace, so its empirical observations are not converted into EQ points.

Sources:
- https://shabazz.fr/wp-content/uploads/2021/10/Sega_New_Astro_City__Manual.pdf
- https://bidouillouzzz.blogspot.com/2020/07/test-comparatif-de-haut-parleurs-pour.html
- https://bidouillouzzz.blogspot.com/2020/05/test-comparatif-du-da-pc83-4-et-du-cs.html

Sega's Japanese material also identifies the original Astro City as a 1993 resin cabinet and states that approximately 50,000 were deployed, which is useful cabinet-context evidence but not an acoustic measurement.

Source:
- https://www.sega.jp/astrocitymini/original/outline.html

**Project consequence:** New Astro City is now one of the highest-value targets for obtaining a future real measurement because the original driver is unambiguously identified.

### Japanese arcade sound-environment measurement

A Japanese interiors/acoustics study measured sound emitted by arcade machines in two game-center facilities. It reports broad measured frequency-band levels from the low-frequency bands through 4 kHz. This is genuine measured arcade acoustic data, but it is an environmental measurement across facilities rather than a calibrated single-cabinet transfer function.

Source:
- https://www.jstage.jst.go.jp/article/japaninteriorsociety/14/0/14_79/_pdf/-char/en

**Project consequence:** use only as contextual validation; never turn these SPL ranges into an individual cabinet GraphicEQ.

### Real arcade impulse responses exist

Plogue documents a dedicated process of capturing arcade-cabinet impulse responses and its current chipcrusher product lists actual full-size **Galaxian Arcade Cabinet** and **Nintendo VS cabinet** impulses among its game-device cabinet responses. This is valuable proof that genuine arcade-cabinet IR capture is practical and has been performed. However, Plogue's EULA explicitly prohibits redistribution/reuse of its enclosed IRs in another sample library or competitive product.

Sources:
- https://ploguechipsounds.blogspot.com/2013/06/making-arcade-cabinet-impulse-responses.html
- https://www.plogue.com/products/chipcrusher.html
- https://www.plogue.com/chipcrusher/eula-text-chipcrusher.html

**Project consequence:** do not copy or package Plogue IRs. They can be cited as an external commercial reference and as a model for the measurement methodology.

## Hardware-routing corrections consolidated in v7.0

- **CPS1:** do not describe the family as inherently stereo. Current MAME configurations for representative CPS1 hardware route YM2151/OKI audio to a mono speaker device. Individual boards/bootlegs can differ, so the preset documentation now calls the acoustic profile a mono/JAMMA baseline rather than a universal stereo system.
- **K053260:** do not describe the family as universally stereo. MAME documents genuine stereo output on boards such as The Simpsons, while other Konami boards have different routing. The preset is therefore a source-family acoustic approximation, not a claim that every K053260 cabinet is stereo.
- **Midway DCS:** preserve the distinction between original DCS ROM-based mono, DCS2 stereo, and later multi-channel Denver variants. The existing MKII/MK3/UMK3/NBA Hangtime preset is explicitly limited to the mono family.
- **CV1000:** current MAME identifies the YMZ770 path as mono in the CV1000 driver and distinguishes later CV1000-D hardware. Game-family assignment remains important; PGM/PGM2 titles are not silently folded into CV1000.

Sources:
- https://github.com/mamedev/mame/blob/master/src/mame/capcom/fcrash.cpp
- https://github.com/mamedev/mame/blob/master/src/mame/konami/simpsons.cpp
- https://github.com/mamedev/mame/blob/master/src/mame/shared/dcs.cpp
- https://github.com/mamedev/mame/blob/master/src/mame/cave/cv1k.cpp

## What is still genuinely missing

No public source located in this research pass supplied a complete, openly redistributable, calibrated **original MVS cabinet** or **New Astro City 130-5124 cabinet** impulse response/frequency-response dataset.

The next useful evidence is therefore not another nominal specification. It is a measurement from a surviving cabinet.

## Measurement target specification

For each target cabinet, capture:

- exact cabinet model/revision;
- exact speaker part number;
- amplifier board/model and supply voltage;
- speaker impedance sweep if safely separable;
- calibrated microphone model and calibration file;
- microphone position and distance;
- cabinet volume/potentiometer setting;
- logarithmic sweep, preferably 20 Hz–20 kHz;
- left and right responses separately when stereo;
- raw recording of the sweep;
- deconvolved impulse response;
- frequency response exported as CSV;
- at least one moderate-level capture and one high-level capture if distortion/compression is of interest.

The resulting dataset can be turned into a real minimum-phase/linear-phase IR and an optional GraphicEQ representation.

## Source-selection rule

Reddit is deliberately **not used as a primary evidence source in this project**. Community posts may occasionally point toward a document, but a claim enters the evidence matrix only when it can be traced to a service manual, schematic, measurement, manufacturer document, MAME source/PCB documentation, or a described real-cabinet test.
