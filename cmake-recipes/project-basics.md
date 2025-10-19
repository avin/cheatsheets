# Базовая структура проекта

```cmake
cmake_minimum_required(VERSION 3.20)

project(
  HelloCMake
  VERSION 1.0.0
  LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json

add_executable(hello
  src/main.cpp
  src/greetings.cpp
)

target_include_directories(hello PRIVATE include)
```

- Всегда указывайте `cmake_minimum_required`. Для современных проектов используйте 3.20+.
- Разделите исходники/заголовки в `src` и `include`.

## Раскладка каталогов

```
project/
├── CMakeLists.txt
├── cmake/              # вспомогательные модули
├── include/
│   └── hello/greetings.hpp
├── src/
│   ├── main.cpp
│   └── greetings.cpp
└── tests/
    └── CMakeLists.txt
```

## add_subdirectory

- Создайте `CMakeLists.txt` в поддиректориях:

```cmake
# src/CMakeLists.txt
add_library(hello_core STATIC greetings.cpp)
target_include_directories(hello_core PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(hello main.cpp)
target_link_libraries(hello PRIVATE hello_core)
```

```cmake
# Корневой CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(HelloCMake VERSION 1.0 LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(tests)
```

## Сборка

```bash
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/release --target hello
```

- Генераторы:
  - Ninja (рекомендуется): `cmake -G Ninja -S . -B build`
  - Makefiles, Visual Studio, Xcode — через `-G`.

## Переменные и опции

```cmake
option(HELLO_ENABLE_FEATURE "Enable feature" OFF)

if(HELLO_ENABLE_FEATURE)
  target_compile_definitions(hello PRIVATE HELLO_FEATURE=1)
endif()
```

## Отладка конфигурации

- Просмотр списка переменных: `cmake -L build`.
- `cmake --trace-expand -S . -B build` для отображения всех вызовов.
