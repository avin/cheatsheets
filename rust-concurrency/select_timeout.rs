use std::time::Duration;
use tokio::sync::mpsc;
use tokio::time::{self, MissedTickBehavior};

// ---------------------------------------------------
// 📌 select!: ждём сообщение или истечение срока
// ---------------------------------------------------
async fn receive_or_timeout() {
    let (sender, mut receiver) = mpsc::channel(1);
    let producer = tokio::spawn(async move {
        time::sleep(Duration::from_millis(10)).await;
        let _ = sender.send("готово").await;
    });

    tokio::select! {
        message = receiver.recv() => println!("message={message:?}"),
        _ = time::sleep(Duration::from_millis(100)) => println!("время вышло"),
    }
    // recv() безопасно отменять: проигравшая ветка не забирает сообщение.
    producer.await.expect("producer panicked");
}

// ---------------------------------------------------
// 📌 timeout над задачей: таймаут НЕ останавливает spawn
// ---------------------------------------------------
async fn stop_timed_out_task() {
    let mut handle = tokio::spawn(async {
        time::sleep(Duration::from_secs(1)).await;
        42
    });

    match time::timeout(Duration::from_millis(10), &mut handle).await {
        Ok(Ok(value)) => println!("value={value}"),
        Ok(Err(error)) => eprintln!("task failed: {error}"),
        Err(_) => {
            handle.abort();
            let _ = handle.await; // Дожидаемся завершения отмены.
            println!("task timed out");
        }
    }
    // Если передать в timeout обычный future, при таймауте он будет отброшен.
}

// ---------------------------------------------------
// 📌 interval для периодической работы
// ---------------------------------------------------
async fn periodic() {
    let mut ticks = time::interval(Duration::from_millis(20));
    ticks.set_missed_tick_behavior(MissedTickBehavior::Skip);
    for _ in 0..3 {
        ticks.tick().await; // Первый tick срабатывает сразу.
        println!("tick");
    }
}

#[tokio::main]
async fn main() {
    receive_or_timeout().await;
    stop_timed_out_task().await;
    periodic().await;
}
