# Impulse responses (future phase)

This folder is a placeholder. Convolution-based cabinet modeling was
deliberately deferred (see the project discussion history) in favor of
getting the documented, sourced EQ-based presets right first.

When real impulse responses exist (captured per the sine-sweep/deconvolution
method discussed for this project, or contributed by cabinet owners), they
belong here as `.wav` files, one per cabinet/system, and each preset file
would gain one additional line:

```
Convolution: ../impulse-responses/<name>.wav
```

placed after the existing filters. No other changes to the preset files
would be needed — this is why the presets are organized the way they are
now, so this stays a one-line addition per file later rather than a
rewrite.

Requirements when that day comes (from Equalizer APO's own documentation):
the impulse response file's sample rate must exactly match your playback
device's sample rate, or Equalizer APO will refuse to load it.

## MVS provenance rule

No file in this repository is labelled as an "MVS IR" unless it is actually
measured from an MVS cabinet/speaker path. A generic speaker IR may be used as a
clearly labelled proxy for experimentation, but it must not be represented as
SNK/MVS measurement data.
