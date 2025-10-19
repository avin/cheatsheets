# Полезные сокращения и прочее

**API** — Application Programming Interface: контракт, по которому взаимодействуют сервисы/клиенты. Может быть REST, GraphQL, gRPC.

**REST** — Representational State Transfer: стиль API с ресурсами и HTTP-методами (GET/POST/PUT/DELETE), где состояние передаётся через представления.

**RPC** — Remote Procedure Call: вызов удалённой функции как локальной (JSON-RPC, gRPC). Удобно для машин-машин взаимодействия.

**gRPC** — фреймворк RPC поверх HTTP/2 + Protocol Buffers. Обеспечивает строгую типизацию и двунаправленный стриминг, популярен в микросервисах.

**JSON** — JavaScript Object Notation, лёгкий формат обмена данными (в веб/API).

**YAML** — YAML Ain't Markup Language: удобен для конфигураций (Kubernetes, CI), но требует аккуратности с отступами.

**XML** — Extensible Markup Language: кастомные структуры, часто используется в legacy-интеграциях и документах.

**FQDN** — Fully Qualified Domain Name (например, api.example.com). Используется для DNS-записей.

**DNS** — Domain Name System: сопоставляет доменные имена IP-адресам. Базовый сервис интернет-инфраструктуры.

**JWT** — JSON Web Token: компактный формат токена с подписью; несёт клеймы о пользователе, используется в авторизации.

**OAuth** — протокол авторизации, позволяющий приложениям запрашивать доступ к ресурсам ("Login with Google").

**OIDC** — OpenID Connect: слой аутентификации поверх OAuth 2.0 (ID-токен с информацией о пользователе).

**IaaS/PaaS/SaaS/XaaS** — модели облачных сервисов (инфраструктура, платформа, ПО, всё как сервис). Помогают понимать зоны ответственности.

**SOP** — Standard Operating Procedure: стандартизированная инструкция, как выполнять повторяющиеся задачи.

**RFC** — Request for Comments (IETF) — официальные документы спецификаций интернет-протоколов. RFC2119 описывает ключевые слова MUST/SHOULD/MAY.

**FOSS/FLOSS** — (Free/Libre) Open Source Software: программное обеспечение с открытым исходным кодом и свободной лицензией.

**CLI** — Command-Line Interface (например, `kubectl`, `git`). Предпочитается для автоматизации.

**GUI** — Graphical User Interface: графический интерфейс (VS Code, Photoshop).

**SDK** — Software Development Kit: набор инструментов (библиотеки, примеры, документация) для разработки под конкретную платформу.

**PWA** — Progressive Web App: веб-приложение с возможностями нативных (офлайн, push), работает через Service Worker.

**CDN** — Content Delivery Network: сеть серверов для быстрой доставки статического контента пользователям (Cloudflare, Akamai).

**WAF** — Web Application Firewall: фильтрует HTTP-трафик, защищает от OWASP Top 10/ботов (Cloudflare WAF, AWS WAF).
