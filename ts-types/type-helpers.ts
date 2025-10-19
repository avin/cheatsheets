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
