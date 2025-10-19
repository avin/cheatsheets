# Таргеты и свойства

## target_link_libraries

```cmake
add_library(math_utils STATIC math/vec.cpp math/matrix.cpp)
target_include_directories(math_utils PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(app src/main.cpp)
target_link_libraries(app
  PRIVATE
    math_utils
    fmt::fmt
)
```

- PUBLIC: свойства переходят к зависимым таргетам.
- INTERFACE: только для зависимых (сам таргет не использует).
- PRIVATE: только для данного таргета.

## target_include_directories

```cmake
target_include_directories(math_utils
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)
```

## Компиляционные опции

```cmake
target_compile_options(app
  PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
```

- Используйте генераторные выражения (`$<...>`).

## link options

```cmake
target_link_options(app PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-fuse-ld=lld>
)
```

## target_compile_definitions

```cmake
target_compile_definitions(app
  PRIVATE APP_VERSION="${PROJECT_VERSION}"
  PUBLIC APP_USE_FEATURE
)
```

## INTERFACE библиотеки

```cmake
add_library(core_headers INTERFACE)
target_include_directories(core_headers INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_library(core STATIC src/core.cpp)
target_link_libraries(core PUBLIC core_headers)
```

## Импортированные таргеты

```cmake
find_package(OpenSSL REQUIRED)
target_link_libraries(app PRIVATE OpenSSL::SSL OpenSSL::Crypto)
```

- Всегда используйте импортированные таргеты вместо ручных `include_directories`/`link_directories`.

## Генерация исходников

```cmake
add_custom_command(
  OUTPUT generated/config.hpp
  COMMAND python ${PROJECT_SOURCE_DIR}/scripts/gen_config.py > generated/config.hpp
  DEPENDS scripts/gen_config.py
)

add_custom_target(generate_config DEPENDS generated/config.hpp)
add_dependencies(app generate_config)
target_include_directories(app PRIVATE ${CMAKE_BINARY_DIR}/generated)
```
*** End Patch
