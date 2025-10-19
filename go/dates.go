package cheatsheets

import (
	"fmt"
	"time"
)

// ---------------------------------------------------
// 📌 Получение текущего времени
// ---------------------------------------------------
func dateNow() {
	now := time.Now()
	fmt.Println(now)
}

// ---------------------------------------------------
// 📌 Создание даты
// ---------------------------------------------------
func dateCreate() {
	// Из компонентов
	date := time.Date(2024, time.January, 15, 10, 30, 0, 0, time.UTC)

	// Из строки
	parsed, _ := time.Parse("2006-01-02", "2024-01-15")
	parsed2, _ := time.Parse(time.RFC3339, "2024-01-15T10:30:00Z")

	// Из timestamp (Unix seconds)
	fromUnix := time.Unix(1705276800, 0)

	// Из timestamp (Unix milliseconds)
	fromMillis := time.UnixMilli(1705276800000)

	fmt.Println(date, parsed, parsed2, fromUnix, fromMillis)
}

// ---------------------------------------------------
// 📌 Форматирование даты
// ---------------------------------------------------
func dateFormat() {
	now := time.Now()

	// Стандартные форматы
	rfc3339 := now.Format(time.RFC3339) // "2024-01-15T10:30:00Z"
	rfc1123 := now.Format(time.RFC1123) // "Mon, 15 Jan 2024 10:30:00 MST"
	ansic := now.Format(time.ANSIC)     // "Mon Jan 15 10:30:00 2024"

	// Кастомный формат (reference: "Mon Jan 2 15:04:05 -0700 MST 2006")
	custom := now.Format("2006-01-02 15:04:05")
	dateOnly := now.Format("2006-01-02")
	timeOnly := now.Format("15:04:05")

	fmt.Println(rfc3339, rfc1123, ansic, custom, dateOnly, timeOnly)
}

// ---------------------------------------------------
// 📌 Парсинг строки в дату
// ---------------------------------------------------
func dateParse() {
	// Layout должен использовать reference time: "Mon Jan 2 15:04:05 MST 2006"
	date, err := time.Parse("2006-01-02", "2024-01-15")
	if err != nil {
		fmt.Println("Ошибка парсинга:", err)
	}

	datetime, _ := time.Parse("2006-01-02 15:04:05", "2024-01-15 10:30:00")
	rfc, _ := time.Parse(time.RFC3339, "2024-01-15T10:30:00Z")

	fmt.Println(date, datetime, rfc)
}

// ---------------------------------------------------
// 📌 Получение компонентов даты
// ---------------------------------------------------
func dateComponents() {
	now := time.Now()

	year := now.Year()
	month := now.Month()         // time.Month (January = 1)
	monthInt := int(now.Month()) // 1-12
	day := now.Day()             // 1-31
	weekday := now.Weekday()     // time.Weekday (Sunday = 0)
	hour := now.Hour()           // 0-23
	minute := now.Minute()       // 0-59
	second := now.Second()       // 0-59
	nanosecond := now.Nanosecond()
	yearDay := now.YearDay() // 1-365/366

	fmt.Println(year, month, monthInt, day, weekday, hour, minute, second, nanosecond, yearDay)
}

// ---------------------------------------------------
// 📌 Операции с датами (добавление/вычитание)
// ---------------------------------------------------
func dateArithmetic() {
	now := time.Now()

	// Добавление времени
	tomorrow := now.Add(24 * time.Hour)
	tomorrow2 := now.AddDate(0, 0, 1) // (years, months, days)

	nextWeek := now.AddDate(0, 0, 7)
	nextMonth := now.AddDate(0, 1, 0)
	nextYear := now.AddDate(1, 0, 0)

	inOneHour := now.Add(1 * time.Hour)
	in30Minutes := now.Add(30 * time.Minute)
	in5Seconds := now.Add(5 * time.Second)

	// Вычитание (используйте отрицательное значение)
	yesterday := now.Add(-24 * time.Hour)
	yesterdayAlt := now.AddDate(0, 0, -1)

	fmt.Println(tomorrow, tomorrow2, nextWeek, nextMonth, nextYear, inOneHour, in30Minutes, in5Seconds, yesterday, yesterdayAlt)
}

// ---------------------------------------------------
// 📌 Разница между датами
// ---------------------------------------------------
func dateDifference() {
	date1 := time.Date(2024, 1, 1, 0, 0, 0, 0, time.UTC)
	date2 := time.Date(2024, 1, 15, 0, 0, 0, 0, time.UTC)

	diff := date2.Sub(date1)

	diffNanoseconds := diff.Nanoseconds()
	diffMicroseconds := diff.Microseconds()
	diffMilliseconds := diff.Milliseconds()
	diffSeconds := diff.Seconds()
	diffMinutes := diff.Minutes()
	diffHours := diff.Hours()
	diffDays := diff.Hours() / 24

	fmt.Println(diffNanoseconds, diffMicroseconds, diffMilliseconds, diffSeconds, diffMinutes, diffHours, diffDays)
}

// ---------------------------------------------------
// 📌 Сравнение дат
// ---------------------------------------------------
func dateCompare() {
	date1 := time.Now()
	date2 := time.Now().Add(1 * time.Hour)

	isBefore := date1.Before(date2)
	isAfter := date1.After(date2)
	isEqual := date1.Equal(date2)

	fmt.Println(isBefore, isAfter, isEqual)
}

// ---------------------------------------------------
// 📌 Timestamp (Unix seconds/milliseconds)
// ---------------------------------------------------
func dateTimestamp() {
	now := time.Now()

	unixSeconds := now.Unix()
	unixMillis := now.UnixMilli()
	unixMicro := now.UnixMicro()
	unixNano := now.UnixNano()

	fmt.Println(unixSeconds, unixMillis, unixMicro, unixNano)
}

// ---------------------------------------------------
// 📌 Начало/конец дня
// ---------------------------------------------------
func startOfDay(t time.Time) time.Time {
	year, month, day := t.Date()
	return time.Date(year, month, day, 0, 0, 0, 0, t.Location())
}

func endOfDay(t time.Time) time.Time {
	year, month, day := t.Date()
	return time.Date(year, month, day, 23, 59, 59, 999999999, t.Location())
}

// ---------------------------------------------------
// 📌 Начало/конец месяца
// ---------------------------------------------------
func startOfMonth(t time.Time) time.Time {
	year, month, _ := t.Date()
	return time.Date(year, month, 1, 0, 0, 0, 0, t.Location())
}

func endOfMonth(t time.Time) time.Time {
	return startOfMonth(t).AddDate(0, 1, 0).Add(-time.Nanosecond)
}

// ---------------------------------------------------
// 📌 Високосный год
// ---------------------------------------------------
func isLeapYear(year int) bool {
	return (year%4 == 0 && year%100 != 0) || (year%400 == 0)
}

// ---------------------------------------------------
// 📌 Измерение времени выполнения
// ---------------------------------------------------
func measureTime() {
	start := time.Now()

	// ... выполнение кода ...
	time.Sleep(100 * time.Millisecond)

	elapsed := time.Since(start)
	fmt.Println("Время выполнения:", elapsed)
}

// ---------------------------------------------------
// 📌 Таймер и Ticker
// ---------------------------------------------------
func dateTimerTicker() {
	// Timer - одноразовое событие
	timer := time.NewTimer(2 * time.Second)
	<-timer.C
	fmt.Println("Timer expired")

	// Ticker - повторяющееся событие
	ticker := time.NewTicker(500 * time.Millisecond)
	defer ticker.Stop()

	count := 0
	for range ticker.C {
		fmt.Println("Tick", count)
		count++
		if count >= 3 {
			break
		}
	}
}

// ---------------------------------------------------
// 📌 Sleep (задержка)
// ---------------------------------------------------
func dateSleep() {
	time.Sleep(1 * time.Second)
	time.Sleep(500 * time.Millisecond)
}

// ---------------------------------------------------
// 📌 Работа с timezone
// ---------------------------------------------------
func dateTimezone() {
	now := time.Now()

	// UTC
	utc := now.UTC()

	// Локальное время
	local := now.Local()

	// Конкретная таймзона
	loc, _ := time.LoadLocation("America/New_York")
	ny := now.In(loc)

	// Смещение от UTC
	_, offset := now.Zone()
	offsetHours := offset / 3600

	fmt.Println(utc, local, ny, offsetHours)
}

// ---------------------------------------------------
// 📌 Truncate и Round
// ---------------------------------------------------
func dateTruncateRound() {
	now := time.Now()

	// Truncate - отбрасывает меньшие компоненты
	truncHour := now.Truncate(time.Hour)
	truncDay := now.Truncate(24 * time.Hour)

	// Round - округляет
	roundHour := now.Round(time.Hour)
	roundMinute := now.Round(time.Minute)

	fmt.Println(truncHour, truncDay, roundHour, roundMinute)
}

// ---------------------------------------------------
// 📌 Получение номера недели в году
// ---------------------------------------------------
func getWeekNumber(t time.Time) int {
	_, week := t.ISOWeek()
	return week
}

// ---------------------------------------------------
// 📌 Deadline и Timeout
// ---------------------------------------------------
func dateDeadline() {
	deadline := time.Now().Add(5 * time.Second)

	select {
	case <-time.After(3 * time.Second):
		fmt.Println("Операция завершена")
	case <-time.After(time.Until(deadline)):
		fmt.Println("Превышено время ожидания")
	}
}

// ---------------------------------------------------
// 📌 Проверка, что дата нулевая
// ---------------------------------------------------
func isZeroDate(t time.Time) bool {
	return t.IsZero()
}

// ---------------------------------------------------
// 📌 Duration операции
// ---------------------------------------------------
func durationOps() {
	d := 5*time.Hour + 30*time.Minute

	hours := d.Hours()
	minutes := d.Minutes()
	seconds := d.Seconds()

	// Строка
	str := d.String() // "5h30m0s"

	// Truncate
	truncated := d.Truncate(time.Hour) // 5h

	// Round
	rounded := d.Round(time.Hour) // 6h

	fmt.Println(hours, minutes, seconds, str, truncated, rounded)
}

// ---------------------------------------------------
// 📌 Возраст (количество полных лет)
// ---------------------------------------------------
func calculateAge(birthDate time.Time) int {
	now := time.Now()
	age := now.Year() - birthDate.Year()

	if now.YearDay() < birthDate.YearDay() {
		age--
	}

	return age
}

// ---------------------------------------------------
// 📌 Добавление рабочих дней (пропуская выходные)
// ---------------------------------------------------
func addBusinessDays(t time.Time, days int) time.Time {
	result := t
	addedDays := 0

	for addedDays < days {
		result = result.AddDate(0, 0, 1)
		weekday := result.Weekday()
		if weekday != time.Saturday && weekday != time.Sunday {
			addedDays++
		}
	}

	return result
}
