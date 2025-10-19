// ---------------------------------------------------
// 📌 Дженерик-типы и алиасы
// ---------------------------------------------------
type Box<T> = { value: T };

type Maybe<T> = T | null | undefined;

type NonEmptyArray<T> = [T, ...T[]];


// ---------------------------------------------------
// 📌 Ограничения через extends
// ---------------------------------------------------
type HasId = { id: string | number };

type Repository<T extends HasId> = {
  findById(id: T['id']): T | undefined;
  save(entity: T): T;
};

type WithId<T> = T extends { id: infer U } ? U : never;


// ---------------------------------------------------
// 📌 Параметры по умолчанию
// ---------------------------------------------------
type ApiResult<TData = unknown, TError = string> = {
  data: TData;
  error?: TError;
};

type DefaultResult = ApiResult; // TData = unknown, TError = string
type StringResult = ApiResult<string>; // TData = string


// ---------------------------------------------------
// 📌 Безопасный доступ к свойствам
// ---------------------------------------------------
type PropOf<T, K extends keyof T> = T[K];

type PickKeys<T, K extends keyof T> = {
  [P in K]: T[P];
};

interface User {
  id: string;
  name: string;
  email: string;
}

type UserName = PropOf<User, 'name'>; // string
type UserPreview = PickKeys<User, 'id' | 'name'>;


// ---------------------------------------------------
// 📌 Связанные дженерики и несколько параметров
// ---------------------------------------------------
type PairMap<TKey, TValue> = {
  key: TKey;
  value: TValue;
};

type MapEntry<T extends Record<PropertyKey, unknown>> = {
  [K in keyof T]: PairMap<K, T[K]>;
}[keyof T];

type ApiMap = {
  '/users': { id: string; name: string };
  '/teams': { id: number; title: string };
};

type ApiEntry = MapEntry<ApiMap>;


// ---------------------------------------------------
// 📌 Обобщённые сигнатуры методов
// ---------------------------------------------------
type EventEmitter<TEvents extends Record<string, (...args: any[]) => unknown>> = {
  on<K extends keyof TEvents>(event: K, handler: TEvents[K]): void;
  emit<K extends keyof TEvents>(event: K, ...args: Parameters<TEvents[K]>): void;
};

type AppEvents = {
  login: (user: User) => void;
  logout: () => void;
  error: (reason: string) => void;
};

type AppEventEmitter = EventEmitter<AppEvents>;
