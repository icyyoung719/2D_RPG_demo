---
name: cmake-build-troubleshooting
description: 'Troubleshoot CMake configure/build failures for this SFML project. Use for SFML_DIR issues, generator mismatch, and static-vs-dynamic SFML link errors.'
argument-hint: 'Optional failing command or error text'
user-invocable: true
---

# CMake Build Troubleshooting

## When to Use
- CMake configure fails on this repository.
- Build fails with unresolved SFML symbols.
- Visual Studio and Ninja builds behave differently.

## Procedure
1. Read [AGENTS.md](../../../AGENTS.md) and [docs/Build.md](../../../docs/Build.md#L1).
2. Re-run the smallest failing command from repository root.
3. Classify failure:
   - Missing or wrong `SFML_DIR`
   - Generator/toolchain mismatch
   - Static-vs-dynamic SFML linkage mismatch
   - Source-level compile error
4. Suggest the minimal fix and explain why.
5. Re-run configure/build to verify the fix.

## Notes
- Keep edits minimal and avoid gameplay or asset changes unless explicitly requested.
- If tests are requested, state that this repository currently has no automated test suite in CI.