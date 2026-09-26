use std::collections::{HashMap, HashSet};

// ---------------------------------------------------
// 📌 String, &str и UTF-8
// ---------------------------------------------------
fn string_examples() {
    let mut greeting = String::from("Привет");
    greeting.push_str(", Rust!");
    let borrowed: &str = &greeting;
    println!("{borrowed}");

    // Индексировать строку как text[0] нельзя: символ может занимать несколько байт.
    let first_char = borrowed.chars().next();
    let first_three: String = borrowed.chars().take(3).collect();
    println!("{first_char:?}, {first_three}");
    let parts: Vec<&str> = borrowed.split(',').map(str::trim).collect();
    println!("{parts:?}");
}

// ---------------------------------------------------
// 📌 Vec, срезы и безопасный доступ
// ---------------------------------------------------
fn vector_examples() {
    let mut values = vec![10, 20, 30];
    values.push(40);
    if let Some(value) = values.get(10) {
        println!("value={value}");
    } // get возвращает None вместо паники при выходе за границы

    for value in &mut values {
        *value *= 2;
    }
    let middle: &[i32] = &values[1..3];
    println!("{middle:?}");
}

// ---------------------------------------------------
// 📌 HashMap, HashSet и entry
// ---------------------------------------------------
fn collection_examples() {
    let words = ["rust", "go", "rust", "c"];
    let mut counts = HashMap::new();
    for word in words {
        *counts.entry(word).or_insert(0usize) += 1;
    }
    let unique: HashSet<_> = words.into_iter().collect();
    println!(
        "rust={}, unique={}",
        counts.get("rust").copied().unwrap_or(0),
        unique.len()
    );
    // HashMap/HashSet не гарантируют порядок обхода.
}

fn main() {
    string_examples();
    vector_examples();
    collection_examples();
}
