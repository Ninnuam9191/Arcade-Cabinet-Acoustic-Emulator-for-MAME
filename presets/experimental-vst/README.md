# ArcadeAmp settings for every profile

Optional second layer. There are no separate preset files here on purpose:
you use the normal profile from `presets/arcade/` or `presets/crt-tv/`,
add the ArcadeAmp row to it once (illustrated steps in `../../docs/SETUP-GUIDE.md`, reference in `../../docs/VST-SETUP.md`), and
set **one control, Preset**, from this table.

(An earlier version of this folder had separate `.txt` copies of a few
profiles. They had drifted from the real presets, with different routing and
different Preamp, so they are gone. Using the real profile guarantees you are
layering the plugin on the audited model, not on a lookalike.)

## How the presets were assigned

One rule, applied the same way everywhere:

* **Heavy** only where the amplifier/speaker chain is documented as a
  *single small speaker* (the Neo Geo MVS-4-25 is a 4-inch, 8-ohm, 5 W
  speaker per `../../docs/research/MVS.md`).
* **Light** only where the chain is documented as *better than a standard
  cabinet*: Model 2 deluxe cabinets (real subwoofer), CPS3 (stereo output to
  an external amplifier), and 2000s CRT TVs (the reference set has a
  2 x 20 W stereo amplifier).
* **Medium** everything else, i.e. whenever nothing documents otherwise. A
  clean *source* (Midway DCS is 16-bit digital audio) does not make the
  *cabinet amplifier* cleaner, so DCS is Medium.
* **TV early** for 1970s and 1980s TVs, **TV late** for 1990s TVs. These add
  the 15,734 Hz line-rate whine (the NTSC / NTSC-J scan rate).

This is reasoning from documented hardware class, **not a measurement of any
amplifier**. Treat every preset as a starting point for your ears. What each
preset sets is in the table in `../../docs/VST-SETUP.md`.

**Mains 50Hz** is not part of any preset because it depends on where the
machine lived: eastern Japan (Tokyo) and Europe 50 Hz, western Japan (Osaka)
and North America 60 Hz. It only matters for Hum.

## Per profile

### Arcade (`presets/arcade/`)

| Profile | Preset |
|---|---|
| `neogeo-mvs-mono.txt` | Heavy |
| `neogeo-mvs.txt` | Medium |
| `cps1.txt` | Medium |
| `cps2.txt` | Medium |
| `cps3.txt` | Light |
| `segamodel2-deluxe.txt` | Light |
| `cave-1st-gen.txt` | Medium |
| `cave-cv1000.txt` | Medium |
| `igs-pgm2-dodonpachi-daioujou-tamashii.txt` | Medium |
| `toaplan2-raizing.txt` | Medium |
| `videosystem-ym2610.txt` | Medium |
| `segasystem16.txt` | Medium |
| `konami-k053260.txt` | Medium |
| `midway-dcs.txt` | Medium |
| `atari-jsa.txt` | Medium |

### CRT TV (`presets/crt-tv/`)

| Profile | Preset |
|---|---|
| `crt-tv-1970s.txt` | TV early |
| `crt-tv-1980s.txt` | TV early |
| `crt-tv-1990s.txt` | TV late |
| `crt-tv-2000s.txt` | Light |

## If you can't hear it

* **Drive, Warmth and Sag** only act on loud material and are deliberately
  subtle. To confirm the plugin is alive, set Preset to Custom and Drive to
  100 once: loud sounds should audibly flatten and buzz. Then come back
  down.
* **Hum and Hiss** are easiest to hear in silent moments. Use Custom and
  raise Hum until you can just hear it there.
* **Whine** is at 15.7 kHz and is inaudible on many speakers and to many
  adults. Try headphones.

## Evidence status

Everything here is `EXPERIMENTAL`. The effect levels are usability choices,
not measured noise floors or distortion curves (none exist in this project;
see `../../docs/research/MVS.md`). Only the whine frequency (NTSC line rate)
and the 50/60 Hz mains frequencies are documented numbers.
