# Сертификаты и собственный CA

## Self-signed сертификат для тестов

```bash
openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
  -keyout selfsigned.key -out selfsigned.crt \
  -subj "/C=RU/ST=Moscow/L=Moscow/O=Demo/CN=demo.local"
```

## Настройка собственного CA (простая схема)

1. Структура каталогов:

```
ca/
├── index.txt
├── serial (начать с 1000)
├── certs/
├── newcerts/
├── private/
└── openssl.cnf
```

2. Пример `openssl.cnf` (минимальный):

```ini
[ ca ]
default_ca = my_ca

[ my_ca ]
dir               = ./
certs             = $dir/certs
new_certs_dir     = $dir/newcerts
database          = $dir/index.txt
serial            = $dir/serial
private_key       = $dir/private/ca.key
certificate       = $dir/certs/ca.crt
default_md        = sha256
policy            = policy_loose
email_in_dn       = no

[ policy_loose ]
commonName             = supplied
stateOrProvinceName    = optional
countryName            = optional
organizationName       = optional
organizationalUnitName = optional
```

3. Сгенерировать ключ и самоподписанный корневой сертификат:

```bash
openssl genrsa -out private/ca.key 4096
openssl req -x509 -new -key private/ca.key -days 3650 -out certs/ca.crt \
  -subj "/C=RU/O=Example Root CA/CN=Example Root CA"
```

4. Выпуск серверного сертификата на основе CSR:

```bash
openssl ca -config openssl.cnf -in server.csr -out certs/server.crt \
  -extensions server_cert -days 825
```

Добавьте секцию `server_cert` в `openssl.cnf` с SAN/узкими ограничениями.

## Промежуточный CA

- Создайте intermediate CA (тоже ключ + cert, подписанный root).
- Подписывайте серверные сертификаты промежуточным CA.
- Отдавайте цепочку: `server.crt + intermediate.crt`.

## CRL и отзыв

```bash
openssl ca -gencrl -out crl.pem -config openssl.cnf
openssl ca -revoke certs/server.crt -config openssl.cnf
```

## ACME / Let's Encrypt

- **Certbot**:

```bash
sudo apt install certbot
sudo certbot certonly --standalone -d example.com -d www.example.com
```

- **lego** (Go ACME клиент):

```bash
lego --email user@example.com --dns cloudflare --domains example.com run
```

- Автоматизация через cron/systemd timers, `certbot renew`.

## Хранение и защита

- Приватные ключи: права доступа `600`, хранить на защищённых хостах.
- Backup и recovery план (только в зашифрованном виде).
- HSM/KMS — для высоко критичных систем (AWS KMS, HashiCorp Vault PKI).
