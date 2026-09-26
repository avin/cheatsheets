use std::borrow::Cow;

// ---------------------------------------------------
// 📌 Перемещение, Copy и явное клонирование
// ---------------------------------------------------
fn ownership_examples() {
    let original = String::from("hello");
    let moved = original; // String перемещён; original больше использовать нельзя
    let copied = 42;
    let another = copied; // i32 реализует Copy
    let cloned = moved.clone(); // выделяем память для независимой копии
    println!("{moved} {cloned} {copied} {another}");
}

// ---------------------------------------------------
// 📌 Неизменяемое и изменяемое заимствование
// ---------------------------------------------------
fn append_world(text: &mut String) {
    text.push_str(" world");
}

fn first_word(text: &str) -> &str {
    text.split_whitespace().next().unwrap_or("")
}

fn slice_examples() {
    let mut text = String::from("hello");
    append_world(&mut text);
    let first = first_word(&text); // &str не копирует содержимое
    println!("first={first}, full={text}");
    let numbers = [10, 20, 30, 40];
    let middle: &[i32] = &numbers[1..3];
    println!("{middle:?}");
}

// ---------------------------------------------------
// 📌 Время жизни ссылки и Cow
// ---------------------------------------------------
fn longest<'a>(left: &'a str, right: &'a str) -> &'a str {
    if left.len() >= right.len() {
        left
    } else {
        right
    }
}

fn normalize(input: &str) -> Cow<'_, str> {
    let trimmed = input.trim();
    if trimmed.bytes().any(|byte| byte.is_ascii_uppercase()) {
        Cow::Owned(trimmed.to_ascii_lowercase())
    } else {
        Cow::Borrowed(trimmed)
    }
}

fn main() {
    ownership_examples();
    slice_examples();
    println!("{}", longest("short", "longer"));
    println!("{}", normalize(" HELLO "));
}
