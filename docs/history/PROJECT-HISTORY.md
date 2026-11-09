# Project history

How the project got from one broad preset to per-machine profiles, TV
profiles and an optional plugin, in the order it happened, and why each
decision was taken. Dates are given only where a record carries one.
Nothing here replaces the per-topic research in `docs/research/`; this is
the narrative that ties it together.

## Phase 0: the baseline

`arcade-boost.txt` is the original preset: one broad approximation of "an
arcade cabinet" for Equalizer APO, with comparison media (`flat-mame.mp3`,
`arcade-boosted.mp3`) in the upstream repository. It is kept unchanged as
the historical baseline (the original README is preserved in
`docs/history/ORIGINAL-README-v1.md`). It is not represented as a measured response.

**Limitation that started everything:** real cabinets differed (Neo Geo MVS,
Capcom CPS systems, Sega Model 2, Cave and others sounded different), so one
curve cannot be right for all of them.

## Phase 1: research before coding

Decision: **get real data first.** Accurate numbers, signal-processing
references, and documentation on each machine's amplifier, speaker, size and
construction. The most played MAME systems were prioritised, and further
systems (for example the hardware behind Sonic Wing and Battle Garegga) were
requested and looked up in archive.org and Japanese sources where those could
be found. Where no source was found the profile says so.

Convolution question: how can an impulse response be made without owning the
physical machines? Answer recorded in `docs/research/MEASUREMENTS-AND-IR.md`:
it cannot be invented; it needs a genuine capture (or a documented
third-party measurement). Decision: **convolution is a future phase.** No
fabricated cabinet IR is shipped.

Outputs: `docs/research/HARDWARE.md`, `MVS.md`, `REAL-HARDWARE-DATA.md`,
`SOURCES.md`, `EVIDENCE-MATRIX.md`, and the evidence labels
(DOCUMENTED / INFERRED / EMPIRICAL / EXPERIMENTAL / MEASURED).

## Phase 2: packaging for ordinary users

Everything is delivered as one zip; the README was rewritten so users with
less computer experience can use the presets. More machine families were
added beyond the first list (Sega System 16/18, Konami K053260 family,
Midway DCS, Atari JSA, Toaplan/Raizing, Video System YM2610, and others, see
`docs/PROFILE-CATALOG.md`).

## Phase 3: use more of Equalizer APO

Feedback: the profiles used only 3 to 4 EQ bands, while Equalizer APO offers
much more. The profiles were rebuilt with `GraphicEQ`, `Modal` filters (with
T60 decay) for cabinet-panel resonance, `Copy` for channel routing
(mono/stereo variants), `Delay`, and `Include`. Research: `docs/research/EQAPO.md`.

## Phase 4: Peace import and the selector

* **Problem:** the presets did not import with Peace's Import button. **Cause:**
  Peace reads plain numbered `Filter N: ON PK ...` lines only, not
  `GraphicEQ`, `Modal`, `Copy` or `Delay`. **Fix:** `presets-peace/`, with
  restated versions, and a table of what is lost and how to add it back by
  hand.
* Newer Cave hardware was added (`cave-cv1000.txt` next to `cave-1st-gen.txt`).
* **Problem:** replacing the whole `config.txt` to change profile was
  awkward. **Fix:** a single `config.txt` selector with one `Include:` line
  per profile.

## Phase 5: loudness calibration

Field report: some machines were so quiet the effect could not be heard, a
few too loud, and most arcade presets barely reached 30 to 40 percent on the
VU meter. Giving them -5 to -11.5 dB of preamp would have made it worse.

Investigation: an earlier version of the project assumed a worst-case
full-scale (0 dBFS) input when computing preamp. **MAME deliberately runs its
output below full scale** (MAME issue tracker #6840), so that assumption made
presets far too quiet. Fix: preamp is computed from each preset's actual
peak gain with a realistic assumed 6 dB of headroom. Both numbers (the peak
gain and the assumed headroom) are documented in the profile comments and
`docs/research/EQAPO.md`.

## Phase 6: CRT television profiles

Request: replicate home TV sound by gaming era (1970s, 1980s, 1990s, 2000s),
based on real, preferably Japanese, models where documented. Result:
`presets/crt-tv/`. Notable correction made during this phase: an early draft
assumed the US timeline for stereo TV sound; Japan's EIAJ multiplex stereo
broadcasting started in October 1978, earlier than US BTSC (1984), so the
Japanese timeline is used (Sony KV-1375 series is one documented anchor
showing the adapter-based stereo transition). Details: `docs/research/CRT-TV.md`.

## Phase 7: the limits of a linear equalizer

Question: can a VST fix Equalizer APO's linearity and add other sounds? Yes in
principle (saturation, noise), which a filter cannot do. Decision: **keep
everything open source**, keep the VST layer optional, and never bake it into
the evidence-based core presets.

* Off-the-shelf attempts failed with stock Equalizer APO: Airwindows
  PurestDrive (no reliably sourced VST2 build) and PeakEater (ships CLAP, LV2
  and VST3 only; stock Equalizer APO hosts VST2). See `docs/research/VST.md`.
* The project owner had also worked with another assistant on a merged v2.0 and
  an experimental VST v2.1; those were reconciled into one less cluttered
  tree. Earlier duplicate per-profile VST snippets were removed because their
  Heavy/Medium/Light labels did not encode different parameters.

## Phase 8: ArcadeAmp, a plugin written for the project

Because nothing suitable existed, a small open-source plugin was written.
It went through four versions in response to real field reports; each report,
its cause and its fix, with measurements, is in
`docs/history/ARCADEAMP-DEVELOPMENT-LOG.md`.

| Version | Summary |
|---|---|
| v0.1 | First plugin. Field test: volume jumped, drive confused with clipping, mains buzz and colour options inaudible. |
| v0.2 | Root causes found and fixed. Too few controls left. |
| v0.3 | Features restored and extended (Warmth, Sag, Hum, Hiss, Hiss Tone, Whine, Presets). Field test: Open panel crashed Equalizer APO. |
| v0.4 | Root cause of the crash found in Equalizer APO's source; plugin window added. Field test: works, window is fine, the sound changes. |

## Phase 9: this merge (v2.2)

Merged ArcadeAmp, the beginner setup guide with pictures, and the research and
test history into the repository **without replacing existing documents**:
everything earlier is still in place; new material is under `docs/history/`,
`docs/testing/`, `docs/images/`, `vst-plugin/`, and linked from the README and
CHANGELOG.

## Standing rules that shaped every decision

1. Real data first; label every claim DOCUMENTED / INFERRED / EMPIRICAL /
   EXPERIMENTAL / MEASURED.
2. A speaker's power or impedance rating is not a frequency response.
3. Open source only.
4. The core presets never depend on the plugin.
5. No fabricated measurements or impulse responses.
6. Tests must exercise real use (realistic levels, both channels, the real
   plugin binary) because passing a unit test once hid three bugs (see the
   development log).
7. Documentation must be usable by people with little computer experience.

## Open items

* **Convolution** with a genuine measured IR (needs a capture; see
  `impulse-responses/README.md`).
* **Measured amplifier and noise data** for specific cabinets and TVs; until
  then ArcadeAmp stays generic.
* **More screenshots** for the setup guide (`docs/images/README.md`).
* **A profile switcher script** (requested earlier, deferred).
* **Listening comparison protocol** with more than one listener.
