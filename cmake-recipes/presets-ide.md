# Генераторы, пресеты и IDE

## CMake Presets

Создайте `CMakePresets.json` в корне проекта.

```json
{
  "version": 5,
  "configurePresets": [
    {
      "name": "default",
      "displayName": "Default Release",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    },
    {
      "name": "debug",
      "inherits": "default",
      "displayName": "Debug build",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "build-default",
      "configurePreset": "default",
      "targets": ["all"]
    }
  ],
  "testPresets": [
    {
      "name": "test",
      "configurePreset": "debug",
      "output": {
        "outputOnFailure": true
      }
    }
  ]
}
```

Запуск:

```bash
cmake --preset debug
cmake --build --preset build-default
ctest --preset test
```

## Toolchains и cross-compilation

Создайте `toolchains/arm-none-eabi.cmake`:

```cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_RANLIB arm-none-eabi-ranlib)
```

Использование:

```bash
cmake -S . -B build-arm -DCMAKE_TOOLCHAIN_FILE=toolchains/arm-none-eabi.cmake
```

## IDE интеграции

- CLion: открывает директорию, использует CMake напрямую.
- VS Code: расширение CMake Tools читает `CMakePresets.json`.
- Visual Studio: `cmake -G "Visual Studio 17 2022"`.
- Xcode: `cmake -G Xcode`.

## Ninja vs Makefiles

- Ninja быстрее и лучше поддерживает параллелизм.
- Используйте `cmake -G Ninja` или задайте в пресете.

## Cache настроек

- Переменные CMake можно изменять: `cmake -S . -B build -DVAR=VALUE`.
- `cmake -U VAR` удаляет переменную из кэша.

## Export compile_commands

- `set(CMAKE_EXPORT_COMPILE_COMMANDS ON)`.
- Симлинк для clangd:

```bash
ln -s build/compile_commands.json compile_commands.json
```
