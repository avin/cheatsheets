// ---------------------------------------------------
// 📌 Partial / Required / Readonly
// ---------------------------------------------------
interface Profile {
  id: string;
  name: string;
  email?: string;
}

type DraftProfile = Partial<Profile>;
type StrictProfile = Required<Profile>;
type FrozenProfile = Readonly<Profile>;


// ---------------------------------------------------
// 📌 Pick / Omit / Record
// ---------------------------------------------------
type ProfilePreview = Pick<Profile, 'id' | 'name'>;
type ProfileOptions = Omit<Profile, 'id'>;

type FeatureFlags = Record<'beta' | 'darkMode' | 'analytics', boolean>;


// ---------------------------------------------------
// 📌 Exclude / Extract / NonNullable
// ---------------------------------------------------
type Events = 'click' | 'hover' | 'focus';
type BlockedEvents = 'hover';

type AllowedEvents = Exclude<Events, BlockedEvents>; // "click" | "focus"
type HoverOnly = Extract<Events, 'hover' | 'scroll'>; // "hover"

type MaybeString = string | undefined | null;
type StrictString = NonNullable<MaybeString>; // string


// ---------------------------------------------------
// 📌 ReturnType / Parameters / ConstructorParameters
// ---------------------------------------------------
type AsyncFn = (id: string) => Promise<Profile>;

type AsyncResult = Awaited<ReturnType<AsyncFn>>; // Profile
type AsyncParams = Parameters<AsyncFn>; // [id: string]

class Service {
  constructor(public readonly url: string, private token: string) {}
}

type ServiceArgs = ConstructorParameters<typeof Service>; // [url: string, token: string]
type ServiceInstance = InstanceType<typeof Service>;


// ---------------------------------------------------
// 📌 ThisType и контекст
// ---------------------------------------------------
type Vector2D = {
  x: number;
  y: number;
};

type VectorBuilder = {
  point(x: number, y: number): void;
} & ThisType<{ result: Vector2D[] }>;

const builders: Record<string, VectorBuilder> = {
  grid: {
    point(x, y) {
      this.result.push({ x, y });
    },
  },
};


// ---------------------------------------------------
// 📌 Частичные, обязательные и изменяемые поля по ключам
// ---------------------------------------------------
type PartialBy<T, K extends keyof T> = Omit<T, K> & Partial<Pick<T, K>>;
type RequiredBy<T, K extends keyof T> = Omit<T, K> & Required<Pick<T, K>>;
type MutableBy<T, K extends keyof T> = Omit<T, K> & {
  -readonly [P in K]: T[P];
};

type ProfilePatch = PartialBy<Profile, 'name' | 'email'>;
type ProfileIdentified = RequiredBy<Profile, 'name'>;
type EditableProfile = MutableBy<FrozenProfile, 'name'>;


// ---------------------------------------------------
// 📌 Выделение writable/readonly ключей
// ---------------------------------------------------
type WritableKeys<T> = {
  [K in keyof T]-?: { [Q in K]: T[K] } extends {
    -readonly [Q in K]: T[K];
  }
    ? K
    : never;
}[keyof T];

type ReadonlyKeys<T> = {
  [K in keyof T]-?: { [Q in K]: T[K] } extends {
    readonly [Q in K]: T[K];
  }
    ? K
    : never;
}[keyof T];

interface WidgetOptions {
  readonly id: string;
  label: string;
  readonly createdAt: Date;
}

type EditableWidgetFields = Pick<WidgetOptions, WritableKeys<WidgetOptions>>;
type LockedWidgetFields = Pick<WidgetOptions, ReadonlyKeys<WidgetOptions>>;


// ---------------------------------------------------
// 📌 Сигнатуры методов и работа с this
// ---------------------------------------------------
type ContextualMethod = (this: { url: string }, path: string) => Promise<void>;

type MethodContext = ThisParameterType<ContextualMethod>; // { url: string }
type MethodArgs = Parameters<ContextualMethod>; // [path: string]
type BoundMethod = OmitThisParameter<ContextualMethod>; // (path: string) => Promise<void>


// ---------------------------------------------------
// 📌 Конструирование карт обработчиков
// ---------------------------------------------------
type DomainEventMap = {
  'user:created': { id: string; email: string };
  'user:deleted': { id: string; reason?: string };
  'billing:charged': { invoiceId: string; amount: number };
};

type EventHandlerMap<TEvents extends Record<string, unknown>> = {
  [K in keyof TEvents]: (payload: TEvents[K]) => void;
};

type EventHandlers = EventHandlerMap<DomainEventMap>;
