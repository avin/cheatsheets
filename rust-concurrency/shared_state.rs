use std::sync::{Arc, Mutex};
use std::time::Duration;
use tokio::sync::Mutex as AsyncMutex;

// ---------------------------------------------------
// 📌 Обычный Mutex: короткая операция без .await под блокировкой
// ---------------------------------------------------
async fn short_critical_section() {
    let state = Arc::new(Mutex::new(Vec::new()));
    let mut handles = Vec::new();
    for id in 0..3 {
        let state = Arc::clone(&state);
        handles.push(tokio::spawn(async move {
            tokio::time::sleep(Duration::from_millis(10)).await;
            state.lock().expect("mutex poisoned").push(id);
            // Guard уничтожен до следующего .await.
        }));
    }
    for handle in handles {
        handle.await.expect("worker panicked");
    }
    println!("state={:?}", state.lock().expect("mutex poisoned"));
}

// ---------------------------------------------------
// 📌 AsyncMutex: если действительно требуется держать guard через .await
// ---------------------------------------------------
async fn async_critical_section() {
    let state = Arc::new(AsyncMutex::new(String::new()));
    let worker_state = Arc::clone(&state);
    let worker = tokio::spawn(async move {
        let mut guard = worker_state.lock().await;
        guard.push_str("loading");
        tokio::time::sleep(Duration::from_millis(10)).await;
        guard.push_str(" done");
    });
    worker.await.expect("worker panicked");
    println!("{}", state.lock().await);
    // Долгое удержание guard сериализует работу; обычно лучше копировать данные и отпускать его.
}

#[tokio::main]
async fn main() {
    short_critical_section().await;
    async_critical_section().await;
}
