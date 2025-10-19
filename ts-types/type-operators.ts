// ---------------------------------------------------
// 📌 typeof для переноса значений в типы
// ---------------------------------------------------
const routes = {
  home: '/',
  profile: '/users/:id',
  settings: '/settings',
} as const;

type RouteName = keyof typeof routes; // "home" | "profile" | "settings"
type RoutePath = (typeof routes)[RouteName]; // "/" | "/users/:id" | "/settings"


// ---------------------------------------------------
// 📌 keyof и индексный доступ
// ---------------------------------------------------
interface ApiResponse<TData> {
  data: TData;
  status: number;
  error?: string;
}

type ResponseKeys = keyof ApiResponse<unknown>; // "data" | "status" | "error"
type ResponseData<T> = ApiResponse<T>['data'];
type HttpStatus = ApiResponse<unknown>['status']; // number


// ---------------------------------------------------
// 📌 Перебор ключей с помощью in
// ---------------------------------------------------
type FlagRecord<T extends string> = {
  [K in T]: boolean;
};

type FeatureFlag = FlagRecord<'beta' | 'darkMode' | 'analytics'>;


// ---------------------------------------------------
// 📌 Согласование литералов с satisfies
// ---------------------------------------------------
const formSchema = {
  username: { required: true, minLength: 3 },
  email: { required: true, pattern: /@/ },
  newsletter: { required: false },
} satisfies Record<
  string,
  { required: boolean; minLength?: number; pattern?: RegExp }
>;

type FieldName = keyof typeof formSchema; // "username" | "email" | "newsletter"


// ---------------------------------------------------
// 📌 Lookup-типы для связывания коллекций
// ---------------------------------------------------
interface EntityMap {
  user: { id: string; name: string };
  team: { id: number; title: string };
  invoice: { id: string; total: number };
}

type EntityName = keyof EntityMap;
type EntityByName<K extends EntityName> = EntityMap[K];

type Invoice = EntityByName<'invoice'>;


// ---------------------------------------------------
// 📌 Типы для ключей и значений одновременно
// ---------------------------------------------------
type KeyValuePairs<T> = {
  [K in keyof T]: { key: K; value: T[K] };
}[keyof T];

type RoutePair = KeyValuePairs<typeof routes>;
