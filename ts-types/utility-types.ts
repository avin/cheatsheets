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
