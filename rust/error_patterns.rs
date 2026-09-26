use anyhow::{Context, Result};
use std::path::Path;
use thiserror::Error;

// ---------------------------------------------------
// 📌 Cargo.toml: thiserror для публичного типа, anyhow для приложения
// ---------------------------------------------------
// thiserror = "2"
// anyhow = "1"

#[derive(Debug, Error)]
enum PortError {
    #[error("не удалось прочитать файл")]
    Io(#[from] std::io::Error),
    #[error("порт должен быть числом")]
    Parse(#[from] std::num::ParseIntError),
    #[error("порт 0 недопустим")]
    Zero,
}

// ---------------------------------------------------
// 📌 Типизированная ошибка в логике библиотеки
// ---------------------------------------------------
fn parse_port(text: &str) -> std::result::Result<u16, PortError> {
    let port: u16 = text.trim().parse()?;
    if port == 0 {
        return Err(PortError::Zero);
    }
    Ok(port)
}

fn read_port(path: &Path) -> std::result::Result<u16, PortError> {
    let text = std::fs::read_to_string(path)?;
    parse_port(&text)
}

// ---------------------------------------------------
// 📌 Контекст на границе приложения и цепочка причин
// ---------------------------------------------------
fn run(path: &Path) -> Result<()> {
    let port = read_port(path).with_context(|| format!("конфигурация {}", path.display()))?;
    println!("port={port}");
    Ok(())
}

fn main() {
    let Some(path) = std::env::args_os().nth(1) else {
        println!("Передайте путь к файлу с номером порта");
        return;
    };
    if let Err(error) = run(Path::new(&path)) {
        eprintln!("{error:#}"); // Показывает вложенные причины.
        std::process::exit(1);
    }
}
