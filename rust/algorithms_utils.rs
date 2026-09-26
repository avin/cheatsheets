use std::collections::VecDeque;

// ---------------------------------------------------
// 📌 Сортировка и бинарный поиск
// ---------------------------------------------------
fn sort_and_search(mut values: Vec<i32>, target: i32) {
    values.sort_unstable(); // Когда порядок равных элементов важен, используйте sort().
    match values.binary_search(&target) {
        Ok(index) => println!("{target} найден по индексу {index}"),
        Err(index) => println!("{target} отсутствует; позиция вставки {index}"),
    }
    println!("{values:?}");
}

// ---------------------------------------------------
// 📌 Цепочки итераторов
// ---------------------------------------------------
fn even_squares(values: &[i32]) -> Vec<i32> {
    values
        .iter()
        .copied()
        .filter(|value| value % 2 == 0)
        .map(|value| value * value)
        .collect()
}

fn sum(values: &[i32]) -> i32 {
    values.iter().copied().fold(0, |total, value| total + value)
    // Для простой суммы короче: values.iter().sum()
}

// ---------------------------------------------------
// 📌 Стек на Vec и очередь на VecDeque
// ---------------------------------------------------
fn stack_and_queue() {
    let mut stack = Vec::new();
    stack.push(10);
    stack.push(20);
    println!("stack pop={:?}", stack.pop()); // None, если стек пуст.

    let mut queue = VecDeque::new();
    queue.push_back(1);
    queue.push_back(2);
    println!("queue pop={:?}", queue.pop_front());
}

fn main() {
    sort_and_search(vec![5, 2, 9, 1, 7], 7);
    println!("squares={:?}", even_squares(&[1, 2, 3, 4]));
    println!("sum={}", sum(&[1, 2, 3]));
    stack_and_queue();
}
