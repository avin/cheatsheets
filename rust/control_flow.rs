// ---------------------------------------------------
// 📌 Условия и match
// ---------------------------------------------------
fn describe_temperature(celsius: i32) -> &'static str {
    if celsius < 0 {
        "мороз"
    } else if celsius < 25 {
        "прохладно"
    } else {
        "жарко"
    }
}

fn describe_day(day: u8) -> &'static str {
    match day {
        1..=5 => "рабочий день",
        6 | 7 => "выходной",
        _ => "неизвестный день",
    }
}

// ---------------------------------------------------
// 📌 if let и let else для необязательного значения
// ---------------------------------------------------
fn print_first(values: &[i32]) {
    let Some(first) = values.first() else {
        println!("список пуст");
        return;
    };
    println!("первый: {first}");

    if let Some(last) = values.last() {
        println!("последний: {last}");
    }
}

// ---------------------------------------------------
// 📌 for, while, loop и метки
// ---------------------------------------------------
fn find_positive(rows: &[&[i32]]) -> Option<(usize, usize, i32)> {
    'search: {
        for (row_index, row) in rows.iter().enumerate() {
            for (column_index, &value) in row.iter().enumerate() {
                if value > 0 {
                    break 'search Some((row_index, column_index, value));
                }
            }
        }
        None
    }
}

fn main() {
    println!("{}", describe_temperature(10));
    println!("{}", describe_day(6));
    print_first(&[4, 8, 15]);

    for index in 0..3 {
        println!("for: {index}");
    }
    let mut countdown = 3;
    while countdown > 0 {
        countdown -= 1;
    }
    let answer = loop {
        if countdown == 0 {
            break 42;
        }
    };
    println!(
        "answer={answer}, found={:?}",
        find_positive(&[&[-2, 0], &[7, 9]])
    );
}
