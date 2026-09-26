use std::collections::HashMap;
use std::num::ParseIntError;
use std::sync::{LazyLock, OnceLock};
use std::thread;

// Сравнения старых и новых идиом. «Раньше» означает более многословную запись,
// а не запрещённый синтаксис: оба варианта остаются корректными.
// Для let-цепочки нужен Rust 1.88+ и edition 2024; остальные версии указаны ниже.
// При поддержке старого компилятора сначала проверьте rust-version в Cargo.toml.

// ---------------------------------------------------
// 📌 Ранний выход: match -> let else (Rust 1.65+)
// ---------------------------------------------------
// Раньше:
fn print_result_old(result: Result<u32, &str>) {
    let value = match result {
        Ok(value) => value,
        Err(_) => {
            println!("получили ошибку");
            return;
        }
    };
    println!("value={value}");
}

// Сейчас:
fn print_result_new(result: Result<u32, &str>) {
    let Ok(value) = result else {
        println!("получили ошибку");
        return;
    };
    println!("value={value}");
}
// Когда применять: успешное значение нужно ниже, неуспех завершает текущую
// функцию или цикл (return, break, continue).
// Плюс: основной путь без лишнего уровня вложенности.
// Риск: если нужен текст Err(error) или обе ветки вычисляют значение, оставьте match.

// ---------------------------------------------------
// 📌 Проброс ошибки: match -> ?
// ---------------------------------------------------
// Раньше:
fn parse_number_old(text: &str) -> Result<u32, ParseIntError> {
    let number = match text.parse::<u32>() {
        Ok(number) => number,
        Err(error) => return Err(error),
    };
    Ok(number)
}

// Сейчас:
fn parse_number_new(text: &str) -> Result<u32, ParseIntError> {
    let number = text.parse::<u32>()?;
    Ok(number)
}
// Когда применять: функция возвращает совместимый Result или Option,
// а локальная обработка ошибки не нужна.
// Плюс: нет повторяющейся ветки Err(error) => return Err(error).
// Риск: ? передаёт ошибку вызывающему коду; если здесь нужны сообщение,
// контекст или восстановление, обработайте ошибку явно.

// ---------------------------------------------------
// 📌 Один интересующий вариант: match -> if let
// ---------------------------------------------------
// Раньше:
fn greet_old(maybe_name: Option<&str>) {
    match maybe_name {
        Some(name) => println!("Привет, {name}!"),
        None => (),
    }
}

// Сейчас:
fn greet_new(maybe_name: Option<&str>) {
    if let Some(name) = maybe_name {
        println!("Привет, {name}!");
    }
}
// Когда применять: остальные варианты намеренно игнорируются.
// Плюс: нет пустой ветки.
// Риск: match остаётся лучше, если при расширении enum нужно обработать
// каждый новый вариант: компилятор проверяет полноту match.

struct User {
    name: &'static str,
    active: bool,
}

// ---------------------------------------------------
// 📌 Связанные проверки: вложенные if -> let-цепочка (Rust 1.88+, edition 2024)
// ---------------------------------------------------
// Раньше:
fn active_name_old(users: &[User], id: usize) -> Option<&str> {
    if let Some(user) = users.get(id) {
        if user.active {
            return Some(user.name);
        }
    }
    None
}

// Сейчас:
fn active_name_new(users: &[User], id: usize) -> Option<&str> {
    if let Some(user) = users.get(id)
        && user.active
    {
        return Some(user.name);
    }
    None
}
// Когда применять: следующая проверка зависит от значения, полученного
// в предыдущей, и не должна выполняться при неудачном сопоставлении.
// Плюс: одна ветка вместо вложенных блоков.
// Риск: в edition 2021 этот синтаксис недоступен; вложенный if корректен.

// ---------------------------------------------------
// 📌 Предикат для Option: map + unwrap_or -> is_some_and (Rust 1.70+)
// ---------------------------------------------------
// Раньше:
fn valid_port_old(maybe_port: Option<u16>) -> bool {
    maybe_port.map(|port| port > 0).unwrap_or(false)
}

// Сейчас:
fn valid_port_new(maybe_port: Option<u16>) -> bool {
    maybe_port.is_some_and(|port| port > 0)
}
// Когда применять: None означает false, Some нужно проверить предикатом.
// Плюс: не нужно расшифровывать значение по умолчанию.
// Риск: метод потребляет Option. Если значение ещё понадобится, используйте
// maybe_port.as_ref().is_some_and(|port| *port > 0).

// ---------------------------------------------------
// 📌 Заимствование из Option<String>: map -> as_deref (Rust 1.40+)
// ---------------------------------------------------
// Раньше:
fn borrowed_name_old(maybe_name: &Option<String>) -> Option<&str> {
    maybe_name.as_ref().map(String::as_str)
}

// Сейчас:
fn borrowed_name_new(maybe_name: &Option<String>) -> Option<&str> {
    maybe_name.as_deref()
}
// Когда применять: нужен Option<&str> из Option<String> без перемещения строки.
// Плюс: преобразование выражено одним стандартным методом.
// Риск: ссылка не может жить дольше исходного Option<String>.

// ---------------------------------------------------
// 📌 Ожидаемый префикс: starts_with + срез -> strip_prefix (Rust 1.45+)
// ---------------------------------------------------
// Раньше:
fn bearer_old(line: &str) -> Option<&str> {
    if line.starts_with("Bearer ") {
        Some(&line["Bearer ".len()..])
    } else {
        None
    }
}

// Сейчас:
fn bearer_new(line: &str) -> Option<&str> {
    line.strip_prefix("Bearer ")
}
// Когда применять: отсутствие префикса нужно отличать от пустого остатка.
// Плюс: нет ручного вычисления границы среза.
// Риск: strip_prefix снимает префикс ровно один раз; trim_start_matches
// удаляет все начальные совпадения.

// ---------------------------------------------------
// 📌 Первый разделитель: splitn -> split_once (Rust 1.52+)
// ---------------------------------------------------
// Раньше:
fn key_value_old(line: &str) -> Option<(&str, &str)> {
    let mut parts = line.splitn(2, '=');
    match (parts.next(), parts.next()) {
        (Some(key), Some(value)) => Some((key, value)),
        _ => None,
    }
}

// Сейчас:
fn key_value_new(line: &str) -> Option<(&str, &str)> {
    line.split_once('=')
}
// Когда применять: нужны две части по первому разделителю.
// Плюс: сразу получаем Option<(&str, &str)>.
// Риск: для всех частей по-прежнему нужен split, для последнего — rsplit_once.

// ---------------------------------------------------
// 📌 Счётчик в HashMap: get_mut + insert -> entry
// ---------------------------------------------------
// Раньше:
fn count_old(words: &[&str]) -> HashMap<String, usize> {
    let mut counts = HashMap::new();
    for &word in words {
        if let Some(count) = counts.get_mut(word) {
            *count += 1;
        } else {
            counts.insert(word.to_owned(), 1);
        }
    }
    counts
}

// Сейчас:
fn count_new(words: &[&str]) -> HashMap<String, usize> {
    let mut counts = HashMap::new();
    for &word in words {
        *counts.entry(word.to_owned()).or_default() += 1;
    }
    counts
}
// Когда применять: значение нужно создать при отсутствии ключа и сразу изменить.
// Плюс: поиск и вставка собраны в одном месте. entry существует давно,
// это идиома, а не недавно добавленная возможность.
// Риск: or_default подходит лишь при подходящем значении Default.
// Дорогой начальный объект создавайте через or_insert_with(|| make_value()).

// ---------------------------------------------------
// 📌 Сбор Result: цикл -> collect
// ---------------------------------------------------
// Раньше:
fn parse_all_old(words: &[&str]) -> Result<Vec<u32>, ParseIntError> {
    let mut numbers = Vec::new();
    for word in words {
        numbers.push(word.parse::<u32>()?);
    }
    Ok(numbers)
}

// Сейчас:
fn parse_all_new(words: &[&str]) -> Result<Vec<u32>, ParseIntError> {
    words.iter().map(|word| word.parse::<u32>()).collect()
}
// Когда применять: преобразуем каждый элемент, первая ошибка останавливает обход.
// Плюс: тип Result<Vec<_>, _> явно показывает политику ошибок.
// Риск: filter_map(Result::ok) молча теряет ошибки. Оставьте цикл,
// если на каждой итерации нужна дополнительная логика.

// ---------------------------------------------------
// 📌 Вычисляемый массив: перечисление -> array::from_fn (Rust 1.63+)
// ---------------------------------------------------
// Раньше:
fn labels_old() -> [String; 3] {
    [
        format!("item-{}", 0),
        format!("item-{}", 1),
        format!("item-{}", 2),
    ]
}

// Сейчас:
fn labels_new() -> [String; 3] {
    std::array::from_fn(|i| format!("item-{i}"))
}
// Когда применять: размер известен, элементы вычисляются из индекса,
// в том числе для типов без Copy.
// Плюс: длина и правило построения не дублируются.
// Риск: короткий массив с разными значениями яснее задать литералом.

// ---------------------------------------------------
// 📌 Форматирование: явный аргумент -> захват имени (Rust 1.58+)
// ---------------------------------------------------
// Раньше:
fn message_old(user: &str) -> String {
    format!("Пользователь: {}", user)
}

// Сейчас:
fn message_new(user: &str) -> String {
    format!("Пользователь: {user}")
}
// Когда применять: подставляем переменную без дополнительного вычисления.
// Плюс: текст и имя значения видны рядом.
// Риск: для выражения нужны обычные {} и явный аргумент.

// ---------------------------------------------------
// 📌 Ленивая глобальная инициализация: OnceLock -> LazyLock (Rust 1.80+)
// ---------------------------------------------------
// Раньше:
static OLD_WORDS: OnceLock<Vec<String>> = OnceLock::new();

fn words_old() -> &'static Vec<String> {
    OLD_WORDS.get_or_init(|| vec!["rust".to_owned()])
}

// Сейчас:
static NEW_WORDS: LazyLock<Vec<String>> = LazyLock::new(|| vec!["rust".to_owned()]);
// Использование: NEW_WORDS.len().
// Когда применять: глобальное значение создаётся при первом обращении,
// инициализатор не зависит от аргументов вызывающего кода.
// Плюс: обращение похоже на обычное заимствование значения.
// Риск: OnceLock лучше, если значение нужно задать позже или передать параметры.

// ---------------------------------------------------
// 📌 Локальные данные в потоке: clone + spawn -> thread::scope (Rust 1.63+)
// ---------------------------------------------------
// Раньше:
fn sum_old(numbers: &[i32]) -> i32 {
    let copy = numbers.to_vec();
    let handle = thread::spawn(move || copy.iter().sum::<i32>());
    handle.join().expect("поток завершился с паникой")
}

// Сейчас:
fn sum_new(numbers: &[i32]) -> i32 {
    thread::scope(|scope| {
        let handle = scope.spawn(|| numbers.iter().sum::<i32>());
        handle.join().expect("поток завершился с паникой")
    })
}
// Когда применять: поток может заимствовать локальные данные и завершится
// до выхода из области scope.
// Плюс: не нужен clone только ради требования 'static.
// Риск: долгоживущему потоку по-прежнему нужны thread::spawn и владение данными.

fn main() {
    print_result_old(Ok(7));
    print_result_new(Ok(7));
    assert_eq!(
        parse_number_old("42").unwrap(),
        parse_number_new("42").unwrap()
    );
    greet_old(Some("Ada"));
    greet_new(Some("Ada"));

    let users = [User {
        name: "Ada",
        active: true,
    }];
    assert_eq!(active_name_old(&users, 0), active_name_new(&users, 0));
    assert_eq!(valid_port_old(Some(8080)), valid_port_new(Some(8080)));
    let name = Some("Ada".to_owned());
    assert_eq!(borrowed_name_old(&name), borrowed_name_new(&name));
    assert_eq!(bearer_old("Bearer token"), bearer_new("Bearer token"));
    assert_eq!(
        key_value_old("name=Ada=admin"),
        key_value_new("name=Ada=admin")
    );
    assert_eq!(
        count_old(&["rust", "go", "rust"]),
        count_new(&["rust", "go", "rust"])
    );
    assert_eq!(
        parse_all_old(&["1", "2"]).unwrap(),
        parse_all_new(&["1", "2"]).unwrap()
    );
    assert_eq!(labels_old(), labels_new());
    assert_eq!(message_old("Ada"), message_new("Ada"));
    assert_eq!(words_old().len(), NEW_WORDS.len());
    assert_eq!(sum_old(&[1, 2, 3]), sum_new(&[1, 2, 3]));
}

// Проверяйте замену в проекте: cargo fmt, cargo clippy --all-targets, cargo test.
// При переходе на edition 2024 сначала cargo fix --edition, затем тесты.
// Источники: https://doc.rust-lang.org/book/ch06-03-if-let.html
// https://doc.rust-lang.org/book/ch09-02-recoverable-errors-with-result.html
// https://doc.rust-lang.org/edition-guide/rust-2024/let-chains.html
// https://doc.rust-lang.org/std/option/enum.Option.html
// https://doc.rust-lang.org/std/primitive.str.html
// https://doc.rust-lang.org/std/collections/hash_map/enum.Entry.html
// https://doc.rust-lang.org/std/result/
// https://doc.rust-lang.org/std/array/fn.from_fn.html
// https://doc.rust-lang.org/std/sync/struct.LazyLock.html
// https://doc.rust-lang.org/std/thread/fn.scope.html
