# Profile Catalog

## Arcade

| Profile | Target | Routing | Acoustic status |
|---|---|---|---|
| `neogeo-mvs.txt` | Neo Geo MVS | Preserve stereo by default | Experimental EQ |
| `neogeo-mvs-mono.txt` | Mono MVS cabinet | L/R fold-down | Experimental EQ |
| `cps1.txt` | Capcom CPS1 | Representative mono | Experimental EQ |
| `cps2.txt` | Capcom CPS2/QSound | Stereo source | Experimental EQ |
| `cps3.txt` | Capcom CPS3 | Stereo-oriented source | Experimental EQ |
| `segasystem16.txt` | Sega System 16/18 | Hardware-dependent | Experimental EQ |
| `segamodel2-deluxe.txt` | Sega Model 2 deluxe | Installation-specific | Experimental EQ |
| `konami-k053260.txt` | Konami K053260 family | Board/game dependent | Experimental EQ |
| `midway-dcs.txt` | Midway DCS examples | Mono target | Experimental EQ |
| `atari-jsa.txt` | Atari JSA family | Revision-dependent | Experimental EQ |
| `cave-1st-gen.txt` | Early Cave boards | JAMMA | Experimental EQ |
| `cave-cv1000.txt` | Cave CV1000 | JAMMA | Experimental EQ |
| `toaplan2-raizing.txt` | Toaplan/Raizing family | JAMMA | Experimental EQ |
| `videosystem-ym2610.txt` | Video System YM2610 | JAMMA | Experimental EQ |
| `igs-pgm2-dodonpachi-daioujou-tamashii.txt` | PGM2 title | Stereo | Experimental EQ |

## CRT TV

The current profiles are organized by era because the available evidence is
stronger for broad architectural changes than for a single universal model
curve. Model references inside the profiles are evidence anchors, not claims
that the EQ is a measured response of that exact set.

- 1970s — early mono consumer CRT
- 1980s — transitional / Famicom-era consumer CRT
- 1990s — integrated stereo consumer CRT
- 2000s — larger late-generation CRT / higher-power stereo TV

## Optional ArcadeAmp preset per profile

Suggested starting preset for the optional plugin (see `docs/SETUP-GUIDE.md`
Part B and the reasoning in `presets/experimental-vst/README.md`). These are
starting points by hardware class, not measurements.

| Profile | ArcadeAmp preset |
|---|---|
| `neogeo-mvs-mono.txt` | Heavy |
| `neogeo-mvs.txt`, `cps1.txt`, `cps2.txt`, `segasystem16.txt`, `konami-k053260.txt`, `midway-dcs.txt`, `atari-jsa.txt`, `cave-1st-gen.txt`, `cave-cv1000.txt`, `toaplan2-raizing.txt`, `videosystem-ym2610.txt`, `igs-pgm2-dodonpachi-daioujou-tamashii.txt` | Medium |
| `cps3.txt`, `segamodel2-deluxe.txt` | Light |
| `crt-tv-1970s.txt`, `crt-tv-1980s.txt` | TV early |
| `crt-tv-1990s.txt` | TV late |
| `crt-tv-2000s.txt` | Light |
