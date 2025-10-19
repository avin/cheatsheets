# UI и клиентские паттерны

## MVC / MVP / MVVM
- **MVC**: Controller управляет View; классика для серверных веб-фреймворков.
- **MVP**: Presenter ведёт View, удобно для desktop/mobile (Android pre-Architecture Components).
- **MVVM**: ViewModel связывает данные и View через binding (WPF, SwiftUI, Jetpack Compose).

## Flux / Redux / Unidirectional Data Flow
- **Суть**: однонаправленный поток данных → actions → reducers → store → view.
- **Использование**: React, frontend с богатым состоянием.

## Observer / Pub-Sub
- **Суть**: подписчики реагируют на события. Применяется в UI фреймворках, event buses.

## Reactive programming (Rx)
- **Суть**: асинхронные потоки событий + операторы. RxJS, Project Reactor, Combine.
- **Когда**: сложные UI, realtime обновления, асинхронные цепочки.

## Progressive Web App (PWA)
- **Суть**: веб-приложение, которое ощущается как нативное (офлайн, push, installable).
- **Компоненты**: Service Worker, Web App Manifest, HTTPS.

## Micro Frontends
- **Суть**: большие UI разбиваются на независимые фрагменты (команды деплоят отдельно).
- **Плюсы**: масштабирование команд, независимые release.
- **Минусы**: сложность интеграции, размер бандла.

## Atomic Design
- **Суть**: дизайн-системы из атомов/молекул/организмов. Помогает создавать повторно используемые UI-компоненты.

## Responsive Design / Mobile-first
- **Суть**: UI адаптируется к размерам экрана, подход mobile-first учитывает смартфоны как primary устройство.

## Accessibility (a11y)
- **Суть**: обеспечение доступности UI для людей с ограниченными возможностями (ARIA, контраст, клавиатурная навигация).
