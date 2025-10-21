# Паттерны хуков и состояния

Как структурировать состояние в React-приложениях с помощью хуков, контекста и внешних стораджей.

## Кастомный хук как модуль

- **Суть**: инкапсулировать логику и побочные эффекты в пользовательский хук, возвращающий API из значений и функций.
- **Когда**: повторяющаяся логика (формы, таймеры, синхронизация), требуется переиспользование вне компонента.
- **Плюсы**: композиция, тестирование как функции, единая точка настройки.
- **Минусы**: без мемоизации значения/функции пересоздаются, что вызывает переработку дочерних компонентов.

```tsx
function useModal(initial = false) {
  const [open, setOpen] = useState(initial);
  const openModal = useCallback(() => setOpen(true), []);
  const closeModal = useCallback(() => setOpen(false), []);
  const toggleModal = useCallback(() => setOpen((state) => !state), []);

  return useMemo(
    () => ({ open, openModal, closeModal, toggleModal }),
    [open, openModal, closeModal, toggleModal],
  );
}

const ModalExample = () => {
  const modal = useModal();
  return (
    <>
      <button onClick={modal.openModal}>Открыть</button>
      {modal.open && <Dialog onClose={modal.closeModal}>Содержимое</Dialog>}
    </>
  );
};
```

## useReducer + Context

- **Когда**: сложное состояние с множеством действий, требуются чистые редьюсеры и диспетчеры.
- **Плюсы**: предсказуемость, easy тестирование редьюсера, DevTools/TODO logging.
- **Минусы**: без мемоизации селекторов контекста весь потребительский компонент перерендерится.

```tsx
type State = { items: CartItem[] };
type Action = { type: 'add'; item: CartItem } | { type: 'remove'; id: string };

const CartContext = createContext<{ state: State; dispatch: Dispatch<Action> } | null>(null);

function cartReducer(state: State, action: Action): State {
  switch (action.type) {
    case 'add':
      return { items: [...state.items, action.item] };
    case 'remove':
      return { items: state.items.filter((item) => item.id !== action.id) };
    default:
      return state;
  }
}

export function CartProvider({ children }: { children: React.ReactNode }) {
  const [state, dispatch] = useReducer(cartReducer, { items: [] });
  const value = useMemo(() => ({ state, dispatch }), [state]);
  return <CartContext.Provider value={value}>{children}</CartContext.Provider>;
}
```

## Контекст-селекторы

- **Проблема**: потребители контекста ререндерятся при каждом изменении value.
- **Решение**: использовать разделение контекста или селекторы (`use-context-selector`, Zustand selectors).
- **Плюсы**: перерисовываются только нужные части UI, проще оптимизировать большие таблицы/списки.
- **Минусы**: добавляется зависимость или необходимость вручную писать селекторы.

```tsx
const Context = createContext<StateStore | null>(null);

export function useCartSelector<T>(selector: (state: State) => T): T {
  const store = useContext(Context);
  if (!store) throw new Error('useCartSelector must be used within provider');
  return selector(store.getState());
}
```

```tsx
// Zustand пример
const useCartStore = create<CartState>()((set, get) => ({
  items: [],
  add: (item) => set({ items: [...get().items, item] }),
}));

const Count = () => {
  const count = useCartStore((state) => state.items.length);
  return <span>{count}</span>;
};
```

## useRef и Imperative Handle

- **Назначение**: хранить изменяемые значения, не вызывающие ререндер (таймеры, DOM-ссылки, кэш).
- **Паттерн**: создавайте API с `forwardRef` + `useImperativeHandle`, когда нужно дать родителю управлять дочерним компонентом (фокус, прокрутка).
- **Риски**: чрезмерное использование imperatively ломает декларативность; документируйте предоставляемые методы.

```tsx
type InputHandle = {
  focus: () => void;
};

export const FocusInput = forwardRef<InputHandle, JSX.IntrinsicElements['input']>(
  function FocusInput(props, ref) {
    const inputRef = useRef<HTMLInputElement>(null);

    useImperativeHandle(ref, () => ({
      focus() {
        inputRef.current?.focus();
      },
    }));

    return <input ref={inputRef} {...props} />;
  },
);
```

## Синхронизация состояния с URL

- **Зачем**: поддержать shareable ссылки, SSR и персистентность фильтров/сортировок.
- **Инструменты**: `useSearchParams` (React Router), `useSyncExternalStore` + собственный history listener, Zod для валидации.
- **Советы**: держите преобразование типов в одном месте, debounce обновления history, различайте `replace` и `push`.

```tsx
function useFilterState() {
  const [params, setParams] = useSearchParams();
  const filter = params.get('filter') ?? 'all';

  const setFilter = useCallback(
    (next: string) => {
      const nextParams = new URLSearchParams(params);
      nextParams.set('filter', next);
      setParams(nextParams, { replace: true });
    },
    [params, setParams],
  );

  return { filter, setFilter };
}
```
