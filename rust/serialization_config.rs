use serde::{Deserialize, Serialize};
use std::env;
use std::error::Error;
use std::path::{Path, PathBuf};

// ---------------------------------------------------
// 📌 Cargo.toml: типизированный JSON
// ---------------------------------------------------
// serde = { version = "1", features = ["derive"] }
// serde_json = "1"

fn default_port() -> u16 {
    8080
}

#[derive(Debug, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
struct Config {
    host: String,
    #[serde(default = "default_port")]
    port: u16,
    #[serde(default)]
    verbose: bool,
}

// ---------------------------------------------------
// 📌 Разбор и проверка конфигурации
// ---------------------------------------------------
fn parse_config(json: &str) -> Result<Config, Box<dyn Error>> {
    let mut config: Config = serde_json::from_str(json)?;
    if let Ok(value) = env::var("APP_PORT") {
        config.port = value.parse()?;
    }
    if config.port == 0 {
        return Err("port must be non-zero".into());
    }
    Ok(config)
}

fn load_config(path: &Path) -> Result<Config, Box<dyn Error>> {
    let contents = std::fs::read_to_string(path)?;
    parse_config(&contents)
}

// ---------------------------------------------------
// 📌 Сериализация и путь к конфигурации
// ---------------------------------------------------
fn main() -> Result<(), Box<dyn Error>> {
    let config = match env::args_os().nth(1) {
        Some(path) => load_config(&PathBuf::from(path))?,
        None => parse_config(r#"{"host":"127.0.0.1","verbose":true}"#)?,
    };
    println!("{config:?}");
    println!("{}", serde_json::to_string_pretty(&config)?);
    // Для секретов не выводите Config целиком: Debug может раскрыть пароль или токен.
    Ok(())
}
