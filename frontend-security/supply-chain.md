# Supply chain и зависимости

## Пакеты
- Проверяйте npm packages (maintainers, downloads, `npm audit`/`yarn audit`).
- Pin версий (package-lock, yarn.lock). Используйте `npm ci`.
- SCA (Software Composition Analysis): Snyk, Dependabot, Renovate.
- Подписывайте пакеты (npm `npm audit signatures`, Sigstore).

## Build chain
- Билд системы (Webpack/Vite) → проверяйте плугины.
- Скрипты: не запускайте `npm install` без review (postinstall scripts).
- .npmrc: `ignore-scripts=true` если возможно.
- npm `fund` — но проверка (не гарантия).

## CDN & third-party
- Используйте Subresource Integrity (SRI) для скриптов/стилей.
- Зафиксируйте версию CDN (`?version=...`).
- CSP `script-src` ограничивает источники.
- Избегайте третьих скриптов, загружайте async/defer.

## Faʻcebook Pixel etc
- Оборачивайте third-party через Consent Management (GDPR, ePrivacy).
- Переключайте/удаляйте при отказе пользователя.

## Supply chain атаки
- compromises: Equifax, event-stream.
- Защита: private registry (Verdaccio, Artifactory), review updates.
- OSS Sigstore/Notary — подписи.

## CI/CD security
- Secrets в CI (GitHub Actions Secrets, GitLab CI variables).
- least privilege для деплой ключей.
- Rotating tokens/API keys.
- SBOM (Software Bill of Materials) — генерация (CycloneDX, SPDX).

