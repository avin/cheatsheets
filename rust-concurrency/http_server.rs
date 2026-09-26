use axum::extract::{Path, State};
use axum::http::StatusCode;
use axum::routing::get;
use axum::{Json, Router};
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::sync::atomic::{AtomicU64, Ordering};
use std::sync::{Arc, Mutex};

// ---------------------------------------------------
// 📌 Cargo.toml: HTTP-сервер и JSON
// ---------------------------------------------------
// tokio = { version = "1", features = ["full"] }
// axum = "0.8"
// serde = { version = "1", features = ["derive"] }
// [dev-dependencies]
// tower = { version = "0.5", features = ["util"] }
// serde_json = "1"

#[derive(Clone, Default)]
struct AppState {
    next_id: Arc<AtomicU64>,
    items: Arc<Mutex<HashMap<u64, Item>>>,
}

#[derive(Clone, Serialize)]
struct Item {
    id: u64,
    name: String,
}

#[derive(Deserialize)]
struct NewItem {
    name: String,
}

// ---------------------------------------------------
// 📌 Маршруты, общее состояние и явные статусы
// ---------------------------------------------------
fn app() -> Router {
    Router::new()
        .route("/health", get(|| async { StatusCode::OK }))
        .route("/items", get(list_items).post(create_item))
        .route("/items/{id}", get(get_item))
        .with_state(AppState::default())
}

async fn create_item(
    State(state): State<AppState>,
    Json(input): Json<NewItem>,
) -> Result<(StatusCode, Json<Item>), (StatusCode, &'static str)> {
    let name = input.name.trim();
    if name.is_empty() {
        return Err((StatusCode::BAD_REQUEST, "name is empty"));
    }
    let id = state.next_id.fetch_add(1, Ordering::Relaxed) + 1;
    let item = Item {
        id,
        name: name.to_owned(),
    };
    state
        .items
        .lock()
        .expect("mutex poisoned")
        .insert(id, item.clone());
    Ok((StatusCode::CREATED, Json(item)))
}

async fn list_items(State(state): State<AppState>) -> Json<Vec<Item>> {
    let mut items: Vec<_> = state
        .items
        .lock()
        .expect("mutex poisoned")
        .values()
        .cloned()
        .collect();
    items.sort_by_key(|item| item.id);
    Json(items)
}

async fn get_item(
    Path(id): Path<u64>,
    State(state): State<AppState>,
) -> Result<Json<Item>, StatusCode> {
    state
        .items
        .lock()
        .expect("mutex poisoned")
        .get(&id)
        .cloned()
        .map(Json)
        .ok_or(StatusCode::NOT_FOUND)
}

// ---------------------------------------------------
// 📌 Сервис ждёт Ctrl+C и завершает приём новых запросов
// ---------------------------------------------------
#[tokio::main]
async fn main() -> std::io::Result<()> {
    let listener = tokio::net::TcpListener::bind("127.0.0.1:3000").await?;
    axum::serve(listener, app())
        .with_graceful_shutdown(async { tokio::signal::ctrl_c().await.expect("signal error") })
        .await
}

// ---------------------------------------------------
// 📌 Проверка маршрута без запуска TCP-сервера
// ---------------------------------------------------
#[cfg(test)]
mod tests {
    use super::*;
    use axum::body::{Body, to_bytes};
    use axum::http::Request;
    use tower::ServiceExt;

    #[tokio::test]
    async fn health_is_ok() {
        let response = app()
            .oneshot(
                Request::builder()
                    .uri("/health")
                    .body(Body::empty())
                    .unwrap(),
            )
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::OK);
    }

    #[tokio::test]
    async fn creates_and_reads_item() {
        let router = app();
        let created = router
            .clone()
            .oneshot(
                Request::builder()
                    .method("POST")
                    .uri("/items")
                    .header("content-type", "application/json")
                    .body(Body::from(r#"{"name":"Ada"}"#))
                    .unwrap(),
            )
            .await
            .unwrap();
        assert_eq!(created.status(), StatusCode::CREATED);
        let body = to_bytes(created.into_body(), 1024).await.unwrap();
        let item: serde_json::Value = serde_json::from_slice(&body).unwrap();
        assert_eq!(item["name"], "Ada");

        let id = item["id"].as_u64().unwrap();
        let found = router
            .oneshot(
                Request::builder()
                    .uri(format!("/items/{id}"))
                    .body(Body::empty())
                    .unwrap(),
            )
            .await
            .unwrap();
        assert_eq!(found.status(), StatusCode::OK);
    }
}
