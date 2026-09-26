use std::fmt;

// ---------------------------------------------------
// 📌 struct, методы и derive
// ---------------------------------------------------
#[derive(Debug, Clone, PartialEq)]
struct Movie {
    title: String,
    year: u16,
    rating: f32,
}

impl Movie {
    fn new(title: impl Into<String>, year: u16, rating: f32) -> Self {
        Self {
            title: title.into(),
            year,
            rating,
        }
    }

    fn is_classic(&self) -> bool {
        self.year < 2000
    }
}

// ---------------------------------------------------
// 📌 enum с данными вместо отдельного тега и union
// ---------------------------------------------------
enum Event {
    Key { code: u32 },
    Mouse { x: i32, y: i32 },
    Quit,
}

fn handle_event(event: Event) {
    match event {
        Event::Key { code } => println!("key: {code}"),
        Event::Mouse { x, y } => println!("mouse: ({x}, {y})"),
        Event::Quit => println!("quit"),
    }
}

// ---------------------------------------------------
// 📌 Реализация трейта и impl Trait
// ---------------------------------------------------
impl fmt::Display for Movie {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            formatter,
            "{} ({}) — {:.1}",
            self.title, self.year, self.rating
        )
    }
}

fn print_item(item: &impl fmt::Display) {
    println!("{item}");
}

fn main() {
    let movie = Movie::new("The Matrix", 1999, 8.7);
    println!("classic={}", movie.is_classic());
    print_item(&movie);
    handle_event(Event::Key { code: 65 });
    handle_event(Event::Mouse { x: 120, y: 64 });
    handle_event(Event::Quit);
}
