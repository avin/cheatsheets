use reqwest::Client;
use serde::Serialize;
use serde_json::Value;
use std::error::Error;
use std::time::Duration;

// ---------------------------------------------------
// 📌 Cargo.toml: HTTP-клиент и JSON
// ---------------------------------------------------
// tokio = { version = "1", features = ["full"] }
// reqwest = { version = "0.13", features = ["json", "query"] }
// serde = { version = "1", features = ["derive"] }
// serde_json = "1"

#[derive(Serialize)]
struct NewItem<'a> {
    name: &'a str,
}

// ---------------------------------------------------
// 📌 Один Client переиспользует соединения между запросами
// ---------------------------------------------------
fn http_client() -> reqwest::Result<Client> {
    Client::builder().timeout(Duration::from_secs(5)).build()
}

async fn get_items(client: &Client, base_url: &str) -> reqwest::Result<Value> {
    client
        .get(format!("{base_url}/items"))
        .query(&[("limit", 10)]) // Не собирайте query string вручную.
        .send()
        .await?
        .error_for_status()? // 4xx/5xx становятся Err.
        .json::<Value>()
        .await
}

// ---------------------------------------------------
// 📌 POST JSON и проверка статуса ответа
// ---------------------------------------------------
async fn create_item(client: &Client, base_url: &str, name: &str) -> reqwest::Result<Value> {
    client
        .post(format!("{base_url}/items"))
        .json(&NewItem { name })
        .send()
        .await?
        .error_for_status()?
        .json::<Value>()
        .await
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let Some(base_url) = std::env::args().nth(1) else {
        println!("Передайте базовый URL, например http://127.0.0.1:3000");
        return Ok(());
    };
    let client = http_client()?;
    println!(
        "created={}",
        create_item(&client, &base_url, "example").await?
    );
    println!("items={}", get_items(&client, &base_url).await?);
    // Повторять запрос можно лишь с учётом его идемпотентности и результата сервера.
    Ok(())
}
