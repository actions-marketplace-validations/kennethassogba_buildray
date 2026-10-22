# Buildray

Measure C++ build times and remove unnecessary includes and library dependencies.
Buildray tests each patch in a copy of your project and leaves your source tree untouched.

**[Read the guide · English / Français](https://kennethassogba.github.io/buildray/)**

## In GitHub Actions

[GitHub Marketplace](https://github.com/marketplace/actions/buildray-build-time)

Add `.github/workflows/build-time.yml`:

```yaml
name: Build time
on: [pull_request]
permissions:
  contents: read
jobs:
  buildray:
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@v6
        with:
          fetch-depth: 0
      - uses: kennethassogba/buildray@v0.13.0
        with:
          cmake-args: '["-DBUILD_TESTING=ON"]'
```

This compares the PR with its base revision. The Action installs its tools, runs
builds and tests, and saves the timings in the job summary and artifacts.
A slowdown fails the check only when it exceeds the budget and measured noise.

To try a cleanup instead, add `command: clean` for includes or `command: clean-deps`
for dependencies. Cleanup saves a patch for review; it does not apply it or open a PR.
See [CI setup](https://kennethassogba.github.io/buildray/ci.html) for project options.

## On your machine

```bash
buildray measure .       # Measure the build
buildray clean .         # Remove unused includes
buildray clean-deps .    # Remove unused private library dependencies
```

The directory defaults to the current one. Clang and clang-tidy are detected
automatically; dependency cleanup also selects LLD for both compared builds.
Pass your project's CMake options after `--`:

```bash
buildray clean-deps . --rebuild-file src/model.cc --exclude third_party \
  -- -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
```

Buildray saves reports, logs and patches under `.buildray/`. Builds and CTest must
pass before a patch is validated. Validation covers the supplied configuration;
a valid patch is only recommended as a speedup when repeated measurements support it.
Review it before applying, and test other configurations your project supports.

`buildray --help` shows the basics. `buildray --help-all` lists profiling, header
cleanup, revision comparisons and other options. The existing `optimize`,
`prune-deps` and `--source` forms still work. PCH experiments remain opt-in.

## Build

Buildray supports Linux, Clang, CMake and Ninja. Install Clang/LLVM 21, their
development libraries (including `libclang-21-dev`), clang-tidy, clang-scan-deps,
LLD, CMake 3.30+, Ninja, GNU diff and Python 3.12+. LLVM packages are available
from [apt.llvm.org](https://apt.llvm.org/).

```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_CXX_COMPILER=clang++-21 \
  -DLLVM_DIR=/usr/lib/llvm-21/lib/cmake/llvm \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
cmake --install build --prefix "$HOME/.local"
```

Add `$HOME/.local/bin` to `PATH`. Keep the installed `buildray` and
`buildray-header-tool` together. Dependency cleanup needs CMake 4.2+ and Ninja 1.13+.
See [Contributing](CONTRIBUTING.md) for the code layout and checks.

## Accepted contributions

Testing Buildray on other projects has led to these merged fixes:

- [Highway #3502](https://github.com/google/highway/pull/3502): removed unused test-library dependencies to avoid unnecessary relinks.
- [Highway #3497](https://github.com/google/highway/pull/3497): fixed a standalone test that could not link.
- [SentencePiece #1350](https://github.com/google/sentencepiece/pull/1350): removed two unused includes. No build-time gain claimed.

[Measured results and limits](https://kennethassogba.github.io/buildray/results.html)
· [Technical references](docs/)
