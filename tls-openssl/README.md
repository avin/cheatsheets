# TLS / SSL / OpenSSL шпаргалка

Практические заметки о работе с TLS: базовая теория, инструменты OpenSSL, выпуск и настройка сертификатов, аудит и безопасные практики.

### 🔐 [Основы TLS](./tls-basics.md)

- Handshake и ciphersuites — версии TLS, алгоритмы
- Типы сертификатов — DV/OV/EV, SAN, wildcard
- Chain of Trust — корневые и промежуточные CA
- Ключевые алгоритмы — RSA, ECDSA, Ed25519
- mTLS и client auth — взаимная аутентификация

### 🛠️ [Команды OpenSSL](./openssl-cli.md)

- Генерация ключей — RSA, EC, Ed25519
- Создание CSR — subj, SAN, конфиг
- Диагностика — s_client, s_server
- Конвертация форматов — PEM, DER, PKCS#12
- Хеши и rand — подписи, тестовые данные

### 📜 [Сертификаты и собственный CA](./certificates-ca.md)

- Self-signed сертификаты — для тестов
- Собственный CA — структура, openssl.cnf
- Промежуточные CA — цепочки доверия
- CRL/OCSP — отзыв сертификатов
- ACME/Let's Encrypt — автоматический выпуск

### 🌐 [Настройка серверов и приложений](./server-config.md)

- Nginx/Apache — tls protocols, ciphers
- gRPC/Go и Node.js — пример кода
- Java/Spring Boot — конфигурация
- HAProxy — TLS termination
- Reload без downtime — nginx reload

### ✅ [Проверка и аудит](./testing-audit.md)

- openssl s_client — быстрый чек
- testssl.sh — автоматический скан
- Qualys SSL Labs — внешний аудит
- OCSP/expiry — мониторинг срока
- Инструменты — sslscan, Prometheus

### 🛡️ [Лучшие практики и безопасность](./best-practices.md)

- Протоколы/шифры — только TLS 1.2/1.3
- HSTS/OCSP Stapling — обязательные настройки
- Session resumption — TLS tickets/cache
- Управление ключами — права, HSM/KMS
- Типичные ошибки — неполный chain, слабые шифры
