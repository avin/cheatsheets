# Cargo и структура проекта

Cargo управляет сборкой, зависимостями и тестами Rust-пакета. Для одного приложения обычно достаточно одного пакета; workspace полезен, когда библиотека и несколько приложений разрабатываются вместе.

## Создание пакета и расположение файлов

```bash
cargo new shop-core --lib   # публичный API в src/lib.rs
cargo new shop-cli          # точка входа в src/main.rs
cargo run --bin shop-cli
```

В пакете можно разместить дополнительные бинарники в `src/bin/`, примеры в `examples/` и интеграционные тесты в `tests/`:

```text
shop-cli/
├── Cargo.toml
├── src/
│   ├── main.rs
│   └── bin/import.rs
├── examples/basic.rs
└── tests/cli.rs
```

- **Когда применять**: выносите повторно используемую логику в `src/lib.rs`, а запуск и разбор аргументов оставляйте в бинарнике.
- **Типичная ошибка**: запуск `cargo run` без `--bin`, когда в пакете несколько бинарников и Cargo не может выбрать нужный.

## Workspace с общими зависимостями

Корневой `Cargo.toml` виртуального workspace:

```toml
[workspace]
members = ["crates/core", "crates/cli"]
resolver = "3"

[workspace.package]
edition = "2024"

[workspace.dependencies]
serde = { version = "1", features = ["derive"] }

[profile.release]
lto = "thin"
```

`crates/core/Cargo.toml`:

```toml
[package]
name = "shop-core"
version = "0.1.0"
edition.workspace = true

[dependencies]
serde.workspace = true
```

`crates/cli/Cargo.toml`:

```toml
[package]
name = "shop-cli"
version = "0.1.0"
edition.workspace = true

[dependencies]
shop-core = { path = "../core" }
```

- **Плюсы**: общие `Cargo.lock` и каталог `target/`; версии зависимостей можно задавать в корне.
- **Риск**: features зависимостей объединяются между пакетами. Проверяйте нужные сочетания при сборке и тестировании.
- В виртуальном workspace задавайте `resolver` явно: у корневого манифеста нет собственного `package.edition`.

## Необязательные возможности пакета

В `Cargo.toml` пакета:

```toml
[features]
default = []
json = ["dep:serde_json"]

[dependencies]
serde_json = { version = "1", optional = true }
```

В Rust-коде возможность включают так:

```rust
#[cfg(feature = "json")]
fn encode(value: &str) -> String {
    serde_json::to_string(value).expect("&str serializes to JSON")
}
```

- **Когда применять**: когда функциональность и её зависимости действительно необязательны для части пользователей пакета.
- **Типичная ошибка**: забыть проверить сборку без default features и с нужной комбинацией features.

## Команды рабочего цикла

```bash
cargo check --workspace --all-targets
cargo test --workspace
cargo fmt --all -- --check
cargo clippy --workspace --all-targets -- -D warnings
cargo build -p shop-core --release --features json
cargo run -p shop-cli --bin shop-cli
```

`Cargo.lock` в корне workspace фиксирует разрешённые версии зависимостей для совместной сборки. Для приложения его обычно коммитят; для библиотеки учитывайте политику проекта и способ публикации.
