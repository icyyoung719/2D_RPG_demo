---
description: "Use when editing C++ gameplay, SFML integration, or CMake config in this repository. Covers module boundaries, build expectations, and path pitfalls."
applyTo:
  - "SFML_test/src/**/*.h"
  - "SFML_test/src/**/*.hpp"
  - "SFML_test/src/**/*.c"
  - "SFML_test/src/**/*.cc"
  - "SFML_test/src/**/*.cpp"
  - "CMakeLists.txt"
  - "SFML_test/CMakeLists.txt"
---

# C++ SFML Editing Guidelines

- Keep module boundaries stable: `core` coordinates systems, `entities` holds gameplay objects, `world` handles map/tileset parsing, and `graphics` handles animation/state visuals.
- Preserve existing include style and directory layout shown in [docs/ProjectStructure.md](../../docs/ProjectStructure.md#L1).
- Avoid introducing path assumptions. Existing runtime asset path uses `assests` spelling and a relative root from executable location.
- For build changes, keep CMake compatible with Visual Studio generator and current CI usage documented in [docs/Build.md](../../docs/Build.md#L1) and [.github/workflows/ci.yml](../workflows/ci.yml#L1).
- If a change impacts map or tileset loading, reference [docs/Assets_and_Maps.md](../../docs/Assets_and_Maps.md#L1) and maintain JSON compatibility.
- Keep changes focused and minimal; do not change gameplay behavior unless explicitly requested.