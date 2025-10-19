// ---------------------------------------------------
// 📌 Базовые условные типы
// ---------------------------------------------------
type Primitive = string | number | boolean | symbol | bigint | null | undefined;

type IsPrimitive<T> = T extends Primitive ? true : false;

type Example1 = IsPrimitive<string>; // true
type Example2 = IsPrimitive<Date>; // false


// ---------------------------------------------------
// 📌 Дистрибутивность по объединениям
// ---------------------------------------------------
type ToArray<T> = T extends unknown ? T[] : never;

type Result = ToArray<'a' | 'b'>; // ('a' | 'b')[]

type NonDistributive<T> = [T] extends [unknown] ? T[] : never;

type NonDistributiveResult = NonDistributive<'a' | 'b'>; // ('a' | 'b')[]


// ---------------------------------------------------
// 📌 Извлечение типов через infer
// ---------------------------------------------------
type AwaitedValue<T> = T extends Promise<infer U> ? AwaitedValue<U> : T;

type AsyncResult = AwaitedValue<Promise<Promise<number>>>; // number

type ExtractSecond<T> = T extends [any, infer Second, ...any[]] ? Second : never;

type SecondOfTuple = ExtractSecond<[string, number, boolean]>; // number


// ---------------------------------------------------
// 📌 Нормализация never и вспомогательные типы
// ---------------------------------------------------
type IsNever<T> = [T] extends [never] ? true : false;

type NonNullableDeep<T> = T extends object
  ? { [K in keyof T]: NonNullableDeep<NonNullable<T[K]>> }
  : NonNullable<T>;

type Demo = NonNullableDeep<{ value: string | null; nested?: { count?: number } }>;


// ---------------------------------------------------
// 📌 Выбор конкретных веток
// ---------------------------------------------------
type ExtractEvent<TEvents, TName> = TEvents extends {
  name: infer Name;
  payload: infer Payload;
}
  ? Name extends TName
    ? Payload
    : never
  : never;

type Events =
  | { name: 'login'; payload: { userId: string } }
  | { name: 'logout'; payload: null }
  | { name: 'error'; payload: { reason: string } };

type LogoutPayload = ExtractEvent<Events, 'logout'>; // null


// ---------------------------------------------------
// 📌 Объединение с mapped types
// ---------------------------------------------------
type PickByValue<T, TValue> = {
  [K in keyof T as T[K] extends TValue ? K : never]: T[K];
};

type Account = {
  id: string;
  createdAt: Date;
  active: boolean;
  tags: string[];
};

type AccountDates = PickByValue<Account, Date>;
