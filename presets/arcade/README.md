# Arcade profiles

These are the canonical full Equalizer APO profiles.

Use the profile matching the actual hardware family when the game's platform is known. The comments inside each profile identify important routing/hardware assumptions.

| Profile | Target |
|---|---|
| `neogeo-mvs.txt` | Neo Geo MVS general model |
| `neogeo-mvs-mono.txt` | Explicit mono/single-speaker MVS variant |
| `cps1.txt` | Representative CPS1 mono-oriented routing |
| `cps2.txt` | CPS2/QSound-oriented stereo model |
| `cps3.txt` | CPS3 stereo/external-amp-oriented model |
| `segasystem16.txt` | Sega System 16/16B/18 family |
| `segamodel2-deluxe.txt` | Sega Model 2 deluxe/subwoofer-oriented cabinet |
| `konami-k053260.txt` | Konami K053260-era hardware |
| `midway-dcs.txt` | Midway DCS family |
| `atari-jsa.txt` | Atari JSA family |
| `cave-1st-gen.txt` | Early CAVE 68000-based hardware |
| `cave-cv1000.txt` | CAVE CV1000 family |
| `toaplan2-raizing.txt` | Toaplan2/Raizing-derived hardware |
| `videosystem-ym2610.txt` | Video System YM2610-based hardware |
| `igs-pgm2-dodonpachi-daioujou-tamashii.txt` | IGS PGM2/YMZ774-oriented profile |

These curves are not all measured cabinet responses. See `../../docs/research/EVIDENCE-MATRIX.md` and `../../docs/research/HARDWARE.md` for evidence levels.

For the original broad preset, use `../../arcade-boost.txt` (original) or `../../arcade-boost-apo.txt` (same values, Equalizer APO syntax).

Do not enable multiple arcade profiles at once.
