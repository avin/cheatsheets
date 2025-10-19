# Проверка и аудит TLS

## Быстрая проверка OpenSSL

```bash
openssl s_client -connect example.com:443 -servername example.com <<'EOS'
QUIT
EOS
```
- Смотрите на `SSL-Session`, `Protocol`, `Cipher`, цепочку сертификатов.

## testssl.sh

```bash
./testssl.sh --openssl=/usr/bin/openssl example.com
```
- Показывает поддерживаемые протоколы, шифры, уязвимости (Heartbleed и др.).

## Qualys SSL Labs

1. https://www.ssllabs.com/ssltest/
2. Введите домен → получите отчёт (рейтинг A–F, цепочка, HSTS, renegotiation).

## Mozilla Observatory

- https://observatory.mozilla.org/ — проверка HTTP security headers, TLS конфигурации.

## Проверка expiry и цепочки

```bash
# Дата истечения
openssl s_client -connect example.com:443 -servername example.com 2>/dev/null |
  openssl x509 -noout -dates

# Проверка SAN
openssl x509 -in server.crt -text -noout | grep -A1 "Subject Alternative Name"
```

## OCSP status

```bash
openssl s_client -connect example.com:443 -servername example.com -status
```
- Проверяйте "OCSP Response Status: successful".

## Линтер конфигураций

- [sslscan](https://github.com/rbsec/sslscan)
- [testssl.sh](https://github.com/drwetter/testssl.sh)
- [Mozilla SSL Configuration Generator](https://ssl-config.mozilla.org/)

## Мониторинг сертификатов

- Prometheus + blackbox-exporter / ssl_exporter
- UptimeRobot, Better Uptime — уведомления об истекающих сертификатах
- CT logs мониторинг (CertSpotter, Censys, Facebook CT)
