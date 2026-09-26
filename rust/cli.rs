use clap::{ArgAction, Parser, Subcommand};
use std::path::PathBuf;

// ---------------------------------------------------
// 📌 Cargo.toml: clap с генерацией справки и разбором типов
// ---------------------------------------------------
// clap = { version = "4", features = ["derive"] }

#[derive(Debug, Parser)]
#[command(version, about = "Небольшая CLI-утилита")]
struct Cli {
    /// Путь к конфигурации
    #[arg(short, long)]
    config: Option<PathBuf>,

    /// Увеличить подробность вывода: -v, -vv
    #[arg(short, long, action = ArgAction::Count)]
    verbose: u8,

    #[command(subcommand)]
    command: Option<Command>,
}

// ---------------------------------------------------
// 📌 Подкоманды с типизированными параметрами
// ---------------------------------------------------
#[derive(Debug, Subcommand)]
enum Command {
    /// Повторить текст
    Echo {
        text: String,
        #[arg(short, long, default_value_t = 1)]
        count: u8,
    },
    /// Сложить целые числа
    Sum { values: Vec<i64> },
}

// ---------------------------------------------------
// 📌 Логика отдельно от разбора argv
// ---------------------------------------------------
fn run(cli: Cli) {
    if cli.verbose > 0 {
        eprintln!("config={:?}", cli.config);
    }
    match cli.command {
        Some(Command::Echo { text, count }) => {
            for _ in 0..count {
                println!("{text}");
            }
        }
        Some(Command::Sum { values }) => println!("{}", values.iter().sum::<i64>()),
        None => println!("Используйте --help для списка команд"),
    }
}

fn main() {
    run(Cli::parse());
    // Примеры: cargo run -- echo hello --count 2; cargo run -- sum 10 20 30.
}
