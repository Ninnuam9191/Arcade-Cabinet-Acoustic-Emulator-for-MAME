# Peace-importable presets

**Preamp values in this revision are computed from each preset's actual
peak gain, not hand-picked — see `../EQAPO-FEATURES.md` section 5.**

These are restated versions of the presets in `../presets/arcade/`, written
as plain numbered `Filter N: ON PK ...` lines so Peace's **Import** button
can actually read them. See `../EQAPO-FEATURES.md` for exactly why the
full versions didn't import and what's different here.

**How to import:** in Peace, click Import, browse to the `.txt` file for
your system, and select it. It should populate Peace's filter list
directly — no manual retyping needed.

## What you lose vs. the full version, and how to add it back manually

A few presets rely on features Peace's Import can't carry over. For those,
after importing, open Peace's **Effects** panel (separate from the main
filter list — see Peace's own "How to use Peace" help) and add the
matching effect by hand:

| Preset | What's missing from the import | Add manually via Peace's Effects panel |
|---|---|---|
| `neogeo-mvs.txt` | Mono fold-down, small-driver HP/LP band limiting | Use the up/down-mix effect set to mono |
| `midway-dcs.txt` | Mono fold-down, small-driver HP/LP band limiting | Use the up/down-mix effect set to mono |
| `atari-jsa.txt` | Mono fold-down | Use the up/down-mix effect set to mono |
| `cps2.txt` | Stereo widener (6 ms delay on the right channel + a light gain-matrix blend) | Use the Delay effect on the right channel only (a few ms), plus the crossfeed effect set light |
| `igs-pgm2-dodonpachi-daioujou-tamashii.txt` | APO-only routing/Modal behavior | Import as-is; stereo is preserved and the Modal is represented as a PK filter |

Every other preset in this folder only used GraphicEQ + Modal, both of
which convert cleanly to plain PK filters, so there's nothing extra to add
for those — importing is the whole job.

## What's approximated rather than fully preserved

Every `Modal` filter (cabinet panel resonance) became a plain peaking (PK)
filter at the same frequency/gain/Q. The frequency-domain shape is
preserved; the explicit decay-time (T60) behavior — the actual physical
modeling upgrade described in `EQAPO-FEATURES.md` — is not, because
Peace's filter model has no field for it. If that distinction matters to
you, use `presets/arcade/` via direct config.txt editing instead; that's the
only path that keeps it.


## v6.2 audit note

The Peace files intentionally approximate the corresponding full APO presets using numbered parametric filters. Newer APO-only features (GraphicEQ, Modal/T60, routing and convolution/VST components) are not represented one-for-one here. Check the matching `presets/arcade/` file when exact behavior matters.


## v6.3 evidence note

`igs-pgm2-dodonpachi-daioujou-tamashii.txt` is based on documented PGM2/YMZ774 stereo routing. Its acoustic EQ remains experimental because no measured PGM2 cabinet response is included. See `../SOURCE-REGISTER-v6.3.md`.
