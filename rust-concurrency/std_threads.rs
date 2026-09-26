use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Arc, Mutex};
use std::thread;

// ---------------------------------------------------
// 📌 thread::scope: потоки заимствуют данные без Arc
// ---------------------------------------------------
fn sum_in_parallel(values: &[u64]) -> u64 {
    thread::scope(|scope| {
        let handles: Vec<_> = values
            .chunks(2)
            .map(|chunk| scope.spawn(move || chunk.iter().sum::<u64>()))
            .collect();
        handles
            .into_iter()
            .map(|handle| handle.join().expect("worker panicked"))
            .sum()
    })
}

// ---------------------------------------------------
// 📌 Arc<Mutex<T>>: несколько потоков меняют общее состояние
// ---------------------------------------------------
fn shared_log() -> Vec<String> {
    let log = Arc::new(Mutex::new(Vec::new()));
    let handles: Vec<_> = (0..4)
        .map(|id| {
            let log = Arc::clone(&log);
            thread::spawn(move || {
                let message = format!("worker {id} finished");
                log.lock().expect("mutex poisoned").push(message);
            })
        })
        .collect();

    for handle in handles {
        handle.join().expect("worker panicked");
    }
    // Все потоки завершились; извлекаем Vec без лишнего clone.
    // Порядок строк зависит от планировщика.
    Arc::try_unwrap(log)
        .expect("log still shared")
        .into_inner()
        .expect("mutex poisoned")
}

// ---------------------------------------------------
// 📌 Атомик для независимого счётчика
// ---------------------------------------------------
fn count_work() -> usize {
    let counter = AtomicUsize::new(0);
    thread::scope(|scope| {
        for _ in 0..4 {
            scope.spawn(|| {
                for _ in 0..100 {
                    counter.fetch_add(1, Ordering::Relaxed);
                }
            });
        }
    });
    // Relaxed достаточно, пока счётчик не публикует другие данные.
    counter.load(Ordering::Relaxed)
}

fn main() {
    println!("sum={}", sum_in_parallel(&[1, 2, 3, 4]));
    println!("log={:?}", shared_log());
    println!("count={}", count_work());
    // Потоки удобны для блокирующей или продолжительной синхронной работы.
}
