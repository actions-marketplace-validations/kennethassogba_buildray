# Contributing

Documentation changes run the site build and link checks. Code, build, Action and workflow changes also run the C++ tests.

Keep changes small and include a test that exercises the behavior being changed.
Use real before/after measurements for performance claims. A passing build alone
does not prove a speedup.

## Where the code lives

- `src/main.cpp` dispatches commands; `src/cli.cpp` parses arguments and prints help.
- `src/pipeline.cpp` coordinates snapshots, validation and measurements.
- `src/cmake.cpp` finds and edits CMake declarations; `src/linking.cpp` checks archive use and link equivalence.
- Other files in `src/` handle includes, headers, profiling, dependencies and Unity.
- `action/run.py` connects GitHub Action inputs to the same CLI.
- `tests/` contains native unit tests and Python tests that run real CMake projects.

The C++ headers under `include/buildray/` are internal implementation interfaces.
The CLI, `action.yml` and saved reports are the user-facing interfaces. Preserve
existing command names and report fields when adding simpler ways to use them.

## Checks

Follow the build steps in the [README](README.md), then run:

```bash
ctest --test-dir build --output-on-failure --parallel 1
npm ci
npm run docs:build
npm run docs:check
```

Keep compiler parallelism modest on machines with limited memory. Do not rebuild
the CLI while integration tests are running: they use it as a compiler launcher.
Update the affected English and French pages in `website/` with user-visible changes.
