# CRT TV Era Notes

These four presets (`presets/crt-tv/`) are a **separate category** from the
rest of this project. Everything else here models an arcade cabinet's own
speaker/amp/cabinet chain. These model a **home television's** built-in
speaker across four eras, based on Japanese market models and broadcast
standards where documentation allows. Don't combine a CRT-TV-era preset
with an arcade-cabinet preset (e.g. via two `Include:` lines at once) —
they represent two different, mutually exclusive physical playback
chains, not layers that stack.

## Shared basis across all four

Built-in TV speakers are characteristically weak below roughly 1 kHz and
above roughly 5 kHz, regardless of era — per a peer-reviewed thesis on TV
receiver speaker performance (Jeju National University), which describes
this as a structural limitation of small speakers in a thin TV cabinet.
All four presets share this same underlying "weak bass, weak treble,
mid-forward" shape; what changes between them is mono-vs-stereo routing,
how severe that shape is, and the cabinet-resonance modeling.

## Real reference models used

| Era | Model | What's documented |
|---|---|---|
| Pre-1978 | Sony Trinitron KV-1330UB (1972) / KV-1340E (1975) | Wood cabinet, permanent-magnet dynamic (moving-coil) speaker; KV-1340E specifically documented at 8×12 cm oval. Source: radiomuseum.org, citing Sony's own 1978 catalog and a 1975 Funk-Technik trade magazine. |
| 1979-1980s | Sony Trinitron KV-1901A (1980, Japan, 19") | Documented via crtdatabase.com. Speaker-specific detail is limited (listed simply as "Amplifier"), so this era leans more on the shared TV-speaker basis above than a unique measurement. |
| 1990s | *(not found)* | No single mainstream Japanese-domestic model with published speaker specs from this decade turned up in this research pass. This preset is the weakest-sourced of the four — built from the shared TV-speaker basis scaled down in severity, not from a named model. Flagged honestly rather than presented as equally well-sourced. |
| 2000s | Sony Trinitron KV-29X5K (29") | Documented via an elektroda.com teardown/service-manual writeup: built-in 2×20 W stereo speaker system. **Caveat:** this unit's documentation shows SCART sockets, a European AV connector standard not used in Japan, so it's used here only as a general "how capable had large deluxe CRT TV audio become by this point" data point, not asserted as Japanese-domestic. |

If you have access to better Japanese-market documentation for the 1980s
or 1990s specifically (a service manual with actual speaker
specifications, ideally), that's the single most useful thing that could
improve this file.

## Mono vs. stereo, by era — Japan's timeline, not the US's

This is worth getting right because it's a genuinely different story than
the US. **Japan's own stereo TV audio standard, EIAJ MTS, began
broadcasting in October 1978** — NHK's tentative stereo service in Tokyo
and Osaka — and was formally standardized by the EIAJ in January 1979
(Wikipedia's "EIAJ MTS" article, citing Numaguchi, *Present Status of
Multichannel-Sound Television Broadcasting in Japan*, IEEE Transactions
on Broadcasting, Dec. 1979). That's **more than five years before** the
US's BTSC standard (FCC adoption, 1984) — an earlier version of this
document assumed the US timeline applied here, which was a mistake for a
project specifically trying to use Japanese sourcing.

The real picture is more nuanced than "stereo from 1978," though:

- Sony's own **KV-1375** series is a nice concrete illustration of the
  transition actually happening in real products: the base 1977 KV-1375
  had no stereo provision; the **KV-1375A** added dual RCA mono outputs
  (for an external sound system, still mono); the **KV-1375D** (1978)
  added a 3.5mm "音声多重出力" (audio multiplex output) jack specifically
  for an outboard **multiplex sound adapter** (Sony's own MLV-2000) —
  meaning even once the broadcast standard existed, the TV set itself
  often still needed a separate box plugged in to actually produce
  stereo, rather than having an integrated second speaker and decoder
  built in from day one. (Source: consolemods.org's KV-1375 page.)
- Because of that, this project still treats the 1980s preset as
  single-speaker/mono by default — a Famicom-era TV with the multiplex
  jack but no adapter attached, or an older/cheaper single-speaker set,
  both remained plausible and probably more common than a full stereo
  setup. The header comment in `crt-tv-1980s.txt` explains this rather
  than just asserting mono outright, since the broadcast-capability fact
  and the actual-common-setup fact are two different things and
  shouldn't be blurred together.
- By the **1990s**, integrated stereo (decoder and two speakers in one
  set, no separate adapter box) was standard equipment — this is the
  point where the preset switches to stereo.

## What's NOT modeled, and why

TV audio circuits across all four eras also had real, audible amplifier
distortion and noise-floor characteristics that get more pronounced the
older the era. Equalizer APO's toolset is linear — it reshapes frequency
content and timing but cannot add harmonic distortion or saturation. Only
the frequency-response shape and mono/stereo routing are modeled here;
see `docs/research/VST.md` for what a future nonlinear processing stage could
add on top of this.

## Sources referenced
- radiomuseum.org — Sony Trinitron KV-1330UB and KV-1340E model pages
  (cabinet material, speaker type/size, citing Sony's 1978 catalog and a
  1975 Funk-Technik magazine issue)
- crtdatabase.com — Sony Trinitron KV-1901A model page
- consolemods.org — Sony Trinitron KV-1375/KV-1375A/KV-1375D model page
  (the multiplex-output-jack timeline)
- elektroda.com — Sony Trinitron KV-29X5K teardown/service-manual writeup
  (2×20 W stereo speaker spec)
- Wikipedia — "EIAJ MTS" article (Japan's stereo TV audio standard and
  its October 1978/January 1979 timeline), citing Numaguchi (1979) in
  IEEE Transactions on Broadcasting
- Jeju National University thesis on TV receiver speaker sound quality —
  general built-in-speaker frequency-response shape
