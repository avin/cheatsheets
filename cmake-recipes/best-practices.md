# Практики и диагностика

## Стиль и организация

- Один `project()` на корневой `CMakeLists.txt`.
- Используйте цели (`target_*`) вместо глобальных директив (`include_directories`, `link_directories`).
- Старайтесь избегать глобальных `set(CMAKE_CXX_FLAGS ...)`; используйте `target_compile_options`.
- Сохраняйте вспомогательные функции в `cmake/Modules/*.cmake`.

## Работа с кэшем

- Просмотр: `cmake -L build`.
- Очистка: удалите каталог сборки (`rm -rf build`) или используйте `cmake -S . -B build -U VAR`.
- Используйте `CACHE STRING "" FORCE` для обновления значения.

## message и диагностика

```cmake
message(STATUS "Configuring for ${CMAKE_BUILD_TYPE}")
message(VERBOSE "Detailed info")
message(WARNING "Deprecated option")
message(FATAL_ERROR "Missing dependency")
```

- `cmake --log-level=VERBOSE`.
- `cmake --trace-expand -S . -B build` — вывод всех вызовов (много).

## Проверки опций

- Отключайте флаги по умолчанию:

```cmake
if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_TESTING "Build tests" ON)
else()
  option(BUILD_TESTING "Build tests" OFF)
endif()
```

## Частые ошибки

- Использование `add_subdirectory` для внешних проектов → лучше FetchContent.
- Не устанавливайте `CMAKE_BUILD_TYPE` при multi-config генераторе (Visual Studio/Xcode). Используйте `--config`.
- Не смешивайте `find_package` без REQUIRED → всегда проверяйте `if(NOT Foo_FOUND)`.

## Переиспользование конфигурации

- В мультипроектной структуре используйте `CMAKE_PROJECT_TOP_LEVEL_INCLUDES`.
- Создавайте функции/макросы для повторяющихся шаблонов:

```cmake
function(enable_warnings target)
  target_compile_options(${target} PRIVATE -Wall -Wextra)
endfunction()
```

## Интеграция с clang-format/clang-tidy

```cmake
set(CMAKE_CXX_CLANG_TIDY clang-tidy;-checks=*)
set(CMAKE_CXX_CLANG_TIDY_EXPORT_FIXES ON)
```

## Генераторы платформ

- Для Windows: используйте `-A x64` с Visual Studio генераторами.
- Для macOS: выбирайте `-G Xcode` или `-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"`.

## Логи и профилирование сборки

- `cmake --build build --parallel`.
- `ninja -d stats` (Ninja показывает статистику).
- `cmake --build build --clean-first` перед релизной сборкой.
