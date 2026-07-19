# Naming Conventions

This document describes the naming conventions used throughout the ResInsight codebase.

## C++ Class Prefixes

ResInsight uses a set of domain-specific prefixes to categorize classes by their role in the architecture.

| Prefix | Stands For | Purpose | Example |
|--------|-----------|---------|---------|
| `Rim` | **Ri**nsight **M**odel | PDM data model objects — persistable, UI-generating model classes | `RimWellLog`, `RimWellPath`, `RimValveTemplate` |
| `Rig` | **Ri**nsight **G**eometry/**D**ata | Non-PDM data/geometry containers, computation results | `RigWellLogData`, `RigGridManager` |
| `Ria` | **Ri**nsight **A**pplication | Application-level utilities, helpers, and entry points | `RiaMain`, `RiaMainTools`, `RiaGrpcGuiApplication` |
| `Riu` | **Ri**nsight **U**I | Qt UI widgets, dialogs, and rendering components | `RiuMainWindow`, `RiuTheme` |
| `Ric` | **Ri**nsight **C**ommand | Command pattern implementations (undo/redo) | `RicNewWellPathCommand` |

### General Rules

- All ResInsight-specific C++ classes use one of the `Ri*` prefixes above.
- The prefix is always followed by PascalCase for the remainder of the class name.
- Names should be descriptive and unambiguous within their category.

## PDM (Project Data Model) Conventions

### Object Names

- PDM objects use the `Rim` prefix and inherit from `caf::PdmObject`.
- The PDM class keyword (used in `CAF_PDM_SOURCE_INIT`) must be unique and descriptive:

```cpp
CAF_PDM_SOURCE_INIT(RimClassName, "UniqueClassKeyword");
```

### Field Names

- PDM field member variables use the `m_` prefix with **camelCase**:

```cpp
caf::PdmField<QDateTime> m_date;
caf::PdmField<double>     m_startMD;
```

- The script field name (second argument to `CAF_PDM_InitScriptableField`) should use **camelCase**:

```cpp
CAF_PDM_InitScriptableField(&m_startMD, "StartMd", 0.0, "Start MD");
```

- Avoid abbreviations in field names — prefer `"StartMd"` over `"StartMD"`.
- Python generators automatically convert camelCase script field names to snake_case (`start_md`).

## Python Conventions

- Use **snake_case** for all Python identifiers (variables, functions, methods).
- Use type hints where helpful.
- Format code with `ruff format`.
- Follow PEP 8.

## General Code Style

### C++

- **Language standard**: C++23
- **Formatting**: clang-format with the repository's `.clang-format` configuration (clang-format-19).
- **Header/Implementation split**: Function bodies go in `.cpp` files, not inline in headers. Exceptions: function templates and `constexpr` functions that require the definition to be visible.
- **Smart pointers**: Prefer `std::unique_ptr` and `std::shared_ptr` over raw ownership.
- **Type safety**: Use C++23 type system features, avoid implicit conversions.
- **Error handling**: Use `std::expected` where appropriate.

### File Naming

- C++ headers: `*.h`
- C++ sources: `*.cpp`
- File names typically match the class name (e.g., `RimWellLog.h` contains class `RimWellLog`).
- Organization mirrors the namespace/project structure within `ApplicationLibCode/`.
