use std::error::Error;
use std::sync::Arc;
use std::time::Duration;
use tokio::sync::{Semaphore, mpsc};
use tokio::task::JoinSet;

// ---------------------------------------------------
// 📌 Ограниченная очередь: производитель ждёт медленного потребителя
// ---------------------------------------------------
async fn produce(sender: mpsc::Sender<u32>) {
    for item in 1..=8 {
        if sender.send(item).await.is_err() {
            return; // Потребитель завершился.
        }
    }
} // При выходе sender уничтожается; recv() вернёт None после опустошения очереди.

// ---------------------------------------------------
// 📌 Семафор ограничивает число одновременно запущенных задач
// ---------------------------------------------------
async fn run_pipeline() -> Result<Vec<u32>, Box<dyn Error>> {
    let (sender, mut receiver) = mpsc::channel(4);
    let producer = tokio::spawn(produce(sender));
    let limit = Arc::new(Semaphore::new(2));
    let mut workers = JoinSet::new();

    while let Some(item) = receiver.recv().await {
        let permit = Arc::clone(&limit).acquire_owned().await?;
        // Разрешение получено ДО spawn: число ожидающих задач тоже ограничено.
        workers.spawn(async move {
            let _permit = permit; // Освобождается автоматически при выходе из задачи.
            tokio::time::sleep(Duration::from_millis(10)).await;
            item * item
        });
    }

    producer.await?;
    let mut output = Vec::new();
    while let Some(result) = workers.join_next().await {
        output.push(result?);
    }
    output.sort_unstable(); // Завершение задач может идти в другом порядке.
    Ok(output)
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    println!("{:?}", run_pipeline().await?);
    Ok(())
}
