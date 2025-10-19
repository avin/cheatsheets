// ---------------------------------------------------
// 📌 type vs interface, объединения и пересечения
// ---------------------------------------------------
type UserId = string | number;

interface UserProfile {
  id: UserId;
  name: string;
}

type WithTimestamps = { createdAt: Date; updatedAt: Date };

type DetailedUser = UserProfile & WithTimestamps;

// interface допускает декларативное расширение
interface UserProfile {
  email?: string;
}


// ---------------------------------------------------
// 📌 Литеральные типы и строго типизированные объекты
// ---------------------------------------------------
type Theme = 'light' | 'dark' | 'system';

const defaultTheme = 'light' as const; // тип 'light'

const settings = {
  theme: defaultTheme,
  pageSize: 20,
} as const; // все поля становятся readonly и literal

type Settings = typeof settings; // { readonly theme: 'light'; readonly pageSize: 20 }


// ---------------------------------------------------
// 📌 Readonly, optional и обязательные поля
// ---------------------------------------------------
interface ApiConfig {
  readonly baseUrl: string;
  timeoutMs?: number;
}

const apiConfig: ApiConfig = { baseUrl: 'https://api.example.com' };

// apiConfig.baseUrl = '' // ошибка: readonly


// ---------------------------------------------------
// 📌 Кортежи с именованными позициями
// ---------------------------------------------------
type Point = [x: number, y: number, z?: number];

type HttpEntry = readonly [
  method: 'GET' | 'POST',
  url: string,
  status: number,
];


// ---------------------------------------------------
// 📌 Типы never и unknown
// ---------------------------------------------------
type JsonValue =
  | string
  | number
  | boolean
  | null
  | { [key: string]: JsonValue }
  | JsonValue[];

type AnyFunction = (...args: unknown[]) => unknown;

type EnsureNever<T> = [T] extends [never] ? true : false;

type EnsureJson<T> = T extends JsonValue ? T : never;

type LocaleExhaustiveCheck = EnsureNever<Exclude<'en' | 'ru', Locale>>; // true


// ---------------------------------------------------
// 📌 Уникальные символы для номинальной типизации
// ---------------------------------------------------
declare const userIdBrand: unique symbol;

type BrandedId<T extends string> = string & { readonly [userIdBrand]: T };

type InvoiceId = BrandedId<'Invoice'>;
type CustomerId = BrandedId<'Customer'>;


// ---------------------------------------------------
// 📌 satisfies для согласования структур
// ---------------------------------------------------
const locales = {
  en: { title: 'Dashboard', shortcut: '⌘D' },
  ru: { title: 'Панель', shortcut: 'Ctrl+D' },
} satisfies Record<string, { title: string; shortcut: string }>;

type Locale = keyof typeof locales; // "en" | "ru"


// ---------------------------------------------------
// 📌 Аннотации сигнатур и типы функций
// ---------------------------------------------------
type AsyncFn<TArgs extends unknown[], TResult> = (
  ...args: TArgs
) => Promise<TResult>;

type FetchUser = AsyncFn<[id: UserId], DetailedUser>;
