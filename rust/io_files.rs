use std::fs::{self, File, OpenOptions};
use std::io::{self, BufRead, BufReader, Read, Write};
use std::path::Path;

// ---------------------------------------------------
// 📌 Чтение и запись целого файла
// ---------------------------------------------------
fn write_report(path: &Path, title: &str, body: &str) -> io::Result<()> {
    fs::write(path, format!("# {title}\n\n{body}\n"))?;
    Ok(())
}

fn read_report(path: &Path) -> io::Result<String> {
    fs::read_to_string(path) // Для нетекстовых файлов используйте fs::read.
}

// ---------------------------------------------------
// 📌 Построчное чтение большого файла
// ---------------------------------------------------
fn print_lines(path: &Path) -> io::Result<()> {
    let file = File::open(path)?;
    for (index, line) in BufReader::new(file).lines().enumerate() {
        println!("{}: {}", index + 1, line?);
    }
    Ok(())
}

// ---------------------------------------------------
// 📌 Дозапись и бинарный ввод-вывод
// ---------------------------------------------------
fn append_line(path: &Path, line: &str) -> io::Result<()> {
    let mut file = OpenOptions::new().create(true).append(true).open(path)?;
    writeln!(file, "{line}")?;
    Ok(())
}

fn write_numbers(path: &Path, values: &[u32]) -> io::Result<()> {
    let mut file = File::create(path)?;
    for &value in values {
        file.write_all(&value.to_le_bytes())?; // Явный порядок байт для переносимого формата.
    }
    Ok(())
}

fn read_numbers(path: &Path) -> io::Result<Vec<u32>> {
    let mut file = File::open(path)?;
    let mut bytes = Vec::new();
    file.read_to_end(&mut bytes)?;
    let chunks = bytes.chunks_exact(4);
    if !chunks.remainder().is_empty() {
        return Err(io::Error::new(io::ErrorKind::InvalidData, "неполное число"));
    }
    Ok(chunks
        .map(|chunk| u32::from_le_bytes(chunk.try_into().unwrap()))
        .collect())
}

fn main() -> io::Result<()> {
    let report = Path::new("report.txt");
    write_report(report, "Metrics", "CPU usage: 15%")?;
    append_line(report, "Memory: 128 MB")?;
    println!("{}", read_report(report)?);
    print_lines(report)?;

    let data = Path::new("data.bin");
    write_numbers(data, &[1, 2, 42])?;
    println!("{:?}", read_numbers(data)?);
    Ok(())
}
