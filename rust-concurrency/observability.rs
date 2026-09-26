use tracing::{error, info, instrument};
use tracing_subscriber::EnvFilter;

// ---------------------------------------------------
// 📌 Cargo.toml: структурированные события и подписчик
// ---------------------------------------------------
// tokio = { version = "1", features = ["full"] }
// tracing = "0.1"
// tracing-subscriber = { version = "0.3", features = ["env-filter"] }

// ---------------------------------------------------
// 📌 Спан связывает события с конкретной операцией
// ---------------------------------------------------
#[instrument(skip(payload), fields(payload_len = payload.len()))]
async fn process(id: u64, payload: String) -> Result<usize, &'static str> {
    if payload.is_empty() {
        error!("empty payload");
        return Err("empty payload");
    }
    info!("processing started");
    tokio::task::yield_now().await;
    info!("processing finished");
    let _ = id; // id уже записан в поля спана.
    Ok(payload.len())
}

// ---------------------------------------------------
// 📌 Фильтр из RUST_LOG и проверка результата задачи
// ---------------------------------------------------
#[tokio::main]
async fn main() {
    let filter = EnvFilter::try_from_default_env().unwrap_or_else(|_| EnvFilter::new("info"));
    tracing_subscriber::fmt().with_env_filter(filter).init();

    let handle = tokio::spawn(process(42, "secret data".to_owned()));
    match handle.await {
        Ok(Ok(size)) => info!(size, "task completed"),
        Ok(Err(error)) => error!(%error, "task failed"),
        Err(join_error) => error!(%join_error, "task panicked or was cancelled"),
    }
    // RUST_LOG=debug включает более подробные события без правки кода.
    // skip(payload) не записывает чувствительное содержимое в спан.
}
