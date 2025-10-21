# Установка и пользовательский опыт

## Installation prompts
- `beforeinstallprompt` событие (Chrome/Edge/Android):
```js
window.addEventListener('beforeinstallprompt', event => {
  event.preventDefault();
  showInstallButton(event);
});
```
- При клике: `prompt()` → `userChoice`.
- Safari iOS: нет `beforeinstallprompt`, инструкция "Add to Home Screen".

## Display modes
- Manifest `display`: `standalone` (без адресной строки), `fullscreen`, `minimal-ui`.
- Add CSS для `:fullscreen` если `display=fullscreen`.

## Launch screen
- `background_color`, `theme_color` в manifest → splash screen (Android/Chrome).
- iOS: `<link rel="apple-touch-startup-image" ...>` (размеры).

## Icons
- Минимум 192x192 и 512x512. Для maskable используйте `purpose: maskable`.
- Apple Touch Icons: `<link rel="apple-touch-icon" href="/icons/apple-touch-icon.png">`.

## App-like UX
- Skeleton screen при startup.
- Offline indicator, manual refresh.
- Shortcut actions (manifest shortcuts). На Android (Chrome).

## Integrations
- Web Share API (`navigator.share`) — пользовательская share sheet.
- Web Share Target — manifest `share_target` (принимает данные).
- Badges API (`navigator.setAppBadge`) — уведомления.

## Constraints
- iOS: 50MB limit для storage, SW работает только 30 сек после закрытия.
- Chrome Desktop: install icon в адресной строке, manifest display=standalone.

