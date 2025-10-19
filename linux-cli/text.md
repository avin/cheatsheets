# Обработка текста

## Просмотр и базовые операции

```bash
cat notes.txt                        # вывести файл
less +F app.log                      # режим tail в less
head -n 20 access.log                # первые 20 строк
tail -n 50 -f access.log             # последние строки с обновлением в реальном времени
```

## Извлечение колонок

```bash
cut -d',' -f1,3 data.csv             # выбрать столбцы 1 и 3
awk '{print $1, $NF}' access.log     # первая и последняя колонка
```

## Сортировка и подсчёт

```bash
sort words.txt | uniq -c | sort -nr  # частоты слов
sort -t, -k2,2n data.csv             # сортировка по второй колонке как числу
```

## Манипуляции со строками

```bash
sed -n 's/error/ERROR/p' app.log     # заменить и вывести совпадения
sed '1,5d' file.txt                  # удалить первые 5 строк
awk '$3 > 100 {print $1,$3}' stats   # фильтрация по условию
```

## Форматирование и выравнивание

```bash
column -t -s, data.csv               # таблица с выравниванием
paste -d',' col1.txt col2.txt        # объединить файлы по строкам
tr '[:lower:]' '[:upper:]' < text    # преобразовать регистр
```
