use rusqlite::{Connection, OptionalExtension, Result, params};

// ---------------------------------------------------
// 📌 Cargo.toml: SQLite без отдельной серверной установки
// ---------------------------------------------------
// rusqlite = { version = "0.40", features = ["bundled"] }

#[derive(Debug)]
struct User {
    id: i64,
    name: String,
}

// ---------------------------------------------------
// 📌 Создание таблицы и параметризованный запрос
// ---------------------------------------------------
fn init(connection: &Connection) -> Result<()> {
    connection
        .execute_batch("CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT NOT NULL UNIQUE);")
}

fn find_user(connection: &Connection, id: i64) -> Result<Option<User>> {
    connection
        .query_row(
            "SELECT id, name FROM users WHERE id = ?1",
            params![id],
            |row| {
                Ok(User {
                    id: row.get(0)?,
                    name: row.get(1)?,
                })
            },
        )
        .optional()
}

// ---------------------------------------------------
// 📌 Транзакция: ошибка до commit вызывает откат при drop
// ---------------------------------------------------
fn add_users(connection: &mut Connection, names: &[&str]) -> Result<()> {
    let transaction = connection.transaction()?;
    for name in names {
        transaction.execute("INSERT INTO users (name) VALUES (?1)", params![name])?;
    }
    transaction.commit()
}

fn list_users(connection: &Connection) -> Result<Vec<User>> {
    let mut statement = connection.prepare("SELECT id, name FROM users ORDER BY id")?;
    statement
        .query_map([], |row| {
            Ok(User {
                id: row.get(0)?,
                name: row.get(1)?,
            })
        })?
        .collect()
}

fn main() -> Result<()> {
    let mut connection = Connection::open_in_memory()?;
    init(&connection)?;
    add_users(&mut connection, &["Ada", "Lin"])?;
    if let Some(user) = find_user(&connection, 1)? {
        println!("{}: {}", user.id, user.name);
    }
    println!("{:?}", list_users(&connection)?);
    // Для async runtime синхронный драйвер выполняйте в spawn_blocking или используйте async-драйвер.
    Ok(())
}
