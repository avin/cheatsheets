# Установка и пакеты

## install(TARGETS)

```cmake
add_library(my_lib STATIC src/lib.cpp)
target_include_directories(my_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

install(
  TARGETS my_lib
  EXPORT MyLibTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)
```

- Для заголовочных-only библиотек используйте `INTERFACE`.

## export и Config.cmake

```cmake
install(EXPORT MyLibTargets
  FILE MyLibTargets.cmake
  NAMESPACE MyLib::
  DESTINATION lib/cmake/MyLib
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/MyLibConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  cmake/MyLibConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/MyLibConfig.cmake"
  INSTALL_DESTINATION lib/cmake/MyLib
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/MyLibConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/MyLibConfigVersion.cmake"
  DESTINATION lib/cmake/MyLib
)
```

`MyLibConfig.cmake.in`:

```cmake
@PACKAGE_INIT@
include("${CMAKE_CURRENT_LIST_DIR}/MyLibTargets.cmake")
```

## cpack

```cmake
include(CPack)

set(CPACK_GENERATOR "TGZ;ZIP")
set(CPACK_PACKAGE_NAME "MyLib")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "dev@example.com")
```

Создание пакетов:

```bash
cmake --build build --target package
```

- Поддерживаемые генераторы: `TGZ`, `ZIP`, `DEB`, `RPM`, `NSIS` и т.д.

## install scripts и hooks

- `install(SCRIPT post_install.cmake)` — выполнить скрипт.
- `install(CODE "message(\"Installing...\")")` — встроенный код.

## Локальная установка

```bash
cmake --build build --target install
```

- Каталог установки: `CMAKE_INSTALL_PREFIX` (по умолчанию `/usr/local` на Unix).
- Для user-local: `cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local`.
