# Компонентная архитектура

Практические паттерны композиции React-компонентов, позволяющие управлять ответственностью, переиспользованием и API.

## Container / Presenter

- **Когда использовать**: отделить бизнес-логику (data fetching, state) от отображения, чтобы переиспользовать UI и облегчить тестирование.
- **Плюсы**: переиспользуемые презентационные компоненты, упрощённые снапшот-тесты, возможность менять источник данных без изменений UI.
- **Минусы**: дополнительный слой абстракции, рост количества файлов при малых компонентах.

```tsx
function UserListContainer() {
  const [users, setUsers] = useState<User[]>([]);

  useEffect(() => {
    let cancelled = false;
    fetch('/api/users')
      .then((res) => res.json())
      .then((data) => !cancelled && setUsers(data));
    return () => {
      cancelled = true;
    };
  }, []);

  const handleSelect = useCallback((user: User) => {
    // side effect: tracking / navigation
  }, []);

  return <UserList users={users} onSelect={handleSelect} />;
}

const UserList: React.FC<{ users: User[]; onSelect: (user: User) => void }> = ({ users, onSelect }) => (
  <ul>
    {users.map((user) => (
      <li key={user.id}>
        <button onClick={() => onSelect(user)}>{user.name}</button>
      </li>
    ))}
  </ul>
);
```

## Compound-компоненты

- **Суть**: родительский компонент предоставляет контекст и API, а дочерние элементы (Button, List, Item) читают общие данные через контекст.
- **Когда**: сложные UI вроде модальных окон, меню, табов, где требуется совместное состояние.
- **Плюсы**: декларативное API, единый источник состояния, поддержка произвольного порядка детей.
- **Минусы**: реализация требует дополнительного кода (контекст + проверки), сложности с tree-shaking.

```tsx
const TabsContext = createContext<TabsState | null>(null);

export function Tabs({ value, onChange, children }: TabsProps) {
  const ctx = useMemo(() => ({ value, onChange }), [value, onChange]);
  return <TabsContext.Provider value={ctx}>{children}</TabsContext.Provider>;
}

Tabs.List = function TabsList({ children }: { children: ReactNode }) {
  return <div role="tablist">{children}</div>;
};

Tabs.Trigger = function TabsTrigger({ value, children }: { value: string; children: ReactNode }) {
  const ctx = useContext(TabsContext);
  if (!ctx) throw new Error('Tabs.Trigger must be used within Tabs');
  const active = ctx.value === value;
  return (
    <button
      type="button"
      role="tab"
      aria-selected={active}
      onClick={() => ctx.onChange(value)}
    >
      {children}
    </button>
  );
};
```

## Controlled vs Uncontrolled

- **Контролируемые** компоненты получают значение/обработчик через props, состояние хранится выше.
- **Неконтролируемые** используют `useRef` и браузерное состояние (`defaultValue`, `defaultChecked`).
- **Паттерн**: предоставляйте оба варианта через `value` + `defaultValue`, `onChange` + `onValueChange`.
- **Риски**: смешение контролируемого и неконтролируемого режима приводит к предупреждениям; валидируйте пропсы и синхронизацию.

```tsx
type InputProps = {
  value?: string;
  defaultValue?: string;
  onValueChange?: (value: string) => void;
};

export function TextInput({ value, defaultValue, onValueChange }: InputProps) {
  const [internal, setInternal] = useState(defaultValue ?? '');
  const isControlled = value !== undefined;
  const current = isControlled ? value : internal;

  const handleChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    const next = event.target.value;
    if (!isControlled) setInternal(next);
    onValueChange?.(next);
  };

  return <input value={current} onChange={handleChange} />;
}
```

## Headless / Render Props

- **Идея**: разделить логику и визуализацию — компонент-«голова» отдаёт данные/состояние через render props или контекст.
- **Когда**: нужен полный контроль за версткой, но одинаковая логика (например, доступность, drag-and-drop, формы).
- **Плюсы**: гибкость, возможность использовать любую библиотеку UI.
- **Минусы**: вложенные функции усложняют дерево, требуются мемоизации или `React.memo` для производительности.

```tsx
type ToggleRenderProps = {
  on: boolean;
  toggle: () => void;
};

export function Toggle({ children }: { children: (props: ToggleRenderProps) => ReactNode }) {
  const [on, setOn] = useState(false);
  const toggle = useCallback(() => setOn((value) => !value), []);
  return <>{children({ on, toggle })}</>;
}

// Использование
<Toggle>
  {({ on, toggle }) => (
    <button aria-pressed={on} onClick={toggle}>
      {on ? 'Включено' : 'Выключено'}
    </button>
  )}
</Toggle>;
```

## Layered Providers

- **Проблема**: дерево из множества провайдеров контекста усложняет чтение и тестирование.
- **Решение**: соберите провайдеры в `Providers` компонент и поддерживайте композицию через функции.
- **Советы**: храните инициализацию в модуле, используйте `createContext` с явными дефолтами, разбивайте глобальное состояние по доменам.

```tsx
export function AppProviders({ children }: { children: ReactNode }) {
  return (
    <ThemeProvider>
      <QueryClientProvider client={queryClient}>
        <AuthProvider>
          <FeatureFlagsProvider>{children}</FeatureFlagsProvider>
        </AuthProvider>
      </QueryClientProvider>
    </ThemeProvider>
  );
}
```

- **Лучшие практики**: избегайте «магических» контекстов без описания; документируйте порядок провайдеров; выносите провайдеры, которые нужны не везде, на уровни ниже.
