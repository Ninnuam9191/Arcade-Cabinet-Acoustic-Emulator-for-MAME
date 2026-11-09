# Source Register

This register identifies the principal technical and research sources used by the project. Evidence is kept separate from experimental acoustic assumptions.

| Evidence target | Source | Evidence type | Use |
|---|---|---|---|
| MVS-4-25 speaker | SNK MVS US manual | Manufacturer/service documentation | Documented speaker anchor |
| MVS amplifier variants | NeoGeo Development Wiki | Technical hardware reference | Documented board variants |
| MV-1F stereo/mono | MV-1F service manual | Service documentation | Documented routing |
| New Astro City speaker | Sega service manual | Service documentation | Documented speaker anchor |
| New Astro City speaker comparison | Bidouillouzzz restoration/test material | Real-cabinet empirical test | Empirical; no calibrated FR curve |
| Japanese arcade sound environment | Japan Interior Society / J-STAGE study | Acoustic measurement | Measured contextual data |
| K053260 routing | MAME source | Emulation/technical source | Documented title/board routing |
| Midway DCS topology | MAME source | Emulation/technical source | Documented mono/stereo variants |
| CPS1 routing examples | MAME source | Emulation/technical source | Documented representative routing |
| Arcade cabinet IR methodology | Plogue documentation | Developer/product documentation | Documented capture methodology |
| IR licensing | Plogue EULA | License documentation | Redistribution constraint |
| ESS IR workflow | IR Maker | Open-source technical tool | Measurement/deconvolution workflow |
| Equalizer APO processing | Equalizer APO documentation | Official software documentation | Configuration and DSP capability |
| VST loading | Equalizer APO discussion archive | Maintainer/community technical documentation | VSTPlugin syntax/parameter behaviour |
| PurestDrive | Airwindows | Open-source plugin documentation/source | Tried first; no reliably sourced VST2 build, superseded by ArcadeAmp |
| PeakEater | Open-source clipper | Release page | Tried; ships CLAP/LV2/VST3 only, not loadable by stock EQ APO |
| DISTRHO Plugin Framework | DISTRHO (ISC) | https://github.com/DISTRHO/DPF | Framework ArcadeAmp is built on |
| Equalizer APO source code (VST host, Editor, installer) | Equalizer APO project, GitHub mirror `mirror/equalizerapo` | https://github.com/mirror/equalizerapo | Read to explain the Open panel crash and to confirm the `VSTPlugin:` row syntax, the + button behaviour, the VSTPlugins folder and the panel buttons (see `docs/history/ARCADEAMP-DEVELOPMENT-LOG.md`) |

## Primary references

### Equalizer APO

https://sourceforge.net/p/equalizerapo/wiki/Documentation/

https://sourceforge.net/p/equalizerapo/wiki/Configuration%20reference/

VST loading examples:

https://sourceforge.net/p/equalizerapo/discussion/general/thread/fc7e51287e/

https://sourceforge.net/p/equalizerapo/discussion/general/thread/611ebfe897/

### Neo Geo MVS

https://www.videogameconsolelibrary.com/images/Manuals/91_Neo_Geo_MVS_US-Manual.pdf

https://manualzz.com/doc/6299009/snk-neo-geo-mv-1f-arcade-console-service-manual

https://wiki.neogeodev.org/index.php/Power_amp

### Sega New Astro City

https://shabazz.fr/wp-content/uploads/2021/10/Sega_New_Astro_City__Manual.pdf

https://bidouillouzzz.blogspot.com/2020/07/test-comparatif-de-haut-parleurs-pour.html

https://www.sega.jp/astrocitymini/original/outline.html

### Japanese arcade acoustics

https://www.jstage.jst.go.jp/article/japaninteriorsociety/14/0/14_79/_pdf/-char/en

### MAME technical routing

https://github.com/mamedev/mame/blob/master/src/mame/capcom/fcrash.cpp

https://github.com/mamedev/mame/blob/master/src/mame/konami/simpsons.cpp

https://github.com/mamedev/mame/blob/master/src/mame/shared/dcs.cpp

https://github.com/mamedev/mame/blob/master/src/mame/cave/cv1k.cpp

### Arcade IR research

https://ploguechipsounds.blogspot.com/2013/06/making-arcade-cabinet-impulse-responses.html

https://www.plogue.com/products/chipcrusher.html

https://www.plogue.com/chipcrusher/eula-text-chipcrusher.html

https://github.com/JuanPabloZed/IR_Maker

### Airwindows (tried, superseded by ArcadeAmp)

https://www.airwindows.com/purestdrive-vst/

https://github.com/airwindows/airwindows

## Evidence policy

A technical source can establish hardware topology, speaker part numbers, amplifier type, or routing without establishing an acoustic frequency response.

The project therefore does not turn nominal speaker specifications, subjective descriptions, or unrelated measurements into fabricated EQ curves.

When a real measurement becomes available, it should be stored with its capture conditions and provenance and then used to replace the relevant experimental model.
