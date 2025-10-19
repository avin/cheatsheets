// ---------------------------------------------------
// 📌 Комбинация строковых литералов
// ---------------------------------------------------
type Language = 'en' | 'ru' | 'es';
type Namespace = 'common' | 'auth' | 'dashboard';

type TranslationKey = `${Language}:${Namespace}`;

type ExampleKey = TranslationKey; // "en:common" | "en:auth" | ...


// ---------------------------------------------------
// 📌 Изменение регистра встроенными утилитами
// ---------------------------------------------------
type EventName<T extends string> = `on${Capitalize<T>}`;

type ButtonEvent = EventName<'click'>; // "onClick"

type ScreamingSnake<T extends string> = Uppercase<
  T extends `${infer Head}-${infer Tail}` ? `${Head}_${ScreamingSnake<Tail>}` : T
>;

type Route = ScreamingSnake<'admin-panel-settings'>; // "ADMIN_PANEL_SETTINGS"


// ---------------------------------------------------
// 📌 Импорт ключей из объекта
// ---------------------------------------------------
const statuses = {
  draft: 0,
  published: 1,
  archived: 2,
} as const;

type StatusName = keyof typeof statuses;
type StatusLabel = `status:${StatusName}`;


// ---------------------------------------------------
// 📌 Парсинг строк с infer
// ---------------------------------------------------
type Split<
  T extends string,
  Delimiter extends string,
> = T extends `${infer Head}${Delimiter}${infer Tail}`
  ? [Head, ...Split<Tail, Delimiter>]
  : T extends '' ? [] : [T];

type Segments = Split<'user/profile/edit', '/'>; // ["user", "profile", "edit"]


// ---------------------------------------------------
// 📌 Генерация путей и событий
// ---------------------------------------------------
type Resource = 'user' | 'team' | 'invoice';
type Action = 'created' | 'updated' | 'deleted';

type EventTopic = `${Resource}.${Action}`;

type ApiRoute<ID extends string | number> = `/${Resource}/${ID}`;

type SampleRoute = ApiRoute<':id'>; // "/user/:id" | "/team/:id" | "/invoice/:id"
