use std::fmt;
use std::fs;
use std::num::ParseIntError;

// ---------------------------------------------------
// 📌 Option: значение может отсутствовать
// ---------------------------------------------------
fn first_even(values: &[i32]) -> Option<i32> {
    values.iter().copied().find(|value| value % 2 == 0)
}

// ---------------------------------------------------
// 📌 Result и оператор ?: передача ошибки вызывающему коду
// ---------------------------------------------------
fn read_number(path: &str) -> Result<i32, Box<dyn std::error::Error>> {
    let contents = fs::read_to_string(path)?;
    let number = contents.trim().parse::<i32>()?;
    Ok(number)
}

// ---------------------------------------------------
// 📌 Собственный тип ошибки и From для оператора ?
// ---------------------------------------------------
#[derive(Debug)]
enum PortError {
    InvalidNumber(ParseIntError),
    OutOfRange(u16),
}

impl fmt::Display for PortError {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::InvalidNumber(error) => write!(formatter, "неверное число: {error}"),
            Self::OutOfRange(port) => write!(formatter, "недопустимый порт: {port}"),
        }
    }
}

impl std::error::Error for PortError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            Self::InvalidNumber(error) => Some(error),
            Self::OutOfRange(_) => None,
        }
    }
}

impl From<ParseIntError> for PortError {
    fn from(error: ParseIntError) -> Self {
        Self::InvalidNumber(error)
    }
}

fn parse_port(input: &str) -> Result<u16, PortError> {
    let port = input.parse::<u16>()?;
    if port == 0 {
        return Err(PortError::OutOfRange(port));
    }
    Ok(port)
}

fn main() {
    println!("first even={:?}", first_even(&[1, 3, 8]));
    match parse_port("8080") {
        Ok(port) => println!("port={port}"),
        Err(error) => eprintln!("{error}"),
    }
    // Для ошибок ввода-вывода сохраняйте контекст: путь и исходную ошибку.
    if let Err(error) = read_number("missing.txt") {
        eprintln!("missing.txt: {error}");
    }
    // unwrap/expect уместны, когда неуспех действительно означает ошибку программы.
}
