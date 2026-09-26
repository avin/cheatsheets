use std::fmt::Display;

// ---------------------------------------------------
// 📌 Переменные, константы и базовые типы
// ---------------------------------------------------
fn basics_variables() {
    let counter: i32 = 42; // let без mut создаёт неизменяемую переменную
    let mut attempts = 0;
    attempts += 1;
    const MAX_RETRIES: u8 = 3;
    let enabled = true;
    let initial = 'Я'; // char — символ Unicode, не один байт
    let pair: (i32, &str) = (counter, "ответ");
    let [first, second, third] = [10, 20, 30];

    println!("{pair:?}, {attempts}/{MAX_RETRIES}, {enabled}, {initial}");
    println!("{} {} {}", first, second, third);
}

// ---------------------------------------------------
// 📌 Функции и выражения
// ---------------------------------------------------
fn add(a: i32, b: i32) -> i32 {
    a + b // последнее выражение без ; становится возвращаемым значением
}

fn clamp(value: i32, min: i32, max: i32) -> i32 {
    value.max(min).min(max)
}

// ---------------------------------------------------
// 📌 Замыкания и обобщённые функции
// ---------------------------------------------------
fn print_twice<T: Display>(value: T) {
    println!("{value} {value}");
}

fn main() {
    basics_variables();
    let result = add(10, 32);
    let doubled = |n: i32| n * 2;
    print_twice(doubled(result));
    println!("clamp={}", clamp(120, 0, 100));
}
