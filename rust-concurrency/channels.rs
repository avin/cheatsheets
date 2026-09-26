use tokio::sync::{broadcast, mpsc, oneshot, watch};

// ---------------------------------------------------
// 📌 Ограниченный mpsc создаёт обратное давление
// ---------------------------------------------------
async fn mpsc_example() {
    let (sender, mut receiver) = mpsc::channel::<u32>(2);
    let producer = tokio::spawn(async move {
        for value in 0..5 {
            if sender.send(value).await.is_err() {
                break; // Получатель закрыт: прекращаем производство.
            }
        }
    });

    while let Some(value) = receiver.recv().await {
        println!("mpsc: {value}");
    }
    producer.await.expect("producer panicked");
}

// ---------------------------------------------------
// 📌 oneshot для единственного ответа
// ---------------------------------------------------
async fn oneshot_example() {
    let (sender, receiver) = oneshot::channel();
    let worker = tokio::spawn(async move {
        let _ = sender.send(42); // send синхронный; Err означает закрытого получателя.
    });
    println!("oneshot: {:?}", receiver.await);
    worker.await.expect("worker panicked");
}

// ---------------------------------------------------
// 📌 watch хранит последнее состояние, а не историю событий
// ---------------------------------------------------
async fn watch_example() {
    let (sender, mut receiver) = watch::channel("starting");
    sender.send("ready").expect("receiver closed");
    receiver.changed().await.expect("sender closed");
    println!("watch: {}", *receiver.borrow_and_update());
}

// ---------------------------------------------------
// 📌 broadcast отправляет сообщение каждому подписчику
// ---------------------------------------------------
async fn broadcast_example() {
    let (sender, mut first) = broadcast::channel::<&str>(4);
    let mut second = sender.subscribe();
    sender.send("refresh").expect("no receivers");
    println!(
        "broadcast: {}, {}",
        first.recv().await.unwrap(),
        second.recv().await.unwrap()
    );
    // Медленный подписчик может получить RecvError::Lagged: историю нужно ограничивать.
}

#[tokio::main]
async fn main() {
    mpsc_example().await;
    oneshot_example().await;
    watch_example().await;
    broadcast_example().await;
}
