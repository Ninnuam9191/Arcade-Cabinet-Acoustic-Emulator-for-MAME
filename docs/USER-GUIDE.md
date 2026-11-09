# User Guide

> **New to this?** Start with [`SETUP-GUIDE.md`](SETUP-GUIDE.md): step by step, with pictures, no technical knowledge needed. This page is the reference version.

## 1. Install Equalizer APO

Install Equalizer APO and use its Configurator to select the playback device used by MAME. Confirm the device is active before changing profiles.

## 2. Install the project

Copy the project's `config.txt` and `presets/` directory into the Equalizer APO configuration directory after backing up your existing configuration.

Do not overwrite a personal configuration without a backup.

## 3. Choose one core profile

Open the supplied `config.txt` and enable exactly one `Include:` line.

Examples:

```text
Include: presets/arcade/neogeo-mvs.txt
```

or:

```text
Include: presets/crt-tv/crt-tv-1990s.txt
```

Arcade and CRT-TV profiles are alternatives. Do not stack them unless you intentionally want both transformations.

## 4. Use the Configuration Editor

The Configuration Editor is the preferred interface for inspecting the active chain and adding optional VST plugins. The text files remain the portable/canonical representation of the project's linear profiles.

Equalizer APO supports `Include:` files, so the selector can stay simple while each profile remains isolated in its own file.

## 5. Optional experimental VST layer

After the core profile works, you can add nonlinear coloration.

Start with:

`presets/experimental-vst/`

Recommended first test: load the normal `presets/arcade/neogeo-mvs.txt`, add the ArcadeAmp row in the Configuration Editor (below Preamp), and set `Preset` to `Medium`.

See `docs/SETUP-GUIDE.md` (Part B, with pictures) or `docs/VST-SETUP.md` for the complete installation procedure, recommended settings and troubleshooting.

The VST profiles are experimental. They add nonlinear colour but are not measured amplifier models.

## 6. Peace

`presets-peace/` contains simplified versions intended for Peace import/use.

Use the full APO profiles when you need project-specific routing or Equalizer APO features that Peace cannot represent one-for-one.

## 7. Convolution / IRs

A genuine measured cabinet/system IR can provide a much stronger representation of the linear acoustic chain than an invented EQ curve.

Do not assume an IR and the experimental VST layer should always be stacked. A full-chain IR may already include the linear cabinet/speaker response.

## 8. Troubleshooting

### No audible change

Check:

- Equalizer APO is installed on the actual playback device.
- The profile's `Include:` line is uncommented.
- The file path is correct relative to `config.txt`.
- The Configuration Editor reports no configuration errors.

### VST does not load

Confirm that a compatible Windows VST2 DLL is installed and that the plugin is visible to the Configuration Editor. Remove the VST if it causes instability and verify the core profile independently.

Equalizer APO's own troubleshooting documentation also covers driver APO conflicts and Configurator troubleshooting options.

### Sound is too loud or clips

Several profiles use negative preamp values because some filters add positive gain. Do not raise the preamp merely to restore volume until clipping has been ruled out.

## 9. Verification

When comparing profiles, keep the MAME game, playback device, Windows volume, sample rate and listening position fixed.

Compare:

1. Core linear profile.
2. Same profile + VST Light.
3. Same profile + VST Medium.

If the VST version sounds less convincing, use the core profile. The experimental layer exists to test whether controlled nonlinear coloration improves the perceptual result; it is not automatically better.
