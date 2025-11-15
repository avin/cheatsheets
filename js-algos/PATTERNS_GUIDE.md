# Алгоритмические Паттерны - Краткая Шпаргалка

## 1. Arrays & Hashing

**Hash Map для поиска O(1)**
- Сохраняй элементы в Map по мере прохода
- Для каждого элемента ищи нужное дополнение в Map
- Пример: Two Sum - ищем `target - num` в карте

**Prefix Sum**
- Создай массив накопительных сумм: `[0, sum[0], sum[0:1], ...]`
- Сумма диапазона [i,j] = prefix[j+1] - prefix[i]
- Для подмассивов: храни префиксные суммы в Map, ищи `sum - k`

**Counting/Frequency Map**
- Посчитай частоту каждого элемента в Map/объекте
- Используй для анаграмм, top K элементов
- Bucket sort: создай массив bucket'ов по частоте

## 2. Two Pointers

**Встречные указатели (left/right)**
- Начни с краев массива: left=0, right=n-1
- Двигай указатели навстречу в зависимости от условия
- Работает на отсортированных массивах и палиндромах

**Быстрый и медленный указатель**
- slow двигается на 1, fast проверяет условие
- slow отвечает за "хорошие" элементы, fast ищет их
- Используй для удаления дубликатов, перемещения нулей

**3Sum/nSum паттерн**
- Отсортируй массив
- Фиксируй первый элемент, для остальных используй two pointers
- Пропускай дубликаты: `if (i > 0 && nums[i] === nums[i-1]) continue`

## 3. Sliding Window

**Fixed size window**
- Посчитай сумму первых k элементов
- Сдвигай окно: добавь новый элемент, убери старый
- Формула: `sum += nums[i] - nums[i-k]`

**Variable size window**
- Расширяй окно правым указателем
- Сжимай окно левым, пока условие нарушено
- Обновляй результат при валидном окне

**Substring problems**
- Используй два Map: need (что нужно) и window (что есть)
- Счетчик valid отслеживает совпадения
- Расширяй right, сжимай left когда valid == need.size

## 4. Stack

**Monotonic Stack**
- Храни элементы в возрастающем/убывающем порядке
- При добавлении элемента удаляй несовместимые из стека
- Используй для "next greater element", температур

**Valid Parentheses**
- Для открывающей скобки - добавь в стек
- Для закрывающей - проверь совпадение с top стека
- В конце стек должен быть пуст

**Evaluate expressions**
- Числа добавляй в стек
- При операторе: возьми два числа, выполни операцию, добавь результат
- Порядок важен: сначала второе число, потом первое

## 5. Binary Search

**На отсортированном массиве**
- Стандартный шаблон: `left=0, right=n-1` или `right=n`
- mid = (left + right) / 2
- Двигай left или right в зависимости от сравнения

**Binary search on answer**
- Ответ лежит в диапазоне [min, max]
- Проверяй, возможен ли ответ mid через функцию canDo(mid)
- Если да - ищи меньше (right=mid), иначе больше (left=mid+1)

**Rotated sorted array**
- Определи, какая половина отсортирована (левая или правая)
- Если target в отсортированной половине - ищи там
- Иначе ищи в другой половине

## 6. Linked List

**Fast & Slow Pointers (Floyd's Cycle)**
- slow идет на 1 шаг, fast на 2 шага
- Если встретились - есть цикл
- Для середины: когда fast дойдет до конца, slow в середине

**Reverse Linked List**
- Храни три указателя: prev, curr, next
- Переворачивай стрелки: `curr.next = prev`
- Двигайся вперед: prev=curr, curr=next

**Dummy node паттерн**
- Создай dummy node перед head
- Упрощает операции с головой списка
- Возвращай dummy.next в конце

## 7. Trees

**DFS (Depth-First Search)**
- Inorder: left -> root -> right (для BST дает sorted)
- Preorder: root -> left -> right (копирование дерева)
- Postorder: left -> right -> root (удаление дерева)
- Рекурсия: база if (!node) return

**BFS (Level-Order)**
- Используй очередь, начни с root
- Обрабатывай уровни: запомни размер очереди
- Добавляй детей в очередь: left, потом right

**Binary Search Tree**
- Левое поддерево < root < правое поддерево
- Для валидации: передавай границы (min, max)
- Для вставки: рекурсивно иди left или right

**Tree height/diameter**
- Height: 1 + max(left_height, right_height)
- Diameter: обновляй максимум left + right на каждом узле
- Используй helper функцию для подсчета высоты

## 8. Graphs

**DFS traversal**
- Рекурсивно: отметь visited, обойди соседей
- Итеративно: используй стек вместо рекурсии
- Для island problems: DFS маркирует всю область

**BFS traversal**
- Используй очередь, отмечай visited при добавлении
- BFS находит кратчайший путь в невзвешенном графе
- Обрабатывай по уровням для shortest path

**Topological Sort**
- Kahn's (BFS): начни с узлов с indegree=0
- Уменьшай indegree соседей, добавляй в очередь при 0
- DFS: после обхода детей добавь узел в результат, reverse в конце

**Union Find**
- find(x): найди корень с path compression
- union(x,y): объедини деревья по rank
- Используй для connected components, cycle detection

**Cycle Detection**
- Undirected: Union Find - если узлы уже в одном компоненте
- Directed: DFS с тремя состояниями (unvisited, visiting, visited)
- visiting set хранит текущий путь

## 9. Dynamic Programming

**1D DP (Fibonacci-like)**
- Состояние зависит от 1-2 предыдущих
- Оптимизация: храни только prev1, prev2 вместо массива
- Переход: `curr = f(prev1, prev2)`

**2D DP (Grid problems)**
- dp[i][j] зависит от dp[i-1][j] и dp[i][j-1]
- Инициализируй первую строку/столбец отдельно
- Идем по всей сетке, заполняем dp

**Knapsack**
- 0/1: dp[i][w] = max(не брать, взять i-й предмет)
- Unbounded: можно использовать элемент многократно
- Coin change: dp[i] = min монет для суммы i

**LCS/LIS**
- LCS: если символы равны dp[i][j] = dp[i-1][j-1] + 1
- Иначе: max(dp[i-1][j], dp[i][j-1])
- LIS: dp[i] = max длина, заканчивающаяся в i

**Kadane's Algorithm**
- Для max subarray: currSum = max(num, currSum + num)
- Обновляй maxSum на каждом шаге
- Для product: храни и max и min (из-за отрицательных)

## 10. Backtracking

**Общий шаблон**
- База рекурсии: когда решение готово, добавь в result
- Перебирай варианты в цикле
- Делай выбор (path.push), рекурсия, отменяй выбор (path.pop)

**Combinations**
- Параметр start предотвращает дубликаты
- backtrack(i+1) - следующий выбор после i
- Для комбинаций с повторениями: backtrack(i) вместо i+1

**Permutations**
- Используй массив used[] для отслеживания
- Перебирай все элементы (start=0 каждый раз)
- Для дубликатов: сортируй + skip если nums[i]==nums[i-1] и !used[i-1]

**Subsets**
- Добавляй path в result на каждом шаге (не только в базе)
- Генерирует все возможные подмножества
- Для дубликатов: skip если i > start && nums[i]==nums[i-1]

**N-Queens/Word Search**
- Используй Set для отслеживания атакуемых позиций
- Маркируй посещенные клетки (board[i][j] = '#')
- Восстанавливай состояние после рекурсии

## 11. Greedy

**Intervals**
- Сортируй по началу или концу
- Merge: если overlap, обновляй конец последнего
- Min удалений: сортируй по концу, отслеживай последний конец

**Meeting Rooms**
- Два указателя: один на начала, второй на концы
- Если start < end текущий конец - нужна новая комната
- Иначе освобождается комната

**Jump Game**
- Отслеживай maxReach = max(maxReach, i + nums[i])
- Если i > maxReach - не можем добраться
- Для min прыжков: обновляй jumps когда достигли currEnd

## 12. Heap (Priority Queue)

**Top K elements**
- Min heap размера k для k largest
- Max heap размера k для k smallest
- Поддерживай размер k: если больше - удали top

**Merge K sorted**
- Добавь первые элементы всех списков в heap
- Бери минимальный, добавляй его next в heap
- Продолжай пока heap не пуст

**Meeting Rooms с heap**
- Heap хранит время окончания встреч
- При новой встрече: если heap.top <= start - освободи комнату
- Добавь конец новой встречи в heap

## 13. Bit Manipulation

**XOR tricks**
- a ^ a = 0, a ^ 0 = a
- Single number: XOR всех элементов
- Missing number: XOR всех индексов и элементов

**Count bits**
- n & (n-1) удаляет самый правый бит 1
- Считай операции до n == 0
- Или используй dp: bits[i] = bits[i>>1] + (i&1)

**Полезные операции**
- Проверить бит: n & (1 << i)
- Установить: n | (1 << i)
- Очистить: n & ~(1 << i)
- Power of 2: n > 0 && (n & (n-1)) == 0

## 14. Trie

**Структура**
- Узел содержит children объект и isEndOfWord флаг
- Insert: создавай узлы по мере необходимости
- Search: проверяй isEndOfWord в конце

**Word Search II**
- Построй Trie из всех слов
- DFS на доске с узлом Trie
- Двигайся по Trie параллельно с доской

## 15. Advanced Structures

**Segment Tree**
- Полное бинарное дерево для range queries
- Лист = элемент массива, узел = f(left_child, right_child)
- Update/Query: O(log n), рекурсивно спускаемся

**Fenwick Tree (BIT)**
- Компактнее Segment Tree для prefix sums
- Update: двигайся вверх добавляя i & -i
- Query: двигайся вниз вычитая i & -i
- Range query: query(r) - query(l-1)

## Когда какой паттерн использовать

**Видишь sorted array** → Binary Search или Two Pointers
**Нужна сумма подмассива** → Prefix Sum или Sliding Window
**Ищешь пары/тройки** → Hash Map или Two Pointers (если sorted)
**Substring/subarray задача** → Sliding Window
**Нужен next greater/smaller** → Monotonic Stack
**Граф/дерево обход** → DFS или BFS
**Оптимальная подструктура** → Dynamic Programming
**Генерация всех комбинаций** → Backtracking
**Выбор по критерию** → Greedy
**Top K элементов** → Heap
**Prefix/word задачи** → Trie
**Single number паттерн** → XOR
**Range queries с updates** → Segment Tree или Fenwick Tree

## Советы по решению

1. **Определи тип задачи** - найди ключевые слова (sorted, substring, all combinations)
2. **Найди паттерн** - похожа ли на задачу, которую уже решал
3. **Начни с brute force** - пойми что нужно, потом оптимизируй
4. **Проверь edge cases** - пустой ввод, один элемент, все одинаковые
5. **Посчитай сложность** - убедись что решение укладывается в ограничения
