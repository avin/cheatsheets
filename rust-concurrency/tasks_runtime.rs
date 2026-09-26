use std::error::Error;
use std::time::Duration;
use tokio::task::{self, JoinSet};

// ---------------------------------------------------
// 📌 Зависимости в Cargo.toml
// ---------------------------------------------------
// [dependencies]
// tokio = { version = "1", features = ["full"] }
// Для shutdown.rs также: tokio-util = { version = "0.7", features = ["rt"] }
// Каждый файл здесь — отдельный пример; помещайте его в src/main.rs проекта Cargo.

// ---------------------------------------------------
// 📌 join!: совместное ожидание без создания новых задач
// ---------------------------------------------------
async fn fetch_part(id: u32) -> String {
    tokio::time::sleep(Duration::from_millis(10)).await;
    format!("part {id}")
}

async fn joined() {
    let (left, right) = tokio::join!(fetch_part(1), fetch_part(2));
    println!("{left}, {right}");
    // Оба future исполняются конкурентно в текущей задаче, не обязательно параллельно.
}

// ---------------------------------------------------
// 📌 JoinSet: собираем результаты по мере завершения задач
// ---------------------------------------------------
async fn task_group() -> Result<Vec<u32>, Box<dyn Error>> {
    let mut set = JoinSet::new();
    for id in 1..=3 {
        set.spawn(async move {
            tokio::time::sleep(Duration::from_millis(u64::from(4 - id) * 10)).await;
            id * id
        });
    }

    let mut results = Vec::new();
    while let Some(result) = set.join_next().await {
        results.push(result?); // JoinError означает панику или отмену задачи.
    }
    Ok(results) // Порядок соответствует завершению, а не запуску.
}

// ---------------------------------------------------
// 📌 spawn_blocking: синхронный код не задерживает async runtime
// ---------------------------------------------------
async fn blocking_work() -> Result<u64, Box<dyn Error>> {
    let result = task::spawn_blocking(|| (1_u64..=100_000).sum::<u64>()).await?;
    // Начавшуюся spawn_blocking-задачу нельзя остановить через abort().
    Ok(result)
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    joined().await;
    let handle = tokio::spawn(fetch_part(3)); // spawn требует Send + 'static.
    println!("spawn={}", handle.await?);
    println!("group={:?}", task_group().await?);
    println!("blocking={}", blocking_work().await?);
    Ok(())
}
