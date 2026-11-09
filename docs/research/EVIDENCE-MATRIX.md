# Preset Evidence Matrix — v6.3

Legend:
- **D** = documented hardware/routing fact
- **V** = installation/PCB variant; do not universalize
- **I** = inference from documented architecture
- **E** = experimental acoustic model; not a measurement
- **M** = measured by this project (none yet)

| Preset | Hardware | Routing / speaker topology | Acoustic EQ | IR |
|---|---|---|---|---|
| `neogeo-mvs` | D | V: MVS boards span mono and stereo amp implementations | E | none (MVS-specific IR unavailable) |
| `neogeo-mvs-mono` | D | V/D for a known mono installation | E | none |
| `neogeo-mvs-original-v6` | D | V | E; legacy duplicate | none |
| `cps1` | D | V; board/game/cabinet-specific | E | none |
| `cps2` | D | D at source level for QSound stereo processing; cabinet still varies | E | none |
| `cps3` | D | D/E depending on installation; preserve stereo source | E | none |
| `cave-cv1000` | D | D in MAME: YMZ770 right output -> mono speaker | E | none |
| `cave-1st-gen` | D | V by board/title | E | none |
| `konami-k053260` | D | V; MAME has both mono and stereo examples | E | none |
| `midway-dcs` | D | V; ROM DCS mono, DCS2 stereo, Denver multi-channel | E | none |
| `segasystem16` | D | V by board/title | E | none |
| `segamodel2-deluxe` | D | V; SCSP is stereo-capable, cabinet implementation varies | E | none |
| `atari-jsa` | D | V by JSA revision | E | none |
| `toaplan2-raizing` | D | V by PCB/title | E | none |
| `videosystem-ym2610` | D | V by board/title; not equivalent to MVS cabinet topology | E | none |
| `igs-pgm2-dodonpachi-daioujou-tamashii` | D | D in MAME: YMZ774 stereo | E | none |

## Interpretation

The project currently has strong evidence for **what the source hardware did** and weak evidence for **what the cabinet acoustically did**. That is acceptable as long as the two are never presented as the same measurement.

The next measurement target is therefore not another speculative EQ curve. It is a real speaker/cabinet transfer function or impulse response, ideally with the exact cabinet and speaker identified.
