// ---------------------------------------------------
// 📌 Зависимость через трейт: тест без реального внешнего сервиса
// ---------------------------------------------------
trait RateSource {
    fn rate_percent(&self) -> Result<u32, &'static str>;
}

fn total_with_tax(source: &impl RateSource, price: u32) -> Result<u32, &'static str> {
    let rate = source.rate_percent()?;
    let tax = price.checked_mul(rate).ok_or("переполнение")? / 100;
    price.checked_add(tax).ok_or("переполнение")
}

struct FixedRate(u32);

impl RateSource for FixedRate {
    fn rate_percent(&self) -> Result<u32, &'static str> {
        Ok(self.0)
    }
}

// ---------------------------------------------------
// 📌 Unit-тесты проверяют успех и ошибку
// ---------------------------------------------------
#[cfg(test)]
mod tests {
    use super::*;

    struct FailingRate;
    impl RateSource for FailingRate {
        fn rate_percent(&self) -> Result<u32, &'static str> {
            Err("источник недоступен")
        }
    }

    #[test]
    fn calculates_total() {
        assert_eq!(total_with_tax(&FixedRate(20), 100), Ok(120));
    }

    #[test]
    fn propagates_error() {
        assert_eq!(
            total_with_tax(&FailingRate, 100),
            Err("источник недоступен")
        );
    }
}

// ---------------------------------------------------
// 📌 Интеграционные тесты — отдельные файлы tests/*.rs
// ---------------------------------------------------
// В настоящем пакете вынесите total_with_tax, RateSource и FixedRate в src/lib.rs как pub.
// tests/public_api.rs:
// use my_crate::{total_with_tax, FixedRate};
// #[test]
// fn public_api_works() {
//     assert_eq!(total_with_tax(&FixedRate(20), 100), Ok(120));
// }
// cargo test public_api_works; cargo test -- --nocapture

fn main() {
    println!("{:?}", total_with_tax(&FixedRate(20), 100));
}
