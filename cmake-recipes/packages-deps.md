# Пакеты и зависимости

## find_package

```cmake
find_package(fmt 10 REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE fmt::fmt OpenSSL::SSL)
```

- `find_package(fmt CONFIG REQUIRED)` ищет `fmtConfig.cmake`.
- `find_package(fmt REQUIRED COMPONENTS fmt-header-only)`.

## pkg-config

```cmake
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

target_link_libraries(app PRIVATE ${GTK_LIBRARIES})
target_include_directories(app PRIVATE ${GTK_INCLUDE_DIRS})
target_compile_options(app PRIVATE ${GTK_CFLAGS_OTHER})
```

## FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

target_link_libraries(app PRIVATE spdlog::spdlog)
```

- Для крупных проектов создайте `cmake/deps.cmake` и подключайте оттуда.

## ExternalProject

```cmake
include(ExternalProject)

ExternalProject_Add(
  curl_external
  GIT_REPOSITORY https://github.com/curl/curl.git
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ext
  BUILD_COMMAND cmake --build . --target install
)

add_dependencies(app curl_external)
target_include_directories(app PRIVATE ${CMAKE_BINARY_DIR}/ext/include)
target_link_directories(app PRIVATE ${CMAKE_BINARY_DIR}/ext/lib)
```

- ExternalProject подходит для тяжелых библиотек; вызывает отдельный CMake.

## Пользовательские find-модули

1. Создайте `cmake/FindFoo.cmake`.
2. Добавьте `set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})`.
3. Внутри модуля определите `Foo_FOUND`, `Foo_INCLUDE_DIR`, `Foo_LIBRARIES`.

## Версии и REQUIRED

- Проверка версии:

```cmake
find_package(fmt REQUIRED)
if(NOT fmt_VERSION VERSION_GREATER_EQUAL "10.0")
  message(FATAL_ERROR "fmt >= 10.0 required")
endif()
```

## Системные переменные

- `CMAKE_PREFIX_PATH`, `CMAKE_MODULE_PATH`, `CMAKE_FIND_ROOT_PATH` влияют на поиск.
- `CMAKE_FIND_PACKAGE_PREFER_CONFIG ON` — отдаёт предпочтение Config-модулям.
