// ---------------------------------------------------
// 📌 dbg!, eprintln! и assert!
// ---------------------------------------------------
fn divide(a: i32, b: i32) -> Option<i32> {
    if b == 0 { None } else { Some(a / b) }
}

fn main() {
    let result = dbg!(divide(10, 2)); // Печатает выражение, значение, файл и строку в stderr.
    assert_eq!(result, Some(5));
    eprintln!("result={result:?}");
}

// ---------------------------------------------------
// 📌 Модуль с модульными тестами
// ---------------------------------------------------
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn divides_numbers() {
        assert_eq!(divide(10, 2), Some(5));
        assert_eq!(divide(10, 0), None);
    }

    #[test]
    #[should_panic(expected = "invalid index")]
    fn reports_invalid_index() {
        panic!("invalid index");
    }
}

// ---------------------------------------------------
// 📌 Команды разработки
// ---------------------------------------------------
// Файл отдельно: rustc --edition=2024 --test debug_testing.rs -o debug_testing_tests
// Затем запустите полученный исполняемый файл.
// Проект Cargo: cargo test; cargo fmt --check; cargo clippy --all-targets
