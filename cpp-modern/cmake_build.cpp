/*
 * ============================================
 * CMAKE ПАТТЕРНЫ ДЛЯ СОВРЕМЕННОГО C++
 * ============================================
 * 
 * Примеры и best practices для CMake с C++20/23.
 * Весь код CMake представлен в комментариях.
 * 
 * Требования: CMake 3.20+
 * Компиляция: Этот файл не компилируется - только справочник
 */

#include <iostream>

// ============================================
// 📌 CMAKE BASICS - МИНИМАЛЬНАЯ НАСТРОЙКА
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Файл: CMakeLists.txt (минимальный проект)                      │
└─────────────────────────────────────────────────────────────────┘

cmake_minimum_required(VERSION 3.20)

# Название проекта и языки
project(MyProject 
    VERSION 1.0.0
    LANGUAGES CXX
)

# Стандарт C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Не использовать GNU расширения

# Исполняемый файл
add_executable(myapp 
    src/main.cpp
    src/utils.cpp
)

# Библиотека
add_library(mylib 
    src/mylib.cpp
    include/mylib.hpp
)

# Связывание
target_link_libraries(myapp PRIVATE mylib)

# Include директории
target_include_directories(mylib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
*/

// ============================================
// 📌 MODERN CMAKE - TARGET-BASED APPROACH
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ ПРАВИЛО: Используйте target_* вместо глобальных команд         │
└─────────────────────────────────────────────────────────────────┘

❌ ПЛОХО - Старый стиль (CMake 2.x):

include_directories(${PROJECT_SOURCE_DIR}/include)  # Глобально!
link_libraries(Boost::filesystem)  # Для всех целей!
add_definitions(-DDEBUG)  # Везде!

add_executable(app1 main.cpp)
add_executable(app2 other.cpp)
# Оба получат все флаги, даже если не нужны!


✅ ХОРОШО - Современный стиль (CMake 3.x):

add_executable(app1 main.cpp)
target_include_directories(app1 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(app1 PRIVATE Boost::filesystem)
target_compile_definitions(app1 PRIVATE DEBUG)

add_executable(app2 other.cpp)
# app2 не получает ненужных зависимостей


┌─────────────────────────────────────────────────────────────────┐
│ VISIBILITY: PRIVATE, PUBLIC, INTERFACE                          │
└─────────────────────────────────────────────────────────────────┘

add_library(mylib src/mylib.cpp)

# PRIVATE: только для этой цели
target_include_directories(mylib PRIVATE src/internal)

# PUBLIC: для этой цели И для тех, кто линкуется с ней
target_include_directories(mylib PUBLIC include)

# INTERFACE: только для тех, кто линкуется (не для самой цели)
target_compile_definitions(mylib INTERFACE USE_MYLIB)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE mylib)
# app получит include и USE_MYLIB от mylib
*/

// ============================================
// 📌 СТРУКТУРА ПРОЕКТА
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Рекомендуемая структура директорий                              │
└─────────────────────────────────────────────────────────────────┘

MyProject/
├── CMakeLists.txt              # Корневой CMake файл
├── CMakePresets.json           # Пресеты (опционально)
├── cmake/                      # CMake модули
│   ├── CompilerWarnings.cmake
│   ├── Sanitizers.cmake
│   └── Conan.cmake
├── src/                        # Исходники
│   ├── CMakeLists.txt
│   ├── main.cpp
│   └── mylib/
│       ├── CMakeLists.txt
│       ├── mylib.cpp
│       └── mylib.hpp
├── include/                    # Публичные заголовки
│   └── myproject/
│       └── mylib.hpp
├── tests/                      # Тесты
│   ├── CMakeLists.txt
│   └── test_mylib.cpp
├── docs/                       # Документация
├── external/                   # Внешние зависимости (git submodules)
└── build/                      # Директория сборки (не в git!)


┌─────────────────────────────────────────────────────────────────┐
│ Корневой CMakeLists.txt                                         │
└─────────────────────────────────────────────────────────────────┘

cmake_minimum_required(VERSION 3.20)

project(MyProject
    VERSION 1.2.3
    DESCRIPTION "My awesome C++ project"
    HOMEPAGE_URL "https://github.com/user/myproject"
    LANGUAGES CXX
)

# Опции проекта
option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(ENABLE_SANITIZERS "Enable sanitizers" OFF)

# Глобальные настройки
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Вывод в определенную директорию
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Подключение модулей
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Поддиректории
add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_DOCS)
    add_subdirectory(docs)
endif()
*/

// ============================================
// 📌 C++20/23 FEATURES SUPPORT
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Проверка поддержки C++20/23 фич                                 │
└─────────────────────────────────────────────────────────────────┘

# Проверка компилятора
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "11.0")
        message(FATAL_ERROR "GCC 11+ требуется для C++20")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14.0")
        message(FATAL_ERROR "Clang 14+ требуется для C++20")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.29")
        message(FATAL_ERROR "MSVC 19.29+ требуется для C++20")
    endif()
endif()

# Проверка поддержки конкретных фич
include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
    #include <concepts>
    template<std::integral T>
    T add(T a, T b) { return a + b; }
    int main() { return add(1, 2); }
" HAS_CONCEPTS)

if(NOT HAS_CONCEPTS)
    message(WARNING "Concepts не поддерживаются")
endif()

check_cxx_source_compiles("
    #include <ranges>
    int main() {
        std::vector<int> v{1, 2, 3};
        auto r = v | std::views::filter([](int i){ return i > 1; });
        return 0;
    }
" HAS_RANGES)


┌─────────────────────────────────────────────────────────────────┐
│ Модули C++20                                                    │
└─────────────────────────────────────────────────────────────────┘

# ЭКСПЕРИМЕНТАЛЬНО! Поддержка модулей еще развивается

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")

add_library(mymodule)
target_sources(mymodule
    PUBLIC
        FILE_SET CXX_MODULES FILES
            src/mymodule.cppm
)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE mymodule)


┌─────────────────────────────────────────────────────────────────┐
│ Корутины                                                        │
└─────────────────────────────────────────────────────────────────┘

# Для GCC/Clang может потребоваться флаг
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(myapp PRIVATE -fcoroutines)
endif()
*/

// ============================================
// 📌 DEPENDENCIES - СТОРОННИЕ БИБЛИОТЕКИ
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ 1. find_package() - Поиск установленных библиотек               │
└─────────────────────────────────────────────────────────────────┘

# Поиск библиотеки
find_package(Boost 1.75 REQUIRED COMPONENTS filesystem system)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

# Использование
add_executable(app main.cpp)
target_link_libraries(app PRIVATE 
    Boost::filesystem
    Boost::system
    fmt::fmt
    spdlog::spdlog
)


┌─────────────────────────────────────────────────────────────────┐
│ 2. FetchContent - Скачивание из интернета                       │
└─────────────────────────────────────────────────────────────────┘

include(FetchContent)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

# nlohmann/json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Использование
add_executable(tests test_main.cpp)
target_link_libraries(tests PRIVATE 
    GTest::gtest_main
    Catch2::Catch2WithMain
    nlohmann_json::nlohmann_json
)


┌─────────────────────────────────────────────────────────────────┐
│ 3. add_subdirectory() - Локальные зависимости                   │
└─────────────────────────────────────────────────────────────────┘

# Если библиотека в external/fmt
add_subdirectory(external/fmt)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE fmt::fmt)


┌─────────────────────────────────────────────────────────────────┐
│ 4. ExternalProject - Полный контроль над сборкой                │
└─────────────────────────────────────────────────────────────────┘

include(ExternalProject)

ExternalProject_Add(
    external_lib
    GIT_REPOSITORY https://github.com/example/lib.git
    GIT_TAG main
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)

# Сложнее в использовании, обычно FetchContent предпочтительнее
*/

// ============================================
// 📌 COMPILER WARNINGS & FLAGS
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Файл: cmake/CompilerWarnings.cmake                              │
└─────────────────────────────────────────────────────────────────┘

function(set_project_warnings target_name)
    set(MSVC_WARNINGS
        /W4     # Высокий уровень предупреждений
        /WX     # Предупреждения как ошибки
        /w14242 # 'identifier': conversion from 'type1' to 'type2'
        /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits'
        /w14263 # 'function': member function does not override
        /w14265 # 'classname': class has virtual functions, but destructor is not virtual
        /w14287 # 'operator': unsigned/negative constant mismatch
        /we4289 # nonstandard extension used: 'variable'
        /w14296 # 'operator': expression is always 'boolean_value'
        /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
        /w14545 # expression before comma evaluates to a function
        /w14546 # function call before comma missing argument list
        /w14547 # 'operator': operator before comma has no effect
        /w14549 # 'operator': operator before comma has no effect
        /w14555 # expression has no effect
        /w14619 # pragma warning: there is no warning number 'number'
        /w14640 # Enable warning on thread un-safe static member initialization
        /w14826 # Conversion from 'type1' to 'type2' is sign-extended
        /w14905 # wide string literal cast to 'LPSTR'
        /w14906 # string literal cast to 'LPWSTR'
        /w14928 # illegal copy-initialization
    )

    set(CLANG_WARNINGS
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wimplicit-fallthrough
    )

    set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
        -Wuseless-cast
    )

    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(PROJECT_WARNINGS ${MSVC_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        set(PROJECT_WARNINGS ${CLANG_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(PROJECT_WARNINGS ${GCC_WARNINGS})
    endif()

    target_compile_options(${target_name} INTERFACE ${PROJECT_WARNINGS})
endfunction()


# Использование в CMakeLists.txt:
include(cmake/CompilerWarnings.cmake)

add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE project_warnings)
*/

// ============================================
// 📌 SANITIZERS
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Файл: cmake/Sanitizers.cmake                                    │
└─────────────────────────────────────────────────────────────────┘

function(enable_sanitizers target_name)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
        option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
        option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
        option(ENABLE_MSAN "Enable MemorySanitizer" OFF)

        set(SANITIZERS "")

        if(ENABLE_ASAN)
            list(APPEND SANITIZERS "address")
        endif()

        if(ENABLE_UBSAN)
            list(APPEND SANITIZERS "undefined")
        endif()

        if(ENABLE_TSAN)
            if(ENABLE_ASAN OR ENABLE_MSAN)
                message(FATAL_ERROR "TSan не совместим с ASan/MSan")
            endif()
            list(APPEND SANITIZERS "thread")
        endif()

        if(ENABLE_MSAN)
            if(ENABLE_ASAN OR ENABLE_TSAN)
                message(FATAL_ERROR "MSan не совместим с ASan/TSan")
            endif()
            list(APPEND SANITIZERS "memory")
        endif()

        list(JOIN SANITIZERS "," SANITIZER_LIST)

        if(SANITIZER_LIST)
            target_compile_options(${target_name} INTERFACE
                -fsanitize=${SANITIZER_LIST}
                -fno-omit-frame-pointer
            )
            target_link_options(${target_name} INTERFACE
                -fsanitize=${SANITIZER_LIST}
            )
        endif()
    endif()
endfunction()


# Использование:
include(cmake/Sanitizers.cmake)

add_library(project_options INTERFACE)
enable_sanitizers(project_options)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE project_options)

# Запуск:
# cmake -B build -DENABLE_ASAN=ON
# cmake --build build
# ./build/myapp
*/

// ============================================
// 📌 TESTING WITH CTEST
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Корневой CMakeLists.txt                                         │
└─────────────────────────────────────────────────────────────────┘

cmake_minimum_required(VERSION 3.20)
project(MyProject)

option(BUILD_TESTING "Build tests" ON)

add_subdirectory(src)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()


┌─────────────────────────────────────────────────────────────────┐
│ tests/CMakeLists.txt                                            │
└─────────────────────────────────────────────────────────────────┘

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

# Тесты
add_executable(tests 
    test_main.cpp
    test_mylib.cpp
    test_utils.cpp
)

target_link_libraries(tests PRIVATE 
    mylib
    Catch2::Catch2WithMain
)

# Регистрация тестов
include(CTest)
include(Catch)
catch_discover_tests(tests)

# Или вручную:
add_test(NAME MyTests COMMAND tests)

# Запуск:
# cmake -B build
# cmake --build build
# ctest --test-dir build --output-on-failure
*/

// ============================================
// 📌 OPTIMIZATION FLAGS
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Debug vs Release                                                │
└─────────────────────────────────────────────────────────────────┘

# Автоматические флаги для разных конфигураций
# CMAKE_BUILD_TYPE может быть: Debug, Release, RelWithDebInfo, MinSizeRel

# Debug - без оптимизаций, с отладочной информацией
# Release - максимальные оптимизации
# RelWithDebInfo - оптимизации + отладочная информация
# MinSizeRel - оптимизация размера

# Установка по умолчанию
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Кастомные флаги для Debug
target_compile_options(myapp PRIVATE
    $<$<CONFIG:Debug>:-O0 -g3 -DDEBUG>
)

# Кастомные флаги для Release
target_compile_options(myapp PRIVATE
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)


┌─────────────────────────────────────────────────────────────────┐
│ LTO/IPO - Link Time Optimization                                │
└─────────────────────────────────────────────────────────────────┘

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

if(ipo_supported)
    message(STATUS "IPO/LTO поддерживается")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO/LTO не поддерживается: ${ipo_error}")
endif()

# Или для конкретной цели:
set_property(TARGET myapp PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)


┌─────────────────────────────────────────────────────────────────┐
│ Архитектурно-специфичные оптимизации                            │
└─────────────────────────────────────────────────────────────────┘

# Оптимизация под текущий CPU (не портабельно!)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(myapp PRIVATE -march=native -mtune=native)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(myapp PRIVATE /arch:AVX2)
endif()

# Векторизация
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(myapp PRIVATE
        -ftree-vectorize
        -fopt-info-vec-optimized  # Отчет о векторизации
    )
endif()


┌─────────────────────────────────────────────────────────────────┐
│ Профилирование                                                  │
└─────────────────────────────────────────────────────────────────┘

option(ENABLE_PROFILING "Enable profiling" OFF)

if(ENABLE_PROFILING)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(myapp PRIVATE -pg)
        target_link_options(myapp PRIVATE -pg)
    endif()
endif()

# Использование:
# cmake -B build -DENABLE_PROFILING=ON
# cmake --build build
# ./build/myapp
# gprof build/myapp gmon.out > analysis.txt
*/

// ============================================
// 📌 CMAKE PRESETS (CMakePresets.json)
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Файл: CMakePresets.json (в корне проекта)                       │
└─────────────────────────────────────────────────────────────────┘

{
    "version": 6,
    "cmakeMinimumRequired": {
        "major": 3,
        "minor": 20,
        "patch": 0
    },
    "configurePresets": [
        {
            "name": "base",
            "hidden": true,
            "generator": "Ninja",
            "binaryDir": "${sourceDir}/build/${presetName}",
            "cacheVariables": {
                "CMAKE_CXX_STANDARD": "23",
                "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
            }
        },
        {
            "name": "debug",
            "inherits": "base",
            "displayName": "Debug",
            "description": "Debug build",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Debug"
            }
        },
        {
            "name": "release",
            "inherits": "base",
            "displayName": "Release",
            "description": "Release build with optimizations",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Release",
                "CMAKE_INTERPROCEDURAL_OPTIMIZATION": "ON"
            }
        },
        {
            "name": "asan",
            "inherits": "debug",
            "displayName": "AddressSanitizer",
            "cacheVariables": {
                "ENABLE_ASAN": "ON"
            }
        },
        {
            "name": "windows-msvc",
            "inherits": "base",
            "displayName": "Windows MSVC",
            "condition": {
                "type": "equals",
                "lhs": "${hostSystemName}",
                "rhs": "Windows"
            },
            "generator": "Visual Studio 17 2022",
            "architecture": "x64"
        }
    ],
    "buildPresets": [
        {
            "name": "debug",
            "configurePreset": "debug",
            "configuration": "Debug"
        },
        {
            "name": "release",
            "configurePreset": "release",
            "configuration": "Release"
        }
    ],
    "testPresets": [
        {
            "name": "debug",
            "configurePreset": "debug",
            "output": {"outputOnFailure": true}
        },
        {
            "name": "release",
            "configurePreset": "release",
            "output": {"outputOnFailure": true}
        }
    ],
    "workflowPresets": [
        {
            "name": "debug",
            "steps": [
                {"type": "configure", "name": "debug"},
                {"type": "build", "name": "debug"},
                {"type": "test", "name": "debug"}
            ]
        }
    ]
}


# Использование пресетов:
cmake --list-presets                    # Список доступных пресетов
cmake --preset debug                    # Конфигурация
cmake --build --preset debug            # Сборка
ctest --preset debug                    # Тестирование
cmake --workflow --preset debug         # Весь workflow
*/

// ============================================
// 📌 INSTALLATION & PACKAGING
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Установка проекта                                               │
└─────────────────────────────────────────────────────────────────┘

# Установка исполняемых файлов
install(TARGETS myapp
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Установка библиотек с export
install(TARGETS mylib
    EXPORT MyLibTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

# Установка заголовков
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

# Export для использования другими проектами
install(EXPORT MyLibTargets
    FILE MyLibTargets.cmake
    NAMESPACE MyLib::
    DESTINATION lib/cmake/MyLib
)

# Config файл для find_package
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/MyLibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MyLibConfig.cmake
    INSTALL_DESTINATION lib/cmake/MyLib
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MyLibConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MyLibConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MyLibConfigVersion.cmake
    DESTINATION lib/cmake/MyLib
)


┌─────────────────────────────────────────────────────────────────┐
│ CPack - создание пакетов                                        │
└─────────────────────────────────────────────────────────────────┘

include(CPack)

set(CPACK_PACKAGE_NAME "MyProject")
set(CPACK_PACKAGE_VENDOR "MyCompany")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "My awesome project")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

# Генераторы пакетов
set(CPACK_GENERATOR "TGZ;ZIP")

if(WIN32)
    list(APPEND CPACK_GENERATOR "NSIS")
elseif(UNIX AND NOT APPLE)
    list(APPEND CPACK_GENERATOR "DEB;RPM")
elseif(APPLE)
    list(APPEND CPACK_GENERATOR "DragNDrop")
endif()

# Создание пакета:
# cmake --build build --target package
# или
# cd build && cpack
*/

// ============================================
// 📌 USEFUL CMAKE TRICKS
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Вывод всех переменных CMake (отладка)                           │
└─────────────────────────────────────────────────────────────────┘

get_cmake_property(_variableNames VARIABLES)
foreach(_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()


┌─────────────────────────────────────────────────────────────────┐
│ compile_commands.json для clangd/LSP                             │
└─────────────────────────────────────────────────────────────────┘

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Создаст build/compile_commands.json
# Можно создать symlink в корне проекта:
# ln -s build/compile_commands.json .


┌─────────────────────────────────────────────────────────────────┐
│ Ccache для ускорения пересборки                                 │
└─────────────────────────────────────────────────────────────────┘

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()


┌─────────────────────────────────────────────────────────────────┐
│ Принудительная цветная диагностика                              │
└─────────────────────────────────────────────────────────────────┘

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fdiagnostics-color=always)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/diagnostics:caret)
endif()


┌─────────────────────────────────────────────────────────────────┐
│ Проверка на out-of-source build                                 │
└─────────────────────────────────────────────────────────────────┘

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR 
        "In-source builds не поддерживаются. "
        "Создайте отдельную директорию для сборки:\n"
        "  mkdir build && cd build && cmake .."
    )
endif()


┌─────────────────────────────────────────────────────────────────┐
│ Автоматическое форматирование с clang-format                    │
└─────────────────────────────────────────────────────────────────┘

find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES 
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.hpp
        ${PROJECT_SOURCE_DIR}/include/*.hpp
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Formatting source files with clang-format"
    )
endif()

# Запуск: cmake --build build --target format
*/

// ============================================
// 📌 ПРИМЕР ПОЛНОГО ПРОЕКТА
// ============================================

/*
┌─────────────────────────────────────────────────────────────────┐
│ Корневой CMakeLists.txt - полный пример                         │
└─────────────────────────────────────────────────────────────────┘

cmake_minimum_required(VERSION 3.20)

# Проверка на in-source build
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds не поддерживаются")
endif()

project(ModernCppProject
    VERSION 1.0.0
    DESCRIPTION "Modern C++ Project Template"
    LANGUAGES CXX
)

# ========== Опции проекта ==========
option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(ENABLE_SANITIZERS "Enable sanitizers" OFF)
option(ENABLE_LTO "Enable Link-Time Optimization" ON)

# ========== Глобальные настройки ==========
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Вывод в bin/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# compile_commands.json для LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Модули
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# ========== Зависимости ==========
include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)

# ========== Warnings & Options ==========
add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

if(ENABLE_SANITIZERS)
    include(cmake/Sanitizers.cmake)
    enable_sanitizers(project_options)
endif()

# LTO
if(ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported)
    if(ipo_supported)
        set_property(TARGET project_options PROPERTY 
            INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

# Ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# ========== Поддиректории ==========
add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ========== Установка ==========
include(GNUInstallDirs)

install(TARGETS myapp
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ========== CPack ==========
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)
*/

int main() {
    std::cout << "=== CMake Patterns для C++20/23 ===\n\n";
    
    std::cout << "Этот файл содержит примеры CMake конфигураций\n";
    std::cout << "в комментариях. Основные темы:\n\n";
    
    std::cout << "1. Modern CMake - target-based подход\n";
    std::cout << "2. Структура проекта и организация файлов\n";
    std::cout << "3. C++20/23 поддержка (модули, концепты)\n";
    std::cout << "4. Управление зависимостями\n";
    std::cout << "5. Compiler warnings и sanitizers\n";
    std::cout << "6. Тестирование с CTest\n";
    std::cout << "7. Оптимизации и LTO\n";
    std::cout << "8. CMakePresets.json\n";
    std::cout << "9. Установка и создание пакетов\n";
    std::cout << "10. Полезные трюки и best practices\n";
    
    return 0;
}