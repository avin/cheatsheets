use tokio::sync::mpsc;
use tokio_util::sync::CancellationToken;

// ---------------------------------------------------
// 📌 Функция для тестирования
// ---------------------------------------------------
async fn forward(mut input: mpsc::Receiver<u32>, output: mpsc::Sender<u32>) {
    while let Some(value) = input.recv().await {
        if output.send(value * 2).await.is_err() {
            break; // Получатель результата отключился.
        }
    }
}

// ---------------------------------------------------
// 📌 Команды разработки
// ---------------------------------------------------
fn main() {
    // Cargo: cargo test; cargo clippy --all-targets; cargo fmt --check.
    // Для сложной гонки проверяйте результат через канал, не через sleep().
    let _ = forward;
    let _ = CancellationToken::new();
}

// ---------------------------------------------------
// 📌 tokio::test: проверяем результат и закрытие канала
// ---------------------------------------------------
#[cfg(test)]
mod tests {
    use super::*;

    #[tokio::test]
    async fn forwards_values() {
        let (input_tx, input_rx) = mpsc::channel(2);
        let (output_tx, mut output_rx) = mpsc::channel(2);
        let worker = tokio::spawn(forward(input_rx, output_tx));

        input_tx.send(21).await.unwrap();
        drop(input_tx); // Без этого worker продолжал бы ждать новые сообщения.
        assert_eq!(output_rx.recv().await, Some(42));
        worker.await.expect("worker panicked");
        assert_eq!(output_rx.recv().await, None);
    }

    #[tokio::test]
    async fn cancellation_wakes_waiter() {
        let token = CancellationToken::new();
        let child = token.child_token();
        let waiter = tokio::spawn(async move {
            child.cancelled().await;
            "stopped"
        });
        token.cancel();
        assert_eq!(waiter.await.unwrap(), "stopped");
    }
}
