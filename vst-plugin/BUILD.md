# Building and testing ArcadeAmp

The binaries in `dist/` were cross-compiled from Linux to Windows x86-64
with mingw-w64 (no Windows machine involved in the build). Everything here
is reproducible from the files in `source/`.

## 1. Run the tests (any Linux/macOS/Windows machine with a C++ compiler)

```sh
cd vst-plugin/tests
g++ -O2 -Wall -I../source -o dsp_test dsp_test.cpp -lm
./dsp_test
```

It exits 0 and prints `ALL TESTS PASSED` when everything checks out. The
result recorded for the shipped binaries is `tests/dsp_test-output.txt`.
The test includes `source/ArcadeAmpDSP.hpp` directly — the same file the
plugin compiles — so it tests what ships, not a copy of the maths.

## 2. Build the plugin

The plugin window uses DPF's NanoVG/OpenGL UI, which needs DPF's `pugl` submodule (hence `--recurse-submodules`; without it the build stops at `pugl/pugl.h`). Needs: a mingw-w64 C++ compiler, `make`, and the
[DPF](https://github.com/DISTRHO/DPF) framework (ISC licence), which is not
vendored here.

Debian/Ubuntu: `sudo apt-get install g++-mingw-w64-x86-64 make git`

Layout the Makefile expects (DPF is found at `../DPF` relative to the
plugin folder):

```
build-dir/
  DPF/            <- git clone --depth 1 --recurse-submodules https://github.com/DISTRHO/DPF.git
  ArcadeAmp/      <- copy of this repo's vst-plugin/source/
```

```sh
mkdir build-dir && cd build-dir
git clone --depth 1 --recurse-submodules https://github.com/DISTRHO/DPF.git
cp -r /path/to/repo/vst-plugin/source ArcadeAmp
cd ArcadeAmp
make CC=x86_64-w64-mingw32-gcc \
     CXX=x86_64-w64-mingw32-g++ \
     AR=x86_64-w64-mingw32-ar \
     WINDRES=x86_64-w64-mingw32-windres \
     vst2 vst3 clap
```

Output appears in `build-dir/bin/` (DPF's own convention: next to the DPF
folder): `ArcadeAmp-vst.dll` (VST2 — rename to `ArcadeAmp.dll` if you like),
`ArcadeAmp.vst3/`, `ArcadeAmp.clap`.

## 3. What was checked on the built binaries

From the Linux side only:

| Check | Result for this release |
|---|---|
| `file` reports a real `PE32+ executable (DLL) x86-64 … for MS Windows` | yes, all three |
| VST2 DLL exports `VSTPluginMain` and `main` | yes (also `DllMain`) |
| Only system DLL imports | `KERNEL32.dll`, `msvcrt.dll`, `SHELL32.dll` — no mingw runtime DLL needed |
| New parameter names embedded in the binary | `Preset`, `Drive`, `Warmth`, `Hiss`, `Hiss Tone`, `Whine`, `Mains 50Hz`, preset label `TV early` (`Sag` and `Hum` are shorter than the `strings` tool's default minimum) |
| Old parameter names (`Noise`, `Mix`, `Noise Color`, `Mains Hz`) absent | yes |

The wrapper itself (parameter list, Preset selector, level behaviour) was
also exercised through the VST2 API by `tests/host_test.cpp`; see
`README.md`. Not covered by any of this: actually loading it in Equalizer APO
and listening.

## Changing the DSP

Edit only `source/ArcadeAmpDSP.hpp`. Re-run the tests first; every claim
in `README.md` that has a number next to it is checked there. If you
change a documented behaviour (for example the noise level mapping), update
the README table and the test's expectations together.

## Running the host test (Linux)

`tests/host_test.cpp` loads a Linux build of the same plugin wrapper through
the VST2 API (it declares the small VST2 structure itself, so no Steinberg
header is needed). In the build folder from step 2:

```sh
make clean && make vst2          # native Linux build, no mingw flags
g++ -O1 -o host_test /path/to/repo/vst-plugin/tests/host_test.cpp -ldl
./host_test ../bin/ArcadeAmp-vst.so
```

## Running the Windows tests under Wine (Linux)

`tests/winhost_test.cpp` (panel-style parameter queries) and
`tests/winui_test.cpp` (opens the plugin window like Equalizer APO does and
clicks it) are Windows programs. Build them with mingw and run them against
`dist/ArcadeAmp.dll`:

```sh
x86_64-w64-mingw32-g++ -O1 -static -o winhost.exe tests/winhost_test.cpp
x86_64-w64-mingw32-g++ -O1 -static -mwindows -o winui.exe tests/winui_test.cpp
cp dist/ArcadeAmp.dll .
wine winhost.exe ArcadeAmp.dll
xvfb-run -a wine winui.exe ArcadeAmp.dll     # needs a (virtual) display
```
