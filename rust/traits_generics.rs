use std::collections::HashMap;
use std::fmt::Display;

// ---------------------------------------------------
// 📌 Ассоциированный тип описывает данные репозитория
// ---------------------------------------------------
trait Repository {
    type Item: Display;

    fn get(&self, key: &str) -> Option<&Self::Item>;
}

struct MemoryRepository {
    values: HashMap<String, String>,
}

impl Repository for MemoryRepository {
    type Item = String;

    fn get(&self, key: &str) -> Option<&Self::Item> {
        self.values.get(key)
    }
}

// ---------------------------------------------------
// 📌 Обобщённая функция и динамическая диспетчеризация
// ---------------------------------------------------
fn print_item<R: Repository + ?Sized>(repository: &R, key: &str) {
    if let Some(item) = repository.get(key) {
        println!("{key}: {item}");
    }
}

fn print_dynamic(repository: &dyn Repository<Item = String>, key: &str) {
    print_item(repository, key); // dyn удобен для набора реализаций за одним интерфейсом.
}

// ---------------------------------------------------
// 📌 Время жизни: результат ссылается на repository или fallback
// ---------------------------------------------------
fn value_or_default<'a>(
    repository: &'a dyn Repository<Item = String>,
    key: &str,
    fallback: &'a str,
) -> &'a str {
    repository.get(key).map(String::as_str).unwrap_or(fallback)
}

fn main() {
    let repository = MemoryRepository {
        values: HashMap::from([("lang".into(), "Rust".into())]),
    };
    print_item(&repository, "lang");
    print_dynamic(&repository, "lang");
    println!("{}", value_or_default(&repository, "missing", "unknown"));
    // Когда результат должен жить независимо от repository, возвращайте String.
}
