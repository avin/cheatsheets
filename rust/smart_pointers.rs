use std::cell::RefCell;
use std::rc::{Rc, Weak};

// ---------------------------------------------------
// 📌 Box<T> задаёт размер рекурсивного типа
// ---------------------------------------------------
struct ListNode {
    value: i32,
    next: Option<Box<ListNode>>,
}

fn list_sum(node: &ListNode) -> i32 {
    node.value + node.next.as_deref().map(list_sum).unwrap_or(0)
}

// ---------------------------------------------------
// 📌 Rc<RefCell<T>>: общее изменяемое состояние в одном потоке
// ---------------------------------------------------
fn shared_local_state() {
    let shared = Rc::new(RefCell::new(Vec::new()));
    let other_owner = Rc::clone(&shared);
    shared.borrow_mut().push("first");
    other_owner.borrow_mut().push("second");
    println!("{:?}", shared.borrow());
    // RefCell проверяет правила заимствования во время выполнения и может вызвать панику.
    // Для нескольких потоков используйте Arc и подходящий Mutex, а не Rc<RefCell<T>>.
}

// ---------------------------------------------------
// 📌 Weak<T> не создаёт цикл владения parent ↔ child
// ---------------------------------------------------
struct TreeNode {
    name: String,
    parent: RefCell<Weak<TreeNode>>,
    children: RefCell<Vec<Rc<TreeNode>>>,
}

fn tree_example() {
    let parent = Rc::new(TreeNode {
        name: "root".into(),
        parent: RefCell::new(Weak::new()),
        children: RefCell::new(Vec::new()),
    });
    let child = Rc::new(TreeNode {
        name: "leaf".into(),
        parent: RefCell::new(Rc::downgrade(&parent)),
        children: RefCell::new(Vec::new()),
    });
    parent.children.borrow_mut().push(Rc::clone(&child));
    println!("{} → {}", parent.name, child.name);
    drop(parent);
    assert!(child.parent.borrow().upgrade().is_none());
}

fn main() {
    let list = ListNode {
        value: 1,
        next: Some(Box::new(ListNode {
            value: 2,
            next: None,
        })),
    };
    println!("sum={}", list_sum(&list));
    shared_local_state();
    tree_example();
}
