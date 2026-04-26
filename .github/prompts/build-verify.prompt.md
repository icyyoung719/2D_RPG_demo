---
name: "Build Verify (Windows VS)"
description: "Run a quick CMake build verification on Windows for this SFML project and summarize errors with actionable fixes."
argument-hint: "Optional focus (for example: linker errors, SFML_DIR, map loading)"
agent: "agent"
tools: [read, search, execute]
---

Validate this repository's build on Windows using CMake + Visual Studio conventions.

## Inputs
- Optional focus from user argument.

## Steps
1. Check [docs/Build.md](../../docs/Build.md#L1) and [AGENTS.md](../../AGENTS.md) for expected commands.
2. Run configure/build commands from repository root:
   - `cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DSFML_DIR="C:/path/to/SFML/lib/cmake/SFML"`
   - `cmake --build build --config Debug`
3. If configure fails due to missing `SFML_DIR`, report exact remediation without editing unrelated files.
4. Summarize build result and the top actionable fixes.

## Output format
- Status: pass/fail
- Commands run
- Key errors (if any)
- Suggested fixes