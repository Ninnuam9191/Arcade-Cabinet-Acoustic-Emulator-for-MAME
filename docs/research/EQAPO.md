# Equalizer APO features used in this project

Everything below is taken from Equalizer APO's own official Configuration
Reference (`sourceforge.net/p/equalizerapo/wiki/Configuration reference`),
the authoritative source for what the software actually supports and the
exact syntax it expects. Earlier preset versions in this project only used
three tools from this list (Preamp, PK/LS/HS filters, and a basic Copy:
mono fold-down). This revision uses more of what's actually available.

## What changed and why

### 1. GraphicEQ instead of 3–4 peaking/shelf filters

**What it is:** `GraphicEQ: <freq> <gain>; <freq> <gain>; ...` — an
arbitrary multi-point curve, linearly interpolated in log-frequency space
between points. Officially supported since version 1.0.

**Why it's better here:** a handful of PK/LS/HS filters can only produce
bell curves and shelves — smooth, but not the actual shape of a small
driver's rising/falling response or a baffle's diffraction curve. A 9–11
point GraphicEQ can trace that shape directly instead of approximating it
with 3 overlapping bells. Every preset below now uses one.

### 2. Modal filters with a T60 decay time, for cabinet panel resonance

**What it is:** one of the filter types Equalizer APO supports is `Modal`,
syntax `ON Modal Fc <freq> Gain <dB> Q <value> T60 target <time> ms`. Unlike
a plain peaking filter, a modal filter has an actual **decay time**
parameter (T60 — the standard acoustics term for "time to decay 60 dB",
borrowed from room acoustics) — it doesn't just change the gain at a
frequency, it models a resonance that *rings* for a specific duration after
being excited.

**Why it's better here:** the "boxy cabinet" character in the earlier
presets was represented as a static gain cut at a frequency — real cabinet
panels don't just have a frequency response dip, they physically resonate
and ring for a measurable amount of time when struck by sound, and that
ring time depends on the panel material. A thin sheet-metal panel (Neo Geo
MVS candy cabinets, many Midway steel-clad uprights) rings longer at a
higher Q than a thicker, more damped wood/particleboard panel (the typical
JAMMA upright shell most CPS1/CPS2/Cave/Konami/Sega System 16/Atari JSA
titles ran in). Each preset's Modal filter uses a T60 consistent with its
documented cabinet material — these are estimated qualitative values
(metal decays noticeably longer than damped wood, which is a standard
result in acoustics, not a specific measurement of a specific cabinet), not
a substitute for an actual measured decay time. That refinement is exactly
what real impulse-response capture (the planned future phase) would
provide.

### 3. Delay-based stereo widening (Haas/precedence effect) for the stereo
   hardware, instead of only a gain matrix

**What it is:** `Delay: <t> ms`, scoped to one channel via `Channel: R` /
`Channel: L`, supported since version 0.9.

**Why it's better here:** the earlier CPS2 preset widened stereo using
only a gain matrix (`Copy: L=1.15*L+-0.15*R ...`). That works, but it's a
loudness-based trick. A short (5–8 ms) delay on one channel relative to the
other is the basis of the well-established Haas/precedence effect (Haas,
1951): the ear localizes sound toward whichever channel arrives *first*,
even by a few milliseconds, which reads perceptually as stereo width. Kept
short (under ~15 ms) it reads as spaciousness, not as an echo — that
becomes audible only well past that range. CPS2's preset now uses a short
delay on the right channel plus a light version of the original gain
matrix, rather than gain alone.

### 4. Explicit Channel: scoping

**What it is:** `Channel: L`, `Channel: R`, `Channel: all`, etc. — scopes
every following Filter/Preamp/Delay command to specific channels until the
next `Channel:` line changes it. Supported since version 0.8.

**Why it's used here:** it's the mechanism the Delay-based widener above
needs (applying a delay to only one channel), and makes the mono
fold-downs and stereo-specific treatments explicit and easy to audit in
the file, rather than relying only on the Copy: matrix to encode channel
behavior.

### 5. Preamp values, corrected twice

**First pass:** earlier versions had hand-picked `Preamp:` values that
were inconsistent enough to cause real clipping at some presets' peak
gain frequency - audible as harshness, not just loudness. The fix at that
point was to compute each preset's actual combined GraphicEQ+Modal peak
gain (sampled across 20 Hz-20 kHz; since these are filters in series, or
a cascade, their dB responses at a given frequency add directly - this is
exact, not an approximation) and set Preamp to the negative of that peak
plus a safety margin, assuming the source audio could hit 0 dBFS.

**That assumption was wrong, and made things worse.** MAME deliberately
runs its audio output below full scale on purpose - confirmed directly on
MAME's own bug tracker (issue #6840): a senior MAME tester explains this
is intentional so that multi-chip mixing scenarios (e.g. a Sega CD or 32X
attached) don't clip once summed, though the MAME developers did not
specify exactly how many dB of headroom this leaves. So assuming the
source could be at 0 dBFS and computing a fully worst-case-safe Preamp
was systematically too conservative for how MAME actually behaves,
producing presets quiet enough that some barely reached 30-40% of the
meter - exactly the opposite problem.

**Current approach:** rather than assuming 0 dBFS, Preamp now assumes a
middle-estimate 6 dB of existing source headroom (informed by that
meter-level feedback plus MAME's confirmed-but-unquantified deliberate
headroom practice - see the note above) and only cuts for whatever a
preset's peak gain exceeds that, plus a small 0.5 dB margin. This is
still a real safety margin, not zero - it's a deliberately chosen middle
ground, not a measured fact, and this document says so rather than
implying it's exact.

**Because it's an estimate, not a fact, calibrate it for your own setup:**
if a preset still sounds too quiet, or you hear clipping on the loudest
moment in a game (a big explosion, a boss's entrance jingle), adjust that
preset's `Preamp:` line in 1 dB steps - up if quiet, down if you hear
distortion specifically on the loudest moments - and leave it there. This
project can get you a well-reasoned starting point; only your own speakers
and your own copy of MAME can get you the exact right number.

### 6. Band-pass (`BP`) instead of separate `HP`/`LP` for narrow-bandwidth
   sources

Used in the CRT-TV-era presets (see `presets/crt-tv/`): where an entire
audio path's passband is the thing being modeled (a 1970s TV's audio
circuit, not a driver's individual low/high rolloff), a single `BP`
filter is the more direct tool than stacking `HP` and `LP` lines - it's
one filter expressing one physical fact (this circuit passes roughly this
band), rather than two filters that happen to produce a similar shape.

## Why Peace's Import button didn't work, and what changed

Peace is a GUI built on top of Equalizer APO, but it maintains its **own**
internal preset model — a fixed-shape list of numbered filter slots, each
holding a filter type/frequency/gain/Q. That model only recognizes plain
`Filter N: ON <type> Fc ... Gain ... Q ...` lines using the small set of
types its own editor exposes (PK/LS/HS in practice). It has no internal
concept of a `GraphicEQ:` curve, a `Copy:` channel matrix, a `Delay:` line,
`Channel:` scoping, or a `Modal` filter's `T60` parameter — those aren't
"a filter type Peace doesn't support," they're not filters in Peace's model
at all, so its Import parser has nothing to put them into and the import
silently fails or produces nothing usable.

This is a limitation of Peace's own preset editor, not of Equalizer APO —
Equalizer APO itself runs every one of these commands perfectly well when
they're pasted directly into `config.txt`, which is why that method
(described in the main README) still gets you the full preset with all
four techniques from this document intact.

To make Peace's Import button actually work, this project now ships two
versions of every preset:

- **`presets/arcade/`** — the complete version described throughout this
  document (GraphicEQ, Modal+T60, Delay-based widening, Channel scoping).
  Use this by pasting it directly into `config.txt`. This is the
  "real" preset.
- **`presets-peace/`** — a version restated entirely as numbered
  `Filter N: ON PK ...` lines so Peace's Import button works. It's a
  translation, not a downgrade in most respects:
  - Each `GraphicEQ:` point becomes its own peaking (`PK`) filter at the
    same frequency and gain, with a fixed Q of 1.4. This specific
    conversion (graphic-EQ band → peaking filter at Q 1.4) is a standard,
    documented technique for going from a graphic to a parametric
    representation, not something invented for this project.
  - Each `Modal` filter becomes a plain `PK` filter with the same
    frequency/gain/Q — this part **is** a real downgrade: the explicit
    decay-time (T60) behavior is lost, because Peace's model has nowhere
    to put it. The frequency-domain shape survives; the time-domain
    "ring" does not.
  - Mono fold-downs, the CPS2 stereo widener, and the small-driver HP/LP
    band-limiting are left out of the Peace-importable file entirely,
    because Peace's own Effects panel already has native GUI controls for
    exactly these things (up/down-mix, delay, crossfeed — per Peace's own
    "How to use Peace" documentation) — using those is more reliable than
    trying to smuggle the equivalent through Import. `presets-peace/README.md`
    has the specific Effects-panel steps for each preset that needs one.

## Available but deliberately not used yet

- **Convolution** (`Convolution: <file>`) — the real impulse-response
  route discussed earlier. Deferred on purpose; see
  `impulse-responses/README.md` for how the presets are structured to
  drop this in later without a rewrite.
- **VSTPlugin:** (not in the official Configuration Reference; the discussion
  archive shows `VSTPlugin: Library "<path>"`) — the amplifier-nonlinearity and
  noise-floor use case is now an opt-in layer, the project's own ArcadeAmp
  plugin in `vst-plugin/`; cabinet convolution
  via NadIR remains deferred. See `VST.md` for both, and the
  VST3 caveat (stock Equalizer APO doesn't speak it without a fork or
  bridge).
- **Device:** — scopes a block of config to a specific *audio output
  device* (matched by device name/connection/GUID), not to a specific
  running application or game. Worth being precise about this: there is no
  Equalizer APO feature that detects which MAME ROM is running — that's
  exactly why the external switcher script is still a separate, later
  piece of work, not something achievable from inside an EQ APO config
  file alone.
- **Stage:** (pre-mix vs. post-mix vs. capture) — lets filtering happen
  per-application (pre-mix) instead of on the final mixed device output
  (post-mix, the default, used here). Not used because there's no need to
  treat different applications differently within a single preset.
- **If/ElseIf/Eval/inline expressions** (e.g. branching on `sampleRate`,
  `deviceName`) — a real embedded expression language Equalizer APO
  supports for conditional configs. Not used here because nothing in this
  project currently needs runtime branching; noted here so it's not
  forgotten as an option for the switcher-script phase, where something
  like matching on `deviceName` could plausibly be relevant.
- **Custom-coefficient IIR filters** (`ON IIR Order <n> Coefficients ...`)
  — lets you supply a raw filter transfer function instead of using the
  built-in types. No current need for this; the built-in Modal/GraphicEQ/
  Delay combination covers everything used so far.

## A practical requirement this creates

GraphicEQ, Modal filters, Delay, and Copy all require **Equalizer APO
version 1.0 or newer** (Modal/Delay/Copy since 0.9, GraphicEQ/Convolution
since 1.0). Equalizer APO silently ignores any command it doesn't
recognize rather than showing an error, so on a very old install these
presets would partially apply (e.g. the Preamp would work but the
GraphicEQ/Modal lines would just do nothing) with no warning that
anything was skipped. Practically: install a current version rather than
whatever an old guide/mirror points to. This is called out in the main
README's troubleshooting section too.

## Source

All syntax, version-support notes, and behavior descriptions above are
drawn directly from: Equalizer APO — Configuration Reference,
`sourceforge.net/p/equalizerapo/wiki/Configuration reference` (official
project wiki, authored by the project maintainer).
