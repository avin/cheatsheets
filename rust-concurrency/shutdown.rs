use std::future::Future;
use std::time::Duration;
use tokio_util::sync::CancellationToken;
use tokio_util::task::TaskTracker;

// ---------------------------------------------------
// 📌 Рабочая задача реагирует на сигнал отмены
// ---------------------------------------------------
async fn worker(id: usize, token: CancellationToken) {
    loop {
        tokio::select! {
            _ = token.cancelled() => break,
            _ = tokio::time::sleep(Duration::from_millis(20)) => {
                println!("worker {id}: heartbeat");
            }
        }
    }
    // Здесь можно завершить запись, сбросить буфер, закрыть соединение.
    println!("worker {id}: stopped");
}

// ---------------------------------------------------
// 📌 Сигнал отмены + ожидание всех задач
// ---------------------------------------------------
async fn run_workers_until(shutdown: impl Future<Output = ()>) {
    let token = CancellationToken::new();
    let tracker = TaskTracker::new();
    for id in 0..3 {
        tracker.spawn(worker(id, token.child_token()));
    }

    shutdown.await;
    token.cancel();
    tracker.close(); // wait() завершится, когда tracker закрыт и все задачи вышли.
    tracker.wait().await;
}

// ---------------------------------------------------
// 📌 В реальном сервисе используйте сигнал ОС вместо таймера
// ---------------------------------------------------
#[tokio::main]
async fn main() {
    run_workers_until(tokio::time::sleep(Duration::from_millis(50))).await;
    // В сервисе вместо sleep():
    // run_workers_until(async { tokio::signal::ctrl_c().await.expect("signal error") }).await;
}
