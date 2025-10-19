// ---------------------------------------------------
// 📌 Базовый перебор ключей
// ---------------------------------------------------
type Optionalize<T> = {
  [K in keyof T]?: T[K];
};

type Readonlyify<T> = {
  readonly [K in keyof T]: T[K];
};

interface Account {
  id: string;
  email: string;
  verified: boolean;
}

type AccountDraft = Optionalize<Account>;
type AccountReadonly = Readonlyify<Account>;


// ---------------------------------------------------
// 📌 Управление модификаторами
// ---------------------------------------------------
type Mutable<T> = {
  -readonly [K in keyof T]: T[K];
};

type RequiredKeys<T> = {
  [K in keyof T]-?: T[K];
};

type RequiredAccount = RequiredKeys<AccountDraft>;


// ---------------------------------------------------
// 📌 Ремапинг ключей и фильтрация
// ---------------------------------------------------
type OnlyBooleans<T> = {
  [K in keyof T as T[K] extends boolean ? K : never]: T[K];
};

type AccountFlags = OnlyBooleans<Account>;


// ---------------------------------------------------
// 📌 Преобразование ключей
// ---------------------------------------------------
type PrefixKeys<T, Prefix extends string> = {
  [K in keyof T as `${Prefix}${Capitalize<string & K>}`]: T[K];
};

type AccountApiShape = PrefixKeys<Account, 'get'>;


// ---------------------------------------------------
// 📌 Глубокие преобразования
// ---------------------------------------------------
type DeepReadonly<T> = T extends Function | symbol
  ? T
  : T extends object
    ? { readonly [K in keyof T]: DeepReadonly<T[K]> }
    : T;

type DeepPartial<T> = T extends object
  ? { [K in keyof T]?: DeepPartial<T[K]> }
  : T;

type Settings = {
  theme: { primary: string; dark: boolean };
  layout: { sidebar: { collapsed: boolean } };
};

type SettingsImmutable = DeepReadonly<Settings>;
type SettingsDraft = DeepPartial<Settings>;


// ---------------------------------------------------
// 📌 Комбинирование с условными типами
// ---------------------------------------------------
type NullableFields<T> = {
  [K in keyof T]: T[K] | null;
};

type StrictNullable<T> = {
  [K in keyof T]: null extends T[K] ? T[K] : T[K] | null;
};

type AccountNullable = NullableFields<Account>;
type AccountStrictNullable = StrictNullable<Account>;
