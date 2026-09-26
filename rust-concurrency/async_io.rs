use std::error::Error;
use tokio::io::{AsyncBufReadExt, AsyncWriteExt, BufReader};
use tokio::net::{TcpListener, TcpStream};

// ---------------------------------------------------
// 📌 TCP-сервер: одно соединение, строки как границы сообщений
// ---------------------------------------------------
async fn serve_one(listener: TcpListener) -> std::io::Result<()> {
    let (socket, _peer) = listener.accept().await?;
    let (read_half, mut write_half) = socket.into_split();
    let mut reader = BufReader::new(read_half);
    let mut line = String::new();

    if reader.read_line(&mut line).await? != 0 {
        write_half.write_all(line.as_bytes()).await?;
        write_half.flush().await?;
    }
    Ok(())
}

// ---------------------------------------------------
// 📌 TCP-клиент: запись и чтение ответа
// ---------------------------------------------------
async fn request(addr: std::net::SocketAddr) -> std::io::Result<String> {
    let socket = TcpStream::connect(addr).await?;
    let (read_half, mut write_half) = socket.into_split();
    write_half.write_all(b"hello\n").await?;
    let mut reader = BufReader::new(read_half);
    let mut response = String::new();
    reader.read_line(&mut response).await?;
    Ok(response)
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let listener = TcpListener::bind("127.0.0.1:0").await?;
    let addr = listener.local_addr()?;
    let server = tokio::spawn(serve_one(listener));
    println!("reply={:?}", request(addr).await?);
    server.await??;
    // TCP — поток байт: read() не гарантирует целого сообщения.
    // Для недоверенного ввода задайте максимальную длину сообщения.
    Ok(())
}
