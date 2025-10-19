package cheatsheets

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"time"
)

// ---------------------------------------------------
// 📌 Определение структур
// ---------------------------------------------------
type User struct {
	ID        int       `json:"id"`
	Name      string    `json:"name"`
	CreatedAt time.Time `json:"created_at"`
	Active    bool      `json:"active"`
}

// ---------------------------------------------------
// 📌 Конструктор и опции
// ---------------------------------------------------
type Option func(*User)

func WithName(name string) Option {
	return func(u *User) { u.Name = name }
}

func WithActive(active bool) Option {
	return func(u *User) { u.Active = active }
}

func NewUser(opts ...Option) *User {
	u := &User{CreatedAt: time.Now(), Active: true}
	for _, opt := range opts {
		opt(u)
	}
	fmt.Println("new user", u.Name, u.Active)
	return u
}

// ---------------------------------------------------
// 📌 Методы со значением и указателем
// ---------------------------------------------------
func (u User) Clone() User {
	return User{ID: u.ID, Name: u.Name, CreatedAt: u.CreatedAt, Active: u.Active}
}

func (u *User) Deactivate() {
	u.Active = false
}

// ---------------------------------------------------
// 📌 Композиция и встраивание
// ---------------------------------------------------
type Audit struct {
	CreatedBy string
	UpdatedBy string
}

type Account struct {
	User
	Audit
	Balance int64
}

func accountExample() {
	acc := Account{}
	acc.User.Name = "Bob"
	acc.Audit.CreatedBy = "system"
	acc.Balance += 100
	fmt.Println(acc.User.Name, acc.Balance)
}

// ---------------------------------------------------
// 📌 Интерфейсы и полиморфизм
// ---------------------------------------------------
type Notifier interface {
	Notify(message string) error
}

type EmailNotifier struct{}

type SMSNotifier struct{}

func (EmailNotifier) Notify(msg string) error {
	fmt.Println("email:", msg)
	return nil
}

func (SMSNotifier) Notify(msg string) error {
	fmt.Println("sms:", msg)
	return nil
}

func SendAll(ns []Notifier, msg string) {
	for _, n := range ns {
		if err := n.Notify(msg); err != nil {
			log.Println("notify error", err)
		}
	}
}

// ---------------------------------------------------
// 📌 JSON сериализация
// ---------------------------------------------------
func objectJSON(u User) User {
	raw, err := json.MarshalIndent(u, "", "  ")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(string(raw))
	var copy User
	if err := json.Unmarshal(raw, &copy); err != nil {
		log.Fatal(err)
	}
	return copy
}

// ---------------------------------------------------
// 📌 Имплементация интерфейсов по умолчанию
// ---------------------------------------------------
type Logger interface {
	Println(...any)
}

func Process(l Logger) {
	l.Println("started")
}

func objectLoggerExample() {
	Process(log.Default())
	Process(log.New(os.Stdout, "prefix ", log.LstdFlags))
}
