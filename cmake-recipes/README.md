# CMake шпаргалка

Практические рецепты для настройки проектов на CMake: от базовых CMakeLists до тестирования, пакетов и интеграции с IDE/CI.

### 🧱 [Базовая структура проекта](./project-basics.md)

* 📌  **`cmake_minimum_required`, `project`, глобальные настройки**
* 📌  **Компиляция исполняемых файлов и библиотек**
* 📌  **Организация исходников, add_subdirectory**

***

### ⚙️ [Таргеты и свойства](./targets-properties.md)

* 📌  **`target_link_libraries`, `target_include_directories`**
* 📌  **Компиляционные опции, генераторы конфигураций**
* 📌  **Интерфейсные библиотеки и наследование свойств**

***

### 📦 [Пакеты и зависимости](./packages-deps.md)

* 📌  **`find_package`, `FetchContent`, ExternalProject**
* 📌  **Работа с pkg-config, custom Find-модули**
* 📌  **Управление версиями и конфигурациями**

***

### 🧪 [Тесты, санитайзеры и покрытия](./testing-sanitizers.md)

* 📌  **`enable_testing`, `add_test`, CTest**
* 📌  **Интеграция с GoogleTest, Catch2**
* 📌  **Address/UB sanitizers, Coverage**

***

### 🔄 [Генераторы, пресеты и IDE](./presets-ide.md)

* 📌  **CMake Presets (`CMakePresets.json`)**
* 📌  **Интеграция с CLion, VS Code, Ninja/Makefile**
* 📌  **Toolchains, cross-compilation**

***

### 🚀 [Установка и пакеты](./install-packaging.md)

* 📌  **`install(TARGETS/FILES)`, `export`**
* 📌  **Создание пакетов (`cpack`, `CPackConfig`)**
* 📌  **Генерация `Config.cmake` для библиотек**

***

### 🔐 [Практики и диагностика](./best-practices.md)

* 📌  **Стиль CMake, модульность, кэш**
* 📌  **Диагностика (`message`, `--trace-expand`)**
* 📌  **Частые ошибки и способы их избежать**
