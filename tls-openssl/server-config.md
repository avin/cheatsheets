# Настройка серверов и приложений

## Nginx

```nginx
server {
    listen 443 ssl http2;
    server_name example.com;

    ssl_certificate     /etc/ssl/certs/example.com.crt;      # полный chain
    ssl_certificate_key /etc/ssl/private/example.com.key;
    ssl_trusted_certificate /etc/ssl/certs/ca_bundle.crt;

    ssl_protocols TLSv1.2 TLSv1.3;
    ssl_ciphers 'ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305';
    ssl_prefer_server_ciphers on;

    ssl_session_timeout 1d;
    ssl_session_cache shared:SSL:10m;

    ssl_stapling on;
    ssl_stapling_verify on;

    add_header Strict-Transport-Security "max-age=63072000; includeSubDomains; preload" always;

    location / {
        proxy_pass http://app;
    }
}
```

- Не забывайте `hsts`, `ocsp stapling`, `ssl_trusted_certificate`.

## Apache HTTPD

```apache
<VirtualHost *:443>
    ServerName example.com

    SSLEngine on
    SSLCertificateFile      /etc/ssl/certs/example.com.crt
    SSLCertificateKeyFile   /etc/ssl/private/example.com.key
    SSLCertificateChainFile /etc/ssl/certs/ca_bundle.crt

    SSLProtocol all -SSLv3 -TLSv1 -TLSv1.1
    SSLCipherSuite ECDHE-ECDSA-AES256-GCM-SHA384:...
    SSLHonorCipherOrder on

    Header always set Strict-Transport-Security "max-age=63072000; includeSubDomains"
</VirtualHost>
```

## gRPC / HTTP/2 (Go)

```go
package main

import (
    "crypto/tls"
    "crypto/x509"
    "io/ioutil"
    "log"
    "net"

    "google.golang.org/grpc"
)

func main() {
    cert, err := tls.LoadX509KeyPair("server.crt", "server.key")
    if err != nil {
        log.Fatal(err)
    }

    caCert, err := ioutil.ReadFile("ca.crt")
    if err != nil {
        log.Fatal(err)
    }
    caPool := x509.NewCertPool()
    caPool.AppendCertsFromPEM(caCert)

    tlsConfig := &tls.Config{
        Certificates: []tls.Certificate{cert},
        ClientAuth:   tls.RequireAndVerifyClientCert,
        ClientCAs:    caPool,
        MinVersion:   tls.VersionTLS12,
    }

    lis, _ := net.Listen("tcp", ":8443")
    s := grpc.NewServer(grpc.Creds(credentials.NewTLS(tlsConfig)))
    // register services
    s.Serve(lis)
}
```

## Node.js (https module)

```js
const https = require('https');
const fs = require('fs');

const options = {
  key: fs.readFileSync('server.key'),
  cert: fs.readFileSync('server.crt'),
  ca: fs.readFileSync('ca_bundle.crt'),
  requestCert: true,             // mutual TLS
  rejectUnauthorized: true,
  minVersion: 'TLSv1.2',
};

https.createServer(options, (req, res) => {
  res.writeHead(200);
  res.end('hello tls');
}).listen(8443);
```

## Java (Spring Boot application.yml)

```yaml
server:
  port: 8443
  ssl:
    enabled: true
    key-store: classpath:server.p12
    key-store-password: changeit
    key-store-type: PKCS12
    trust-store: classpath:truststore.jks
    trust-store-password: changeit
    protocol: TLS

# Санкции TLS в Spring Security
security:
  require-ssl: true
```

## HAProxy (TLS termination)

```haproxy
frontend fe_https
    bind *:443 ssl crt /etc/haproxy/certs/example.com.pem
    http-response set-header Strict-Transport-Security "max-age=63072000; includeSubDomains"
    default_backend be_app

backend be_app
    server app1 10.0.0.10:8080 check
```

## Обновление сертификатов без downtime

- Nginx: `nginx -s reload`.
- HAProxy: `systemctl reload haproxy` (подхватывает новую bundle).
- Сервисы с TLS-настройкой внутри (Go, Node) — настройте ловлю SIGHUP и re-read ключей.

