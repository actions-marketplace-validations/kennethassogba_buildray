# Buildray

Continuous build optimization for C++.

Buildray analyzes compiler traces, include graphs, and CMake dependencies to explain why a project builds slowly and how to improve it.

```bash
buildray analyze --build-dir build
```

```text
Buildray found 12 build-time issues

1. include/core/types.hpp
   Cumulative frontend time: 94.2 s
   Included by: 241 translation units

   Suggestions:
   - replace 4 includes with forward declarations
   - move 3 implementations to a source file

2. Target dependency cycle
   core -> logging -> utilities -> core

3. Overexposed CMake dependency
   Eigen is PUBLIC but only used by private sources
```

## Initial scope

* Linux
* Clang
* CMake and Ninja
* Clang `-ftime-trace`
* `compile_commands.json`
* terminal and JSON reports
