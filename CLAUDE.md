# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & test

The project is C++20, CMake-based, and uses vcpkg for dependencies (`vcpkg.json` manifest). Tracy submodule lives in `externals/tracy` (`git submodule update --init` if missing). Configurations are driven by `CMakePresets.json` (Ninja generator on Windows and Linux); set `VCPKG_ROOT` in the environment so the toolchain file resolves.

```powershell
# One-time
$env:VCPKG_ROOT = "<path-to-vcpkg>"
git submodule update --init   # only if externals/tracy is missing

# Configure / build / test (preset names: debug, release, relwithdebinfo-profiling)
cmake --preset debug
cmake --build --preset debug
ctest --preset debug

# Build a single target
cmake --build --preset debug --target CityBuilder
cmake --build --preset debug --target Neko_TEST

# Run a single GoogleTest case
build/debug/Neko_TEST.exe --gtest_filter=Basic.OpenWindow
```

The `relwithdebinfo-profiling` preset enables `TRACY_ENABLE` and `TRACY_ON_DEMAND` — use it for profiling sessions. `THREAD_SANITIZE=ON` (adds `-fsanitize=thread` on non-MSVC) is not in any preset; pass it manually with `-DTHREAD_SANITIZE=ON` on top of a preset if needed. Build artifacts land under `build/<preset>/`.

The `Neko_COMMON` static library has `UNITY_BUILD ON` and aggressive optimization flags (`/arch:AVX2 /GR- /EHs-c- /GL` on MSVC; `-march=haswell -fno-rtti -fno-exceptions -flto -ffast-math` on others). RTTI and exceptions are disabled — do not introduce `dynamic_cast`, `typeid`, or `try/throw`.

`CityBuilder` accepts these CLI flags (parsed by argh in `main/CityBuilder.cpp`): `--vsync`, `--fpslimit N`, `--infinite-money`, `--quick-spawn`, `--init-spawn`.

## Architecture

The repo is split into two distinct layers:

1. **Neko engine** (`include/`, `src/`) — a small game engine compiled into the `Neko_COMMON` static library. Subsystems: `engine/` (System base, ECS EntityManager, MainEngine + render-thread loop, worker thread pool with task dependencies, transforms, input), `graphics/` (double-buffered SFML command queue, sprite/anim/tilemap/shape managers), `sound/` (OpenAL), `allocator/` (`LinearAllocator`, `ProxyAllocator`, `StandardAllocator<T>` adapter that plugs custom allocators into STL containers), `utilities/`.

2. **Extensions** (`extensions/include/<Name>/`, `extensions/src/<Name>/`) — each subdirectory in `extensions/src/*` is **automatically discovered by CMake** and compiled into its own static library named after the directory (see the loop in `CMakeLists.txt:148-162`). To add a new game/extension, drop a folder into both `extensions/include/Foo` and `extensions/src/Foo` — no CMake edits needed. The `City` extension is the city-builder game; its entrypoint is `extensions/include/City/city_engine.h::CityBuilderEngine` (subclasses `MainEngine`).

3. **Executables** (`main/`) — only `CityBuilder.cpp` is wired up by CMake right now. `Client.cpp`, `Server.cpp`, `DebugServer.cpp`, `Platformer.cpp` are unused leftovers and are not in the build.

### Threading model

`MainEngine::EngineLoop` runs the render thread on a dedicated thread, synchronized with the engine thread via `condSyncRender` / `renderStartMutex`. Graphics submission is double-buffered (`commands_[2]`, `views_[2]`, indexed by `frameIndex`). **Never call SFML draw APIs directly from the engine thread** — push commands through `GraphicsManager::Draw` so they execute on the render thread.

`WorkerManager` (`include/engine/worker_system.h`) is the engine-side task system: it owns a main queue plus named queues (e.g. `"other"`), each backed by `WorkerThread`s. `Task` supports dependencies via `AddDependency(weak_ptr<Task>)`. `CityBuilderEngine::Update` is the canonical example of fan-out: car update + people update + behavior-tree shards are dispatched as tasks with explicit deps.

### Memory

Per-frame transient allocations should use `MainEngine::GetFrameAllocator()` — a `ProxyAllocator` over a `LinearAllocator` (default 50 MB, set via `Configuration::frameTmpDataSize`). The linear allocator is reset every frame. For STL containers, wrap with `StandardAllocator<T>{GetFrameAllocator()}`. The `PathFindingManager` owns its own dedicated `LinearAllocator` (~2 MB) for pathfinding scratch state.

### City extension structure

- `CityBuilderMap` — tile grid + Perlin-noise environment + road/rail sets + `TileMapGraph`.
- `TileMapGraph` / `PathFindingManager` — A* pathfinding scheduled as deferred jobs (`SchedulePathFinding` returns a `PathId`; poll with `IsPathDone`/`GetPath`).
- `CityCommandManager` — game actions go through commands (`CityCommand` subclasses) double-buffered between the engine and render threads. Use `AddCommand(cmd, fromRenderThread=true)` from ImGui/editor code.
- `CityCarManager`, `CityPeopleManager`, `CityZoneManager`, `CityBuildingManager` — ECS-style managers; entity components are tagged with the bitmask in `CityComponentType` (TRANSFORM=1, CAR=2, BUILDING=4, BEHAVIOR_TREE=8, PERSON=16).
- `BehaviorTreeManager` + `behaviortree.h` — JSON-driven behavior trees with composite/decorator/leaf node types. Custom leaf logic is registered via `FunctionMap::SetFunction(name, func)` and referenced from JSON by name.

### ECS conventions

`Entity` is just a `uint32_t` index; `EntityManager` stores an `EntityMask` per entity. Components are plain structs with an `Entity entity` field, stored contiguously in per-system `std::vector`s (one component vector per manager). Systems iterate by calling `entityManager.FilterEntities(mask)` to get the list of matching entities.

## Conventions

- All engine code is in `namespace neko`. The C++ standard is 20.
- Headers in `include/` use `#pragma once` and the project copyright header. Match the style.
- File naming is lower_snake_case for both headers and sources; classes are PascalCase, methods PascalCase, members trailing-underscore (`mainView_`, `cityBuilderMap_`).
- When adding a new System, derive from `neko::System` and implement `Init/Update/Destroy`. Register it as a member of `CityBuilderEngine` (or whichever extension's engine class) and call its lifecycle methods from there.
- Tracy zones are gated behind `#ifdef TRACY_ENABLE` — keep the same gating when adding new instrumentation.
