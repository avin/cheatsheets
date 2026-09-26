use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Arc, Mutex, mpsc};
use std::thread;

// ---------------------------------------------------
// 📌 Поток с передачей владения и join
// ---------------------------------------------------
fn spawn_worker() {
    let message = String::from("hello");
    let handle = thread::spawn(move || println!("worker: {message}"));
    handle.join().expect("worker panicked");
}

// ---------------------------------------------------
// 📌 thread::scope: потоки могут заимствовать локальные данные
// ---------------------------------------------------
fn scoped_workers() {
    let values = [1, 2, 3, 4];
    thread::scope(|scope| {
        scope.spawn(|| println!("left={}", values[0] + values[1]));
        scope.spawn(|| println!("right={}", values[2] + values[3]));
    }); // К этому моменту оба потока завершены.
}

// ---------------------------------------------------
// 📌 Канал для передачи сообщений
// ---------------------------------------------------
fn channel_example() {
    let (sender, receiver) = mpsc::channel();
    let handle = thread::spawn(move || sender.send(String::from("готово")).unwrap());
    println!("{}", receiver.recv().expect("channel closed"));
    handle.join().expect("sender panicked");
}

// ---------------------------------------------------
// 📌 Arc<Mutex<T>> для общего изменяемого состояния
// ---------------------------------------------------
fn mutex_example() {
    let counter = Arc::new(Mutex::new(0usize));
    let handles: Vec<_> = (0..4)
        .map(|_| {
            let counter = Arc::clone(&counter);
            thread::spawn(move || {
                let mut value = counter.lock().expect("mutex poisoned");
                *value += 1;
            })
        })
        .collect();
    for handle in handles {
        handle.join().expect("worker panicked");
    }
    println!("mutex counter={}", *counter.lock().unwrap());
}

// ---------------------------------------------------
// 📌 AtomicUsize, если достаточно атомарных операций
// ---------------------------------------------------
fn atomic_example() {
    let counter = Arc::new(AtomicUsize::new(0));
    thread::scope(|scope| {
        for _ in 0..4 {
            let counter = Arc::clone(&counter);
            scope.spawn(move || {
                counter.fetch_add(1, Ordering::Relaxed);
            });
        }
    });
    // Relaxed подходит для счётчика, если не синхронизируем другие данные.
    println!("atomic counter={}", counter.load(Ordering::Relaxed));
}

fn main() {
    spawn_worker();
    scoped_workers();
    channel_example();
    mutex_example();
    atomic_example();
}
