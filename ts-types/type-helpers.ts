// ---------------------------------------------------
// 📌 Проверка равенства типов
// ---------------------------------------------------
type Equal<A, B> = (<T>() => T extends A ? 1 : 2) extends <T>() => T extends B
  ? 1
  : 2
  ? true
  : false;

type Expect<T extends true> = T;

type TestEqual = Expect<Equal<'a', 'a'>>;
// type TestFail = Expect<Equal<'a', 'b'>>; // ошибка


// ---------------------------------------------------
// 📌 Преобразование объединений
// ---------------------------------------------------
type UnionToIntersection<U> = (
  U extends unknown ? (arg: U) => void : never
) extends (arg: infer R) => void
  ? R
  : never;

type Intersection = UnionToIntersection<
  { id: string } | { createdAt: Date }
>; // { id: string } & { createdAt: Date }


// ---------------------------------------------------
// 📌 Поворот объединения в кортеж
// ---------------------------------------------------
type UnionToTuple<T, R extends unknown[] = []> = [T] extends [never]
  ? R
  : UnionToIntersection<
      T extends unknown ? (x: T) => void : never
    > extends (x: infer L) => void
      ? UnionToTuple<Exclude<T, L>, [L, ...R]>
      : never;

type TupleFromUnion = UnionToTuple<'a' | 'b' | 'c'>; // ["c", "b", "a"]


// ---------------------------------------------------
// 📌 Глубокие partial / required
// ---------------------------------------------------
type DeepPartial<T> = T extends object
  ? { [K in keyof T]?: DeepPartial<T[K]> }
  : T;

type DeepRequired<T> = T extends object
  ? { [K in keyof T]-?: DeepRequired<T[K]> }
  : T;

type Settings = {
  theme?: { mode: 'dark' | 'light'; contrast?: number };
  layout?: { sidebar?: { collapsed?: boolean } };
};

type SettingsDraft = DeepPartial<Settings>;
type SettingsStrict = DeepRequired<Settings>;


// ---------------------------------------------------
// 📌 Фильтрация ключей по типу значения
// ---------------------------------------------------
type KeysByValue<T, TValue> = {
  [K in keyof T]-?: T[K] extends TValue ? K : never;
}[keyof T];

type OnlyStrings<T> = Pick<T, KeysByValue<T, string>>;

type Env = {
  API_URL: string;
  PORT: number;
  DEBUG: boolean;
  TOKEN: string;
};

type EnvStrings = OnlyStrings<Env>;


// ---------------------------------------------------
// 📌 Типовые алгоритмы над массивами
// ---------------------------------------------------
type Zip<TA extends unknown[], TB extends unknown[]> = TA extends [
  infer A,
  ...infer ARest
]
  ? TB extends [infer B, ...infer BRest]
    ? [[A, B], ...Zip<ARest, BRest>]
    : []
  : [];

type Reverse<T extends unknown[]> = T extends [infer Head, ...infer Tail]
  ? [...Reverse<Tail>, Head]
  : [];

type Numbers = [1, 2, 3];
type Letters = ['a', 'b', 'c'];

type Zipped = Zip<Numbers, Letters>; // [[1, "a"], [2, "b"], [3, "c"]]
type Reversed = Reverse<Numbers>; // [3, 2, 1]


// ---------------------------------------------------
// 📌 Точное совпадение структур
// ---------------------------------------------------
type Exact<T, Shape> = T extends Shape
  ? Shape extends T
    ? T
    : never
  : never;

type Shape = { id: string; email?: string };

type ValidShape = Exact<{ id: string }, Shape>; // { id: string }
type ValidFullShape = Exact<{ id: string; email?: string }, Shape>; // { id: string; email?: string }
type InvalidShape = Exact<{ id: string; email?: string; extra: boolean }, Shape>; // never


// ---------------------------------------------------
// 📌 Управление выводом типов через NoInfer
// ---------------------------------------------------
type NoInfer<T> = [T][T extends any ? 0 : never];

type SupportedLocale = 'en' | 'ru';

declare function translate<T extends SupportedLocale>(
  key: string,
  locale: T,
  fallback?: NoInfer<T>,
): string;

// translate('title', 'en', 'fr'); // ошибка: 'fr' не выводится как Locale


// ---------------------------------------------------
// 📌 Работа с дискриминированными объединениями
// ---------------------------------------------------
type DomainEvent =
  | { type: 'user:created'; payload: { id: string; email: string } }
  | { type: 'user:deleted'; payload: { id: string; reason?: string } }
  | { type: 'billing:charged'; payload: { invoiceId: string; amount: number } };

type EventOf<TUnion, TType> = TUnion extends { type: TType } ? TUnion : never;
type PayloadOf<TUnion, TType> = EventOf<TUnion, TType> extends {
  payload: infer P;
}
  ? P
  : never;

type DeletedPayload = PayloadOf<DomainEvent, 'user:deleted'>;


// ---------------------------------------------------
// 📌 Преобразование union → record
// ---------------------------------------------------
type UnionToRecord<U extends { type: PropertyKey; payload: unknown }> = {
  [E in U as E['type']]: E['payload'];
};

type EventPayloadMap = UnionToRecord<DomainEvent>;
