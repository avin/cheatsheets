# Лучшие практики и безопасность

## Протоколы и шифры

- Включайте только TLS 1.2 и TLS 1.3.
- Запрещайте SSLv3, TLS 1.0/1.1.
- Используйте шифры с Perfect Forward Secrecy (ECDHE + AES-GCM/CHACHA20).
- Отключайте `NULL`, `EXPORT`, `RC4`, `3DES`.

## HSTS и HTTPS Everywhere

- Добавляйте заголовок `Strict-Transport-Security: max-age=63072000; includeSubDomains; preload`.
- Для preloading — отправьте заявку на https://hstspreload.org/.

## OCSP Stapling

- Включайте stapling (`ssl_stapling on`), чтобы клиенты быстрее получали статус.
- Храните `ssl_trusted_certificate`, содержащий chain и CA для валидации OCSP ответа.

## Session resumption

- Включайте session tickets/сache (`ssl_session_cache` в Nginx).
- Для TLS 1.3 используется PSK (Pre-Shared Key) resumption.

## Key management

- Приватные ключи должны иметь права 600 и принадлежать root/root (или dedicated user).
- Используйте HSM/KMS для генерации/хранения ключей (AWS KMS, Azure Key Vault, Hashicorp Vault) при высокой критичности.
- Регулярно обновляйте ключи при подозрении компрометации.

## Mutual TLS

- Для внутреннего трафика (service-to-service) используйте mTLS.
- Настройка trust store для клиентских сертификатов.

## Ротация и автоматизация

- Let's Encrypt (90 дней) — автоматические `certbot renew`.
- Для wildcard можно использовать DNS-01 challenge.
- В логах мониторьте "certificate will expire".

## Типичные ошибки

- Самоподписанные сертификаты в продакшене (без доверия клиентов).
- Неполная цепочка (пропущен intermediate) → браузеры не доверяют.
- Слабый key size (RSA < 2048).
- Сертификат не содержит нужных SAN (поддомены).
- HTTP доступ открыт наряду с HTTPS без 301 редиректа.

## Дополнительные настройки

- TLS termination vs end-to-end: оценивайте безопасность LB и внутреннего трафика.
- Подумайте о "SSL offload" на CDN (Cloudflare) и последствиях.
- Для gRPC/HTTP2 включайте ALPN (`ssl_alpn http/1.1 h2`).

## Compliance

- Проверяйте требования: PCI DSS (TLS 1.2+), HIPAA.
- Внутренние политики для управления сертификатами (кто выписывает, где хранит).

