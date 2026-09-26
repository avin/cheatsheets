// ---------------------------------------------------
// 📌 Модули, импорт и видимость
// ---------------------------------------------------
mod math {
    pub fn clamp(value: i32, min: i32, max: i32) -> i32 {
        value.max(min).min(max)
    }

    pub(crate) const ANSWER: i32 = 42; // Доступно в пределах текущего crate.
}

use math::clamp;

// ---------------------------------------------------
// 📌 macro_rules! и встроенные макросы
// ---------------------------------------------------
macro_rules! max_of {
    ($first:expr, $second:expr) => {{
        let first = $first; // Вычисляем каждый аргумент ровно один раз.
        let second = $second;
        if first > second { first } else { second }
    }};
}

fn macro_examples() {
    let values = vec![4, 8, 15];
    let message = format!("count={}", values.len());
    println!("{message}, max={}", max_of!(10, 7));
}

// ---------------------------------------------------
// 📌 Условная компиляция
// ---------------------------------------------------
#[cfg(target_os = "windows")]
fn platform_name() -> &'static str {
    "Windows"
}

#[cfg(not(target_os = "windows"))]
fn platform_name() -> &'static str {
    "другая ОС"
}

fn main() {
    macro_examples();
    println!(
        "clamp={}, answer={}, platform={}",
        clamp(120, 0, 100),
        math::ANSWER,
        platform_name()
    );
    // В большом проекте модуль можно вынести в math.rs: `mod math;`.
}
