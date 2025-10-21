# Security для фронтенда

Практические советы по защите веб-приложений на клиентской стороне: предотвращение XSS/CSRF, безопасное хранение данных, настройка CSP, уязвимости supply chain и защита рантайма.

### 🧠 [Основы и модель угроз](./basics-threat-model.md)

- Defense in depth, least privilege, zero trust
- Основные угрозы (XSS, CSRF, supply chain)
- Threat modeling: assets, entry points, mitigation
- Security headers и политика

### ✋ [XSS, CSP и Trusted Types](./xss-csp.md)

- Reflected/Stored/DOM XSS, sanitization
- CSP (nonce/hash), Subresource Integrity
- Trusted Types и DOMPurify
- Учитывайте особенности framework (React/Vue)

### 🎯 [CSRF, clickjacking и session](./csrf-clickjacking.md)

- CSRF токены, SameSite cookie
- Clickjacking защита (X-Frame-Options)
- Session fixation и login security
- CORS и anti-automation

### 🔐 [Cookies, storage и токены](./cookies-storage.md)

- Secure/HttpOnly/SameSite cookie
- LocalStorage, IndexedDB, риски
- JWT storage лучшие практики
- Secret management и storage events

### 🔄 [Supply chain и зависимости](./supply-chain.md)

- Проверка npm пакетов, SCA
- CI/CD security, scripts
- CDN/SRI, consent management
- SBOM, приватные registry

### 🛡️ [Runtime защита и мониторинг](./runtime-monitoring.md)

- CSP reporting, Trusted Types
- Error reporting (Sentry)
- Tamper detection, RUM
- postMessage/iframe security

### ✅ [Чек-лист и best practices](./best-practices.md)

- XSS/CSRF/CSP чек-лист
- Storage и токены
- Third-party scripts
- Deployment и регулярный аудит
