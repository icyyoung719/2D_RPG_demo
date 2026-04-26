# AGENTS.md

## Purpose
- Keep AI coding agents productive with minimal codebase guessing.

## Build And Run
- Preferred local workflow is CMake + Visual Studio on Windows.
- Build docs: [docs/Build.md](docs/Build.md#L1)
- Project CMake entry: [CMakeLists.txt](CMakeLists.txt)
- Demo target: `SFML_test`

```powershell
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DSFML_DIR="C:/path/to/SFML/lib/cmake/SFML" ..
cmake --build . --config Debug
.\x64\Debug\SFML_test.exe
```

## Architecture Pointers
- Source root: `SFML_test/src/`
- Core game loop and resources: `SFML_test/src/core/`
- Entities and gameplay objects: `SFML_test/src/entities/`
- Map and tileset loading: `SFML_test/src/world/`
- Project structure details: [docs/ProjectStructure.md](docs/ProjectStructure.md#L1)
- Overview docs: [docs/Overview.md](docs/Overview.md#L1)

## Conventions
- Keep edits minimal and consistent with existing C++ style.
- Respect `SFML_DIR`; do not assume machine-wide SFML installation.
- Prefer linking existing docs instead of copying long documentation into instructions.
- Use `assests` spelling as-is when referencing existing asset paths.

## Safety Rules
- Do not modify gameplay logic or assets unless user asks explicitly.
- If no tests exist for a change, report that clearly in your summary.

## Common Pitfalls
- Runtime asset path in `SFML_test/src/main.cpp` uses a relative path and can break if the working directory changes.
- `SFML_test/CMakeLists.txt` enables static SFML linkage; this can conflict with dynamic SFML installs.

## Documentation Index
- Build: [docs/Build.md](docs/Build.md#L1)
- Controls: [docs/Controls.md](docs/Controls.md#L1)
- Assets and maps: [docs/Assets_and_Maps.md](docs/Assets_and_Maps.md#L1)
- CI workflow: [.github/workflows/ci.yml](.github/workflows/ci.yml#L1)
