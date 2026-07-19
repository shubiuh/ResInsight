# Building ResInsight from Source on Windows

This guide walks you through building **ResInsight** on Windows using Visual Studio 2022, vcpkg, and Qt 6.

For cross-platform guidance (Ubuntu, etc.), see [docs/agents/build.md](docs/agents/build.md) and the [official build documentation](https://resinsight.org/releases/build-from-source/).

---

## 1. Prerequisites

Install these tools before you start. Versions shown are the **minimum / recommended** pair.

| Tool                  | Minimum | Recommended | Notes                                                              |
| --------------------- | ------- | ----------- | ------------------------------------------------------------------ |
| Visual Studio         | 2019    | 2022 (17.8+) | Requires the **"Desktop development with C++"** workload and CMake tools |
| Python                | 3.8     | 3.11+       | Needed for `aqtinstall` and the Python API tests                  |
| Qt                    | 6.4     | 6.6         | Qt 6.4 works, Qt 6.7 has known theming issues                     |
| CMake                 | 3.26+   | Latest      | Required by Boost dependency                                       |
| Git for Windows       | Any     | Latest      | Needed for submodules                                              |

### Qt notes

- **6.5** is the official minimum listed in project docs.
- **6.4** compiles but is missing some features.
- **6.7+** has known theming bugs — avoid unless you have a specific reason.
- Required Qt modules: base, charts, networkauth, svg (private headers may be needed).

---

## 2. Clone the Repository

```powershell
git clone https://github.com/OPM/ResInsight
cd ResInsight
git submodule update --init
```

> Always run `git submodule update --init` after a fresh clone or after switching branches — submodules contain critical third-party code.

---

## 3. Bootstrap vcpkg

ResInsight bundles vcpkg as a submodule under `ThirdParty/vcpkg/`. Bootstrap it once after cloning:

```powershell
# Run from an "x64 Native Tools Command Prompt for VS 2022" (Administrator if you hit permission errors)
ThirdParty\vcpkg\bootstrap-vcpkg.bat
```

This fetches vcpkg and installs all C/C++ dependencies declared in `vcpkg.json` automatically when CMake runs.

---

## 4. Install Qt

Pick **one** of the two methods below.

### Option A — `aqtinstall` (recommended for automation / reproducible builds)

```powershell
python -m venv myvenv
myvenv\Scripts\activate
pip install aqtinstall
aqt install-qt windows desktop 6.6.3 win64_msvc2019_64 -m qtnetworkauth
```

Note the Qt install path — you'll need it for the next step. Typically:

```
C:\Users\<you>\Qt\6.6.3\msvc2019_64
```

### Option B — Official Qt Online Installer

1. Download the installer from [qt.io](https://www.qt.io/download-qt-installer).
2. Run `MaintenanceTool.exe`.
3. Select **Qt → Qt 6.6.x → MSVC 2019 64-bit**.
4. Under the matching Qt version, make sure these components are checked:
   - `MSVC 2019 64-bit` (or matching kit)
   - Additional Libraries → **Qt Network Authorization (qtnetworkauth)**
   - Sources / Debug information (optional, useful for debugging)

---

## 5. Configure CMake

### 5a. Create your user preset

Copy the Windows preset example into `CMakeUserPresets.json` at the repository root:

```powershell
copy CMakeUserPresets-example-windows.json CMakeUserPresets.json
```

Open `CMakeUserPresets.json` and update `CMAKE_PREFIX_PATH` to point at your Qt install:

```json
{
  "version": 6,
  "configurePresets": [
    {
      "name": "x64-relwithdebinfo",
      "inherits": "CMakePresets.json:x64-relwithdebinfo",
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "C:/Users/<you>/Qt/6.6.3/msvc2019_64"
      }
    }
  ]
}
```

> Adjust the path to match your actual Qt installation. Use forward slashes.

### 5b. Available Windows presets

| Preset                | Description                              |
| --------------------- | ---------------------------------------- |
| `x64-release`         | Optimized release build                  |
| `x64-relwithdebinfo`  | Release with debug info (recommended)    |
| `x64-debug`           | Debug build (slow to compile)            |

---

## 6. Build

### Option A — Visual Studio (recommended GUI)

1. Launch **Visual Studio 2022** as Administrator.
2. **File → Open → Folder** → select the cloned `ResInsight` directory.
3. Visual Studio detects `CMakePresets.json`. If prompted to configure, accept.
4. In the configuration dropdown, pick **`x64-relwithdebinfo`**.
5. **Build → Build All** (or right-click `ResInsight` → **Build**).

The first configure step will fetch and compile vcpkg dependencies (Boost, etc.) — this can take **30–90 minutes** the first time. Subsequent builds are much faster.

The build output binary is placed under:

```
build\x64-relwithdebinfo\ResInsight.exe
```

(or `build\RelWithDebInfo\ResInsight.exe` depending on the preset).

### Option B — Command line

Always use **`cmake --build`**, never `ninja` directly. Visual Studio injects its own bundled `ninja.exe` into `CMAKE_MAKE_PROGRAM`; calling a different `ninja` from your shell forces a full rebuild because the `.ninja_log` formats are incompatible.

```powershell
# Configure
cmake . --preset=x64-relwithdebinfo

# Build everything
cmake --build --preset x64-relwithdebinfo

# Build only the main executable
cmake --build --preset x64-relwithdebinfo --target ResInsight

# Build a utility target used by the project (e.g. to refresh embedded data)
cmake --build --preset x64-relwithdebinfo --target extract-projectfile-versions
```

If a build unexpectedly rebuilds far more than expected, check:

```powershell
where ninja                                          # multiple ninja.exe on PATH = bad
grep CMAKE_MAKE_PROGRAM build\CMakeCache.txt         # should point at VS-bundled ninja
```

---

## 7. Run ResInsight

After a successful build, launch the executable from the build output directory, e.g.:

```powershell
.\build\x64-relwithdebinfo\ResInsight.exe
```

For development iterations, just re-run `cmake --build --preset x64-relwithdebinfo` and re-launch.

---

## 8. Run the Tests

### C/C++ unit tests (CTest)

```powershell
# All tests for the RelWithDebInfo configuration
ctest --preset x64-relwithdebinfo

# Or directly
cmake --build --preset x64-relwithdebinfo --target test

# Filter to a specific suite
ctest -R "UnitTests" -C RelWithDebInfo
ctest -R "opm-parser-tests" -C RelWithDebInfo
ctest -R "roffcpp-tests" -C RelWithDebInfo
ctest -R "regression-analysis-tests" -C RelWithDebInfo
```

### Python API tests

These exercise the gRPC-based Python interface and require `ResInsight.exe` to be available.

```powershell
python -m venv venv-claude
venv-claude\Scripts\Activate.ps1
pip install pytest

cd GrpcInterface\Python\rips
$env:RESINSIGHT_EXECUTABLE = "..\..\..\..\build\RelWithDebInfo\ResInsight.exe"

# Example: run only the polygons tests
python -m pytest tests/test_polygons.py --console
```

---

## Troubleshooting

**`qtnetworkauth` not found / `Qt6NetworkAuth` not found during configure.**
You installed Qt without the Network Authorization module. Reinstall Qt with `qtnetworkauth` (Option A via `aqt`, or check it in the Qt Maintenance tool).

**CMake complains about missing compiler.**
Launch the **x64 Native Tools Command Prompt for VS 2022** (Start menu → search for it) instead of a plain `cmd`. Or open Visual Studio and let it manage the environment.

**Long configure time on first build.**
This is normal — vcpkg is building Boost and other heavy C++ dependencies from source. Subsequent builds reuse the installed binaries.

**Visual Studio keeps rebuilding from scratch.**
You're probably calling `ninja.exe` from a different shell than VS. Use `cmake --build` consistently and verify `CMAKE_MAKE_PROGRAM` points at VS's bundled ninja.

**Submodule-related errors.**
Run `git submodule status` and re-init any missing submodules:

```powershell
git submodule update --init --recursive
```

---

## Reference

- Official guide: <https://resinsight.org/releases/build-from-source/build-instructions-windows/>
- In-repo build reference: [docs/agents/build.md](docs/agents/build.md)
- CMake Presets in VS: <https://learn.microsoft.com/en-us/cpp/build/cmake-presets-vs>
- CMake configuration docs: <https://resinsight.org/releases/build-from-source/cmake-configuration/>
