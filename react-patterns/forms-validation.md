# Формы и валидация

Паттерны построения форм в React с учётом производительности, валидации, UX и доступности.

## Контролируемые формы и управление состоянием

- **Когда**: небольшие формы с ограниченным количеством полей, когда важна максимальная предсказуемость данных.
- **Плюсы**: лёгкое синхронизирование с другими состояниями (URL, Redux), простая валидация «на лету».
- **Минусы**: при большом числе полей возможны лишние ререндеры; требуется мемоизация обработчиков.

В примере каждое поле хранит значение в React state, а `handleInput` синхронизирует их с объектом формы.

```tsx
type ProfileForm = {
  name: string;
  email: string;
};

function useProfileForm(initial: ProfileForm) {
  const [values, setValues] = useState(initial);

  const handleInput = useCallback(
    (event: React.ChangeEvent<HTMLInputElement>) => {
      const { name, value } = event.target;
      setValues((prev) => ({ ...prev, [name]: value }));
    },
    [],
  );

  return { values, handleInput };
}
```

## Неконтролируемые формы и `FormData`

- **Когда**: формы с множеством полей, богатая DOM-структура или интеграция с сторонними библиотеками (например, `<input type="file">`).
- **Плюсы**: минимальная нагрузка на React-дерево, браузер управляет state.
- **Минусы**: сложнее валидировать динамически; требуется работа с `ref` и `FormData`.

```tsx
const formRef = useRef<HTMLFormElement>(null);

function handleSubmit(event: React.FormEvent<HTMLFormElement>) {
  event.preventDefault();
  const data = new FormData(event.currentTarget);
  const payload = Object.fromEntries(data.entries());
  mutate(payload); // отправка на сервер
}
```

## React Hook Form и управляемые поля

- **Преимущества**: подписка на отдельные поля, минимальные ререндеры, удобная интеграция с UI-библиотеками.
- **Практика**: используйте `Controller` для компонентов без `ref`, `useFieldArray` для динамических коллекций, `FormProvider` для доступа к методам.

```tsx
import { FormProvider, useForm, Controller } from 'react-hook-form';

type SignupForm = {
  email: string;
  password: string;
};

function Signup() {
  const form = useForm<SignupForm>({
    defaultValues: { email: '', password: '' },
    mode: 'onBlur',
  });

  const onSubmit = form.handleSubmit(async (values) => {
    await signup(values);
    toast.success('Готово!');
  });

  return (
    <FormProvider {...form}>
      <form onSubmit={onSubmit} noValidate>
        <label>
          Email
          <input type="email" {...form.register('email', { required: 'Укажите email' })} />
          <FieldError name="email" />
        </label>

        <label>
          Пароль
          <Controller
            name="password"
            control={form.control}
            rules={{ minLength: { value: 8, message: 'Минимум 8 символов' } }}
            render={({ field }) => <PasswordInput {...field} />}
          />
          <FieldError name="password" />
        </label>

        <button type="submit" disabled={form.formState.isSubmitting}>
          Создать аккаунт
        </button>
      </form>
    </FormProvider>
  );
}
```

## Схемы валидации (Zod/Yup) и типобезопасность

- **Паттерн**: описывайте структуру и правила валидации единожды в схеме, используйте их для типизации формы и серверных контрактов.
- **Инструменты**: Zod (`z.object`), Yup, Valibot; `@hookform/resolvers` для интеграции с React Hook Form.
- **Советы**: расшаривайте схему между фронтом и бэком, используйте `transform` для приведения типов.

```tsx
import { z } from 'zod';
import { zodResolver } from '@hookform/resolvers/zod';

const profileSchema = z.object({
  name: z.string().min(1, 'Введите имя'),
  age: z.coerce.number().int().min(18, 'Возраст 18+'),
});

type ProfileInput = z.infer<typeof profileSchema>;

const form = useForm<ProfileInput>({
  defaultValues: { name: '', age: 18 },
  resolver: zodResolver(profileSchema),
});
```

## Оптимистичные сабмиты и состояние отправки

- **Цель**: моментально обновить UI, пока сервер подтверждает изменения (создание сущности, отметка like).
- **Инструменты**: React Query `useMutation`, локальный state с `setOptimisticUpdates`, `startTransition`.
- **Риски**: несоответствие данных при ошибке; добавляйте откат (`onError` → `queryClient.setQueryData(previous)`).

```tsx
const mutation = useMutation({
  mutationFn: updateProfile,
  onMutate: async (payload) => {
    await queryClient.cancelQueries({ queryKey: ['profile'] });
    const prev = queryClient.getQueryData<Profile>(['profile']);
    queryClient.setQueryData(['profile'], (data) => ({ ...data, ...payload }));
    return { prev };
  },
  onError: (_error, _variables, context) => {
    if (context?.prev) {
      queryClient.setQueryData(['profile'], context.prev);
    }
  },
  onSettled: () => {
    queryClient.invalidateQueries({ queryKey: ['profile'] });
  },
});
```

## Доступность и UX форм

- **Лучшие практики**: связывайте `<label>` и `<input>` через `htmlFor`, используйте `aria-describedby` для ошибок, показывайте подсказки до сабмита.
- **Паттерны**: блокируйте кнопки на время отправки, отображайте прогресс (`aria-busy`), сохраняйте введённые значения при ошибках сервера.
- **Инструменты**: `react-aria`, `@radix-ui/react-label`, Storybook accessibility addons.

```tsx
function FieldError({ name }: { name: string }) {
  const {
    formState: { errors },
  } = useFormContext();
  const message = errors[name]?.message;
  if (!message) return null;
  return (
    <p role="alert" id={`${name}-error`} className="field-error">
      {message}
    </p>
  );
}
```
