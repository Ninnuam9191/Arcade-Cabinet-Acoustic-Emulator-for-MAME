# Hardware & Cabinet Reference Notes

This is the canonical hardware reference for the current project. Historical release labels have been removed from the filename; the evidence itself is retained.

This document records the reasoning and sources behind each per-system preset,
separately from the presets themselves, so the tonal choices can be checked,
disputed, or corrected without having to reverse-engineer them from EQ values.

**Note:** as of this revision, the presets translate this hardware/cabinet
research into Equalizer APO configs using GraphicEQ curves and Modal
(decay-time) filters rather than a handful of peaking/shelf bands — see
`EQAPO-FEATURES.md` for what those tools are and why. The cabinet
*material* facts below (metal vs. wood/particleboard, deluxe vs. standard
JAMMA) are what determine each preset's Modal filter decay time (T60).

Two confidence tiers are used:

- **Confirmed** — chip and typical cabinet/wiring standard are documented by
  multiple independent sources.
- **Needs verification** — plausible based on partial sources, but not solid
  enough to hard-code an EQ curve around yet. Listed with what to check next.

---

## Confirmed systems

### Neo Geo MVS
- Sound: Yamaha YM2610 (FM synthesis + ADPCM), per system16.com hardware
  listings for SNK MVS boards.
- Cabinet: the majority of MVS installations were dedicated candy/table
  cabinets (1-slot, 2-slot, 4-slot, 6-slot) with a **single mono full-range
  speaker** in a small metal/particleboard shell, not a stereo pair.
  Confirmed by a scanned SNK cabinet manual (Super Neo 29 candy cabinet,
  archived via the Wayback Machine at web.archive.org) discussed on
  arcade-projects.com — the manual instructs setting the MVS board's own
  **mono/stereo mode jumper to mono**, because the cabinet is only wired
  for a single speaker. That same forum thread notes at least one other
  hardware wiki had this wrong (listed a cabinet as stereo when its own
  manual says mono), which is a good reminder to check the primary manual
  scan over a wiki summary where the two disagree.
- Caveat: since the MVS board itself has a mono/stereo switch, this is a
  cabinet-specific fact, not a universal one — a 1-slot/2-slot/4-slot/6-slot
  candy cabinet with only one speaker needs mono mode, but not every MVS
  installation is that cabinet (home-built JAMMA conversions with 2 speakers
  exist too). The preset below assumes the common candy-cabinet case.
- EQ implication: mono fold-down is not optional coloring, it's what the
  cabinet actually did. Small driver in a small sealed shell implies limited
  bass extension and a resonant "boxy" upper-bass/lower-mid region rather
  than clean sub-bass.

### CPS1 (Capcom CP System)
- Sound: Yamaha YM2151 (FM) + OKI MSM6295 (ADPCM), standard JAMMA upright
  cabinets, no special stereo processing chip.
- EQ implication: closest to a "generic JAMMA cabinet" baseline — moderate
  bass, moderate boxiness cut, no need for stereo-width tricks.

### CPS2 / CPS1.5 ("Dash")
- Sound: OKI MSM6295 (ADPCM) + Capcom/QSound DL-1425 chip. QSound is a
  16-channel PCM stereo-positioning ("expanded stereo image") algorithm with
  its own echo buffer — confirmed via chip documentation used by chiptune
  tracker projects that emulate it directly (Furnace tracker's QSound chip
  docs).
- Cabinet: standard JAMMA upright, same shell as CPS1 in most installs.
- EQ implication: this is the one system where the source hardware was
  *specifically designed to widen the stereo image*. A preset that narrows
  or mono-sums the signal actively undoes the one distinguishing feature of
  this generation. Preset should preserve or mildly enhance width instead.

### Sega Model 2
- Sound: Sega Custom Sound Processor (SCSP), digital PCM.
- Cabinet: unlike the above, Model 2 was frequently deployed in **deluxe /
  sit-down cabinets with real subwoofers** (Daytona USA, Sega Rally, Virtua
  Cop, Virtua Fighter deluxe/twin cabinets), i.e. genuine 2.1-style bass
  management rather than a small full-range driver being asked to fake bass.
- EQ implication: deeper, cleaner low-shelf extension is justified here in a
  way it isn't for MVS/CPS1/CPS2. Less aggressive boxiness cut, since these
  cabinets used better-quality speaker systems than a generic candy cab.
- Caveat: Model 2 boards were also sold as generic JAMMA conversion kits
  without a dedicated deluxe cabinet. The preset below assumes the deluxe/
  subwoofer installation; note this explicitly in the README so users with
  a JAMMA-kit Model 2 setup know to use the CPS1-style generic profile
  instead.

### Cave "1st Generation" hardware (DonPachi, DoDonPachi, ESP Ra.De,
Dangun Feveron, Guwange, etc.)
- Sound: Yamaha YMZ280B 8-channel PCM/ADPCM chip, or on some titles
  OKIM6295 (×1–2) + YM2203/YM2151, per arcadeotaku.com's CAVE 1st Generation
  Hardware page. Standard JAMMA wiring, no deluxe cabinet program.
- EQ implication: clean digital PCM samples rather than FM synthesis —
  don't need heavy tonal reshaping to "add character." The priority is
  clarity and controlled top end (bullet/clink sounds, vocal samples) rather
  than the boxiness-cutting/brightness-adding treatment generic JAMMA/FM
  boards benefit from.

### CPS3 (Capcom CP System III)
- Sound: a custom 16-channel stereo audio chip (not QSound — QSound was
  CPS2-specific and was not carried forward). Confirmed via Wikipedia's
  CPS3 hardware infobox (sourced to system16.com) and the Arcade Otaku wiki.
- Cabinet/wiring: **JAMMA+ with dedicated stereo RCA outputs to an external
  amplifier**, per the Arcade Otaku wiki's CPS3 page — a step up from the
  bare JAMMA mono speaker-out spec most earlier boards relied on (see the
  cross-cutting JAMMA note below). This means CPS3 installs were designed
  around an operator-supplied external amp/speaker pair rather than a small
  onboard cabinet driver.
- EQ implication: less need for the "small driver in a boxy cabinet"
  correction the earlier boards get — assume a reasonably competent
  external amp and speakers, keep stereo intact, lighter tonal reshaping
  overall.

### CAVE CV1000 ("Cave SH-3" hardware)
(Mushihimesama, Espgaluda II, Ibara, Pink Sweets, Mushihimesama Futari,
Muchi Muchi Pork!, Deathsmiles, DoDonPachi DaiFukkatsu, DoDonPachi
SaiDaiOuJou — 2004-2012)
- Sound: Yamaha YMZ770C-F, a compressed-audio codec chip — a distinctly
  later/different generation than the 1st-gen boards' YMZ280B/OKI PCM
  setup. Confirmed independently via the Arcade Otaku wiki's CV1000
  hardware page and Wikipedia's Cave company games table (cross-checked
  against each other rather than relied on alone).
- Cabinet: standard JAMMA wiring, no deluxe cabinet program — same
  situation as the 1st-gen boards.
- Important distinction from CAVE's earlier catalog: **DoDonPachi
  DaiOuJou, Ketsui, and the original Espgaluda did NOT run on Cave-built
  hardware** — per the same Cave company games table, those three ran on
  IGS's PGM board instead. Since IGS PGM hardware is still an unresearched
  "future round" candidate (see below), there is deliberately no preset
  covering those three titles yet — don't use either Cave preset for them.
- A second exception in the other direction: **Deathsmiles II: Makai no
  Merry Christmas did NOT run on CV1000** — the same games table lists it
  as "CAVE PC" hardware, a PC-based platform released in 2009 alongside
  Deathsmiles II. This preset is confirmed for the original Deathsmiles
  (2007, CV1000B) but not for Deathsmiles II. CAVE PC hardware hasn't been
  researched for this project; treat it as another future-round candidate.
- EQ implication: treated very similarly to `cave-1st-gen.txt` (clean
  digital source, light touch) since the cabinet/wiring facts are
  identical — the codec chip changes source fidelity slightly, not the
  cabinet acoustics, so a separately-derived EQ shape wasn't justified.

### Battle Garegga / Raizing-Eighting "Toaplan Version 2"-derived hardware
- Sound: **Yamaha YM2151 (FM) + OKI MSM6295 (ADPCM) + NMK112** — confirmed
  via vgmrips.net's chip listing for Battle Garegga. NMK112 is a bank-
  switching chip for the OKI ADPCM sample ROM (lets the M6295 address more
  sample banks); it doesn't generate audio itself, so the audible chip
  pairing is functionally the same YM2151+OKI combination CPS1 uses.
- Cabinet: standard JAMMA, no dedicated cabinet program (these boards were
  sold to run in whatever upright an operator already owned).
- EQ implication: because the chip pairing matches CPS1, `cps1.txt` is a
  reasonable starting point. A separate preset is provided mainly so this
  family has its own file to diverge from later if listening tests turn up
  a reason to (shmup sound design leans on distinct transient bullet/clink
  cues that a fighting game's mix doesn't), not because the hardware is
  confirmed to differ.

### Video System hardware (Sonic Wings / Aero Fighters)
- Sound: **Yamaha YM2610 @ 8 MHz + Z80 sound CPU @ 4 MHz** — confirmed via
  the Arcade Otaku wiki's Sonic Wings page. This is the *same FM+ADPCM chip*
  Neo Geo MVS uses.
- Cabinet: **not** an SNK MVS candy cabinet — Video System sold their own
  boards to run in standard JAMMA uprights, same as CPS1. (Neo Geo MVS mono
  candy cabinets happened to share a sound chip family with these boards
  purely by coincidence of both using YM2610, not because these games ran
  in MVS cabinets.)
- EQ implication: use the YM2610 FM+ADPCM tonal character, but do **not**
  apply the MVS preset's mono fold-down — that fold-down was justified by
  the MVS candy cabinet specifically, not by the chip. This is close to the
  CPS1 baseline with a touch more low-mid weight, since YM2610's ADPCM
  channels are commonly used for percussive/bass content in this era of
  shooters.

### Sega System 16 / System 16B / System 18
(Golden Axe, Shinobi, Altered Beast, Fantasy Zone, and most Sega arcade
titles of this generation)
- Sound: Yamaha YM2151 (FM) + Sega PCM (SegaPCM). Confirmed via vgmrips.net
  chip listings for this generation of Sega arcade titles.
- Cabinet: mostly standard JAMMA-compatible uprights, including Sega's own
  proprietary cabinet shells. No deluxe/subwoofer treatment — that's
  specific to some of Sega's later driving/gun-game hardware, not this
  generation's platformers and beat-em-ups.
- EQ implication: treated as a standard-JAMMA baseline close to CPS1, with
  a touch more low-mid warmth for SegaPCM's percussive/bass sample use.

### Konami K053260-era hardware
(Sunset Riders, Teenage Mutant Ninja Turtles: Turtles in Time, Asterix,
Vendetta, The Simpsons, and other early-90s Konami beat-em-ups/shooters)
- Sound: Yamaha YM2151 (FM) + Konami K053260, a 4-voice PCM/ADPCM chip with
  genuine stereo output. Confirmed via vgmrips.net's K053260 chip page and
  per-game credits on arcade-history.com.
- Cabinet: standard JAMMA.
- EQ implication: keep a touch more mid-range clarity than the pure-FM
  boards, since K053260 samples (percussion, voice clips) sit in that range
  and get buried by a boxiness cut tuned for FM-only content.

### Midway "DCS" hardware
(Mortal Kombat II, Mortal Kombat 3 / Ultimate MK3, NBA Hangtime, and other
mid-90s Midway titles)
- Sound: Analog Devices ADSP-2105 running Williams/Midway's proprietary DCS
  system — 16-bit digital audio, four+ independently controlled channels.
  Confirmed via Williams' own period documentation describing DCS as the
  first "truly high-fidelity, digital audio system designed specifically
  for coin-op arcade... games."
- Cabinet: standard JAMMA+ upright — and despite the hi-fi source, sound
  capability is documented as **mono** (e.g. Mortal Kombat 3's published
  tech specs list "Sound Capabilities: Mono").
- EQ implication: this is the odd one out. The source audio is already
  clean 16-bit digital content, closer to CD quality than anything else in
  this project, so the preset does the mono fold-down and small-driver
  correction *without* adding the brightness/presence boost the FM-era
  boards get — modeling a clean source through a mediocre mono cabinet
  speaker, not adding color that isn't there in the first place.

### Atari "JSA" sound board family
(STUN Runner, Hydra, Pit Fighter, Batman, ThunderJaws, and other late-80s/
early-90s Atari Games titles)
- Sound: Yamaha YM2151, paired with POKEY and/or OKI MSM6295 depending on
  revision. Confirmed directly from MAME's own driver source comments
  (`src/mame/atari/atarijsa.cpp`), which document four board revisions —
  JSA I (stereo), JSA II (mono), JSA III (mono), JSA IIIs (stereo) — each
  with its own list of games.
- Cabinet: standard JAMMA uprights.
- EQ implication: notable because Atari's own board-revision naming
  already tells you mono vs. stereo per game, rather than requiring outside
  research to establish it. The provided preset targets the mono
  revisions (JSA II/III), which cover most well-known titles on this
  hardware; a note in the file explains how to adapt it for JSA I/IIIs
  games instead.

---

## Candidates for a future round (not yet built)

These came up during research as clearly important but weren't built out
this round, to keep the confirmed/sourced bar consistent with everything
above rather than rushing weaker presets:

- **Data East hardware** (Bad Dudes, RoboCop, Midnight Resistance) — several
  distinct sound board generations, needs its own pass to avoid conflating
  them.
- **Namco System 1/2/22** (Ridge Racer, and earlier boards) — Tekken itself
  is actually Namco System 11/12 (PS1-derived), a different lineage worth
  treating separately from these earlier Namco boards.
- **IGS PGM hardware** (Knights of Valour, Oriental Legend) — very popular
  in the MAME community, Chinese-market hardware with its own cabinet
  conventions worth researching properly rather than assuming JAMMA norms
  apply identically.
- **Williams Y-Unit / pre-DCS Midway hardware** (original Mortal Kombat,
  Smash TV, Terminator 2) — the YM2151+OKI generation that came right
  before the DCS system above; a natural companion preset.
- **Irem M72/M92 hardware** (R-Type and follow-ups) — well documented, just
  not gotten to yet.

---

## Cross-cutting correction: JAMMA is mono by default

This affects every "preserve/enhance stereo" preset in this project (CPS2,
CPS3) and is worth stating plainly: the base JAMMA wiring standard defines
a **single mono Speaker +/- pair**, confirmed via the Arcade Otaku wiki's
JAMMA pinout page and corroborated by arcade-hobbyist forum discussion of
JAMMA vs. JAMMA+ wiring. True stereo output requires either the JAMMA+
extension or non-standard wiring an operator added themselves.

Practically: whether a CPS2 or CPS3 cabinet you're recreating actually
reproduced QSound's/CPS3's stereo width in the wild depended on how that
specific cabinet was wired, not just which board was inside it. The CPS2
and CPS3 presets here assume a stereo-wired setup (reasonable for CPS2,
since QSound was built specifically to be heard in stereo, and closer to
guaranteed for CPS3 given its dedicated stereo RCA output spec) — but if
you're trying to match a plain mono-JAMMA-wired cabinet instead, drop the
stereo-widening `Copy:` line from either preset and treat them as EQ-only.

---

## Sources referenced
- system16.com — arcade PCB/hardware identification and chip listings
- arcadeotaku.com wiki — CAVE 1st Generation Hardware, CPS2, CPS3, JAMMA
  pinout, and Sonic Wings hardware pages
- vgmrips.net — per-game chip identification (used for Battle Garegga)
- Furnace tracker chip documentation — QSound (DL-1425) and YMZ280B chip
  behavior, used here only for chip capability/architecture confirmation,
  not for any audio content
- Wikipedia (CP System III) — sourced in turn to system16.com; cross-checked
  against the Arcade Otaku wiki rather than relied on alone
- MAME source tree (`src/mame/`) — authoritative for exact chip/sample rate
  MAME actually emulates per driver; still the right place to double-check
  any of the above before treating it as final
- forum.arcadecontrols.com (BYOAC) — hobbyist corroboration of JAMMA vs.
  JAMMA+ stereo wiring, used only to confirm the standard's spec, not as a
  primary source
- arcade-history.com — per-game credits and chip listings (Sunset Riders)
- Williams/Midway period technical documentation (planetarypinball.com
  archival copy of "The DCS System" and "Ask Uncle Willy") — primary-source
  description of the DCS system's design intent and fidelity
- pixelatedarcade.com tech-spec pages — per-game sound capability (mono/
  stereo) and cabinet type, used for Mortal Kombat 3
- git.redump.net mirror of MAME source (`src/mame/atari/atarijsa.cpp`) —
  authoritative board-revision-to-game mapping for Atari's JSA family
- arcadeotaku.com wiki's CV1000 hardware page, cross-checked against
  Wikipedia's "Cave (company)" games table — used together specifically
  to catch the DoDonPachi DaiOuJou/Ketsui/Espgaluda exception (those ran
  on IGS PGM, not a Cave board), rather than assuming Cave's whole catalog
  ran on Cave-built hardware


---

## v6.3 source-level audit (2026-09-30)

The v6.3 audit deliberately separates **source hardware/routing** from **cabinet acoustics**. Current MAME source provides useful concrete routing evidence, but it does not provide a universal frequency-response measurement for the physical cabinets represented by these presets.

### Neo Geo MVS
The YM2610 architecture is documented by the NeoGeo Development Wiki, including the external YM3016 DAC and external analog mixing. The same project documents multiple MVS power-amplifier implementations: MV1B and MV1C are mono, while MV1F/MV1FZS are stereo; MV4/MV6 are listed as probably HA13001. Therefore the preset family must remain installation/board dependent. See `SOURCE-REGISTER-v6.3.md`.

### Cave CV1000 vs IGS PGM2
Current MAME identifies the CV1000 family as using Yamaha YMZ770C-F and routes only its right output into a mono speaker in the emulated machine configuration. MAME separately identifies DoDonpachi Daioujou Tamashii as an IGS PGM2 title using YMZ774-S with two-channel speaker routing. These are now separate presets in this package.

### Konami K053260
MAME demonstrates that this chip family cannot be reduced to one universal cabinet topology. The Simpsons and Overdriv drivers contain explicit stereo routing, while TMNT contains a mono speaker configuration. The project therefore treats the existing K053260 acoustic curve as experimental and its routing as title/PCB dependent.

### Midway DCS
MAME's DCS implementation distinguishes ROM-based mono DCS, DCS2 stereo, and Denver multi-channel variants. The existing `midway-dcs` preset is therefore only appropriate to the mono subset named in its header; it should not be generalized to every DCS-derived cabinet.

### QSound
MAME documents QSound as a 16-channel sample player with DSP16A, TDA1543 dual DAC and TC9185P volume-control hardware. The project therefore does not add a generic Haas/3-D widener to the CPS2 full preset merely because QSound sounds spatial; QSound's own source-level spatial processing is the relevant documented feature.

### What remains unknown
No source located in this audit provides a measured, universal arcade-cabinet frequency response, MVS cabinet IR, PGM2 cabinet IR, or measured cabinet T60 database. Accordingly, all existing GraphicEQ curves and Modal/T60 values remain **EXPERIMENTAL** until a physical measurement replaces them.
