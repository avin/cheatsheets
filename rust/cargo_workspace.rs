// ---------------------------------------------------
// 📌 Стандартная структура пакета
// ---------------------------------------------------
// cargo new shop --lib       # библиотека: src/lib.rs, публичный API и тесты
// cargo new shop-cli         # бинарник: src/main.rs
// Дополнительные бинарники: src/bin/import.rs и src/bin/export.rs.
// Интеграционные тесты: tests/api.rs; примеры: examples/basic.rs.
// cargo run --bin import

// ---------------------------------------------------
// 📌 Корневой Cargo.toml виртуального workspace
// ---------------------------------------------------
// [workspace]
// members = ["crates/core", "crates/cli"]
// resolver = "3"                # Для workspace без корневого package укажите resolver.
//
// [workspace.package]
// edition = "2024"
//
// [workspace.dependencies]
// serde = { version = "1", features = ["derive"] }
//
// [profile.release]
// lto = "thin"
//
// В crates/core/Cargo.toml:
// [package]
// name = "core"
// version = "0.1.0"
// edition.workspace = true
// [dependencies]
// serde.workspace = true

// ---------------------------------------------------
// 📌 Features и команды рабочего цикла
// ---------------------------------------------------
// [features]
// default = []
// json = ["dep:serde_json"]
// [dependencies]
// serde_json = { version = "1", optional = true }
//
// cargo check --workspace --all-targets
// cargo test --workspace
// cargo fmt --all -- --check
// cargo clippy --workspace --all-targets -- -D warnings
// cargo build -p core --release --features json
// Cargo.lock коммитят у приложений; для библиотек решение зависит от политики проекта.

fn main() {
    println!("Откройте комментарии с Cargo.toml и командами Cargo.");
}
