# Команды OpenSSL

## Генерация ключей

```bash
# RSA 2048
openssl genrsa -out server.key 2048

# ECC P-256
openssl ecparam -name prime256v1 -genkey -noout -out server.key

# Ed25519 (OpenSSL 3+)
openssl genpkey -algorithm Ed25519 -out server.key
```

## Создание CSR

```bash
openssl req -new -key server.key -out server.csr \
  -subj "/C=RU/ST=Moscow/L=Moscow/O=Example Inc/OU=IT/CN=example.com"

# CSR с SAN
cat <<'CONF' > csr.conf
[ req ]
default_bits        = 2048
distinguished_name = dn
req_extensions     = san
prompt = no

[ dn ]
C  = RU
ST = Moscow
L  = Moscow
O  = Example Inc
CN = example.com

[ san ]
subjectAltName = @alt_names

[ alt_names ]
DNS.1 = example.com
DNS.2 = www.example.com
CONF

openssl req -new -key server.key -out server.csr -config csr.conf
```

## Просмотр сертификата/ключа

```bash
openssl x509 -in server.crt -text -noout
openssl x509 -noout -fingerprint -sha256 -in server.crt
openssl rsa -in server.key -check
```

## Проверка цепочки

```bash
openssl verify -CAfile chain.crt server.crt
```

## Диагностика соединения

```bash
# Подключиться к серверу
openssl s_client -connect example.com:443 -servername example.com

# Проверить TLS 1.3
openssl s_client -connect example.com:443 -tls1_3

# Проверить OCSP stapling
openssl s_client -connect example.com:443 -status
```

## Собственный тестовый сервер

```bash
openssl s_server -accept 8443 -cert server.crt -key server.key -www
```

## Конвертация форматов

```bash
# PEM -> DER
openssl x509 -outform der -in server.crt -out server.der

# PEM + key -> PKCS#12
openssl pkcs12 -export -inkey server.key -in server.crt -certfile chain.crt -out server.p12

# PKCS#12 -> JKS (через keytool)
keytool -importkeystore \
  -srckeystore server.p12 -srcstoretype pkcs12 \
  -destkeystore server.jks -deststoretype JKS
```

## Хеши и случайные данные

```bash
openssl rand -hex 32
openssl dgst -sha256 file.txt
```

