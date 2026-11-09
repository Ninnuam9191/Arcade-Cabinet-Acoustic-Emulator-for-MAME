# SNK Neo Geo MVS — real cabinet data collected for v7.0 consolidation

## Evidence level

This file deliberately separates **documented hardware data** from acoustic estimates. No frequency-response curve is fabricated from these specifications.

## MVS-4-25 / US operation manual

The SNK MVS-4-25 replacement-parts list identifies:

- Speaker: **4 inch diameter, 8 ohm, 5 watt**.
- Monitor: Wells-Gardner 19K7101, 25 inch.
- Cabinet: MVS4(s).
- Power supply: 150 W switching regulator.

Source: SNK Neo-Geo MVS US Operation Manual, replacement parts list, 1991 scan. The searchable reproduction explicitly lists the speaker as “4" Dia., 8 Ohm 5 Watt”.

Source links:
- https://www.videogameconsolelibrary.com/images/Manuals/91_Neo_Geo_MVS_US-Manual.pdf
- https://www.scribd.com/document/559067883/Neo-Geo-MVS-US-Manual

## Important limitation

The 8 ohm / 5 W rating is **not** a frequency response. It establishes a real driver specification for this cabinet model, but does not justify a particular 85 Hz high-pass, 7 kHz low-pass, resonance frequency, or T60.

Therefore the existing MVS GraphicEQ and Modal values remain marked **EXPERIMENTAL** until a response measurement is obtained.

## MVS stereo/mono evidence

The MV-1F service manual documents separate LEFT SPEAKER and RIGHT SPEAKER connections and a STEREO/MONAURAL switch. This is direct evidence that MVS installation topology is variant-dependent and should not be represented as universally mono.

Source:
- https://manualzz.com/doc/6299009/snk-neo-geo-mv-1f-arcade-console-service-manual

A Japanese hobbyist account also documents MV-1B/MV-1C boards as requiring stereo modification and discusses the DAC's left/right outputs. This is useful corroboration but is treated as secondary evidence rather than a cabinet specification.

Source:
- https://blog.toyoshima-house.net/2018/02/mvs.html

## v7.0 consolidation project decision

Do not create a synthetic “MVS IR” from these numbers. The correct future path is to obtain an impulse/transfer measurement from an original MVS cabinet, preferably with the exact cabinet model and speaker part number documented.


---

# Sega New Astro City — real speaker/cabinet data collected for v7.0 consolidation

## Original speaker identification

The Sega New Astro City service documentation identifies the original speaker assembly as:

- Part number: **130-5124**
- Description: **SPEAKER MINI BOX 4 OHM 10W**
- Speaker wiring harnesses are separately listed for the left/right speaker assemblies.

Sources:
- https://shabazz.fr/wp-content/uploads/2021/10/Sega_New_Astro_City__Manual.pdf
- https://manualzz.com/doc/46945624/sega-new-astro-city-15k-ce-arcade-game-serviceman-manual

A French restoration/test report independently identifies the original New Astro City driver as **130-5124 / F77A95-1**, 4 ohm / 10 W, and reports a direct comparison performed in an actual New Astro City cabinet against replacement speakers. The report is useful as empirical evidence that the original driver has a distinctly non-hi-fi response, but its published text does not expose numerical frequency-response points.

Source:
- https://bidouillouzzz.blogspot.com/2020/07/test-comparatif-de-haut-parleurs-pour.html

## What this establishes

We can now identify a real, model-specific arcade speaker and its nominal electrical rating for New Astro City. We can also document that comparative listening/testing was performed in the actual cabinet.

## What it does NOT establish

It does not provide a calibrated SPL-vs-frequency dataset, an impedance sweep, an anechoic response, or an impulse response for the original 130-5124. Therefore no numerical EQ curve should be reverse-engineered from the prose impressions alone.

## v7.0 consolidation project decision

Use the 130-5124 identification as a **hardware anchor**, not as a fabricated EQ curve. If a future owner can measure an original NAC with REW/ARTA and a calibrated microphone, that measurement can become the first genuinely measured arcade-cabinet IR/FR in this project.


---

# Japanese arcade sound-environment measurement — contextual real data

A Japanese architectural/acoustics study measured the sound environment of two game-center leisure facilities and analyzed the sound emitted by game machines.

The study reports that the frequency characteristic of game-machine sound showed a broad, full-band / single-hump characteristic from the 63 Hz band through the 4 kHz band. Across the overall frequency range, the measured sound-pressure levels were approximately **40 dB in the high-frequency region to approximately 90 dB in the low-frequency region**; in the 1 kHz to 4 kHz region, approximately **60–85 dB** was reported.

Source:
- https://www.jstage.jst.go.jp/article/japaninteriorsociety/14/0/14_79/_pdf/-char/en

## Interpretation for this project

This is real measured arcade-environment data, but it is **not** a measurement of one cabinet's electro-acoustic transfer function. It therefore should not be converted directly into a cabinet EQ preset.

It is valuable for validating the broader assumption that real arcade-machine output was strongly non-flat and that low-frequency energy could be substantially higher than high-frequency energy in the measured arcade environment.

## v7.0 consolidation decision

Keep this as contextual validation only. Do not use the reported dB ranges as a per-cabinet frequency-response curve.
