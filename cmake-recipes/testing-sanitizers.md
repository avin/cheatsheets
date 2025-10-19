# Тесты, санитайзеры и покрытия

## enable_testing и add_test

```cmake
include(CTest)

add_executable(unit_tests tests/unit/main.cpp tests/unit/foo_test.cpp)
target_link_libraries(unit_tests PRIVATE project_core gtest::gtest_main)

add_test(NAME unit COMMAND unit_tests)
```

- Запуск: `ctest --output-on-failure -C Debug`.

## GoogleTest

```cmake
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(unit_tests tests/test_main.cpp)
target_link_libraries(unit_tests PRIVATE project_core gtest_main)
gtest_discover_tests(unit_tests)
```

## Catch2

```cmake
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

add_executable(catch_tests tests/catch_main.cpp)
target_link_libraries(catch_tests PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(catch_tests)
```

## Sanitizers (Clang/GCC)

```cmake
option(ENABLE_SANITIZER_ADDRESS "Enable AddressSanitizer" OFF)
option(ENABLE_SANITIZER_UNDEFINED "Enable UndefinedBehaviorSanitizer" OFF)

function(enable_sanitizers target)
  if(ENABLE_SANITIZER_ADDRESS)
    target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(${target} PRIVATE -fsanitize=address)
  endif()
  if(ENABLE_SANITIZER_UNDEFINED)
    target_compile_options(${target} PRIVATE -fsanitize=undefined)
    target_link_options(${target} PRIVATE -fsanitize=undefined)
  endif()
endfunction()

enable_sanitizers(app)
enable_sanitizers(unit_tests)
```

Запуск: `cmake -DENABLE_SANITIZER_ADDRESS=ON -S . -B build`.

## Coverage (GCC/Clang)

```cmake
option(ENABLE_COVERAGE "Enable coverage" OFF)
if(ENABLE_COVERAGE)
  target_compile_options(app PRIVATE --coverage)
  target_link_options(app PRIVATE --coverage)
endif()
```

Генерация отчёта:

```bash
cmake -DENABLE_COVERAGE=ON -S . -B build
cmake --build build
ctest --test-dir build
gcovr -r . build --html --html-details -o coverage.html
```

## CTest конфигурация

- `ctest --label-exclude integration`.
- Для параллельных тестов: `ctest --parallel 4`.
- Параметры в `CTestCustom.cmake` (output truncation, timeouts).
