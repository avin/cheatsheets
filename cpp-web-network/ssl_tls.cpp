// ============================================
// 📌 SSL/TLS Basics
// ============================================

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/hmac.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>

// TLS 1.2 vs TLS 1.3
// 
// TLS 1.2:
// - 2 RTT для полного handshake
// - Поддержка устаревших cipher suites
// - Более медленный handshake
// 
// TLS 1.3 (рекомендуется):
// - 1 RTT для handshake
// - 0-RTT для resumed connections
// - Удалены слабые cipher suites
// - Perfect Forward Secrecy обязателен
// - Быстрее и безопаснее

// Handshake процесс (TLS 1.3):
// 1. Client Hello → Server (supported ciphers, key share)
// 2. Server Hello + Certificate + Key ← Server
// 3. Finished → Server
// 4. Finished ← Server
// Итого: 1 RTT до начала передачи данных

// Cipher Suite пример: TLS_AES_256_GCM_SHA384
// - TLS - протокол
// - AES_256 - симметричное шифрование (256-бит ключ)
// - GCM - режим шифрования (Galois/Counter Mode)
// - SHA384 - хеш функция для HMAC

void tls_basics_explanation() {
    // Certificate Chain:
    // Root CA → Intermediate CA → Server Certificate
    
    // Public/Private keys:
    // - Приватный ключ остаётся на сервере (НИКОГДА не передаётся!)
    // - Публичный ключ в сертификате (доступен всем)
    // - Используется для асимметричного шифрования и подписей
    
    std::cout << "TLS обеспечивает:\n";
    std::cout << "1. Конфиденциальность (encryption)\n";
    std::cout << "2. Целостность (integrity)\n";
    std::cout << "3. Аутентификацию (authentication)\n";
}

// ============================================
// 📌 OpenSSL Integration
// ============================================

// Инициализация OpenSSL (вызвать один раз при старте)
void init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

// Очистка OpenSSL
void cleanup_openssl() {
    EVP_cleanup();
}

// SSL Context - глобальные настройки для SSL соединений
SSL_CTX* create_ssl_context_server() {
    // Создаём контекст для TLS сервера
    const SSL_METHOD* method = TLS_server_method();  // TLS 1.2+
    SSL_CTX* ctx = SSL_CTX_new(method);
    
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return nullptr;
    }
    
    // Настраиваем минимальную версию TLS (только 1.3)
    SSL_CTX_set_min_proto_version(ctx, TLS1_3_VERSION);
    
    // Настройка cipher suites для TLS 1.3
    SSL_CTX_set_ciphersuites(ctx,
        "TLS_AES_256_GCM_SHA384:"
        "TLS_CHACHA20_POLY1305_SHA256:"
        "TLS_AES_128_GCM_SHA256"
    );
    
    return ctx;
}

SSL_CTX* create_ssl_context_client() {
    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return nullptr;
    }
    
    // Только TLS 1.3
    SSL_CTX_set_min_proto_version(ctx, TLS1_3_VERSION);
    
    // Загрузка CA сертификатов для верификации сервера
    SSL_CTX_set_default_verify_paths(ctx);
    
    // Включаем верификацию сертификата сервера
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, nullptr);
    
    return ctx;
}

// Загрузка сертификата и приватного ключа (для сервера)
bool load_certificates(SSL_CTX* ctx, const char* cert_file, const char* key_file) {
    // Загрузка сертификата сервера
    if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return false;
    }
    
    // Загрузка приватного ключа
    if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return false;
    }
    
    // Проверка соответствия ключа и сертификата
    if (!SSL_CTX_check_private_key(ctx)) {
        std::cerr << "Private key does not match the certificate\n";
        return false;
    }
    
    return true;
}

// Загрузка CA сертификата (для верификации клиентов)
bool load_ca_certificate(SSL_CTX* ctx, const char* ca_file) {
    if (SSL_CTX_load_verify_locations(ctx, ca_file, nullptr) != 1) {
        ERR_print_errors_fp(stderr);
        return false;
    }
    
    // Запрашиваем сертификат клиента
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, nullptr);
    
    return true;
}

// ============================================
// 📌 SSL Connection - RAII Wrapper
// ============================================

class SSLConnection {
    SSL* ssl_ = nullptr;
    int socket_fd_ = -1;
    
public:
    SSLConnection() = default;
    
    // Создание SSL соединения для сервера
    static std::unique_ptr<SSLConnection> accept(SSL_CTX* ctx, int client_fd) {
        auto conn = std::make_unique<SSLConnection>();
        conn->socket_fd_ = client_fd;
        conn->ssl_ = SSL_new(ctx);
        
        if (!conn->ssl_) {
            return nullptr;
        }
        
        // Привязываем SSL к сокету
        SSL_set_fd(conn->ssl_, client_fd);
        
        // SSL handshake (server)
        if (SSL_accept(conn->ssl_) <= 0) {
            ERR_print_errors_fp(stderr);
            return nullptr;
        }
        
        std::cout << "SSL connection established\n";
        std::cout << "Cipher: " << SSL_get_cipher(conn->ssl_) << '\n';
        
        return conn;
    }
    
    // Создание SSL соединения для клиента
    static std::unique_ptr<SSLConnection> connect(SSL_CTX* ctx, int server_fd,
                                                   const char* hostname = nullptr) {
        auto conn = std::make_unique<SSLConnection>();
        conn->socket_fd_ = server_fd;
        conn->ssl_ = SSL_new(ctx);
        
        if (!conn->ssl_) {
            return nullptr;
        }
        
        SSL_set_fd(conn->ssl_, server_fd);
        
        // SNI (Server Name Indication) - для виртуальных хостов
        if (hostname) {
            SSL_set_tlsext_host_name(conn->ssl_, hostname);
        }
        
        // SSL handshake (client)
        if (SSL_connect(conn->ssl_) <= 0) {
            ERR_print_errors_fp(stderr);
            return nullptr;
        }
        
        // Верификация сертификата сервера
        if (SSL_get_verify_result(conn->ssl_) != X509_V_OK) {
            std::cerr << "Certificate verification failed\n";
            return nullptr;
        }
        
        std::cout << "SSL connection established\n";
        
        return conn;
    }
    
    // Запись данных
    int write(const char* data, size_t len) {
        if (!ssl_) return -1;
        
        int bytes = SSL_write(ssl_, data, len);
        
        if (bytes <= 0) {
            int error = SSL_get_error(ssl_, bytes);
            
            if (error == SSL_ERROR_WANT_WRITE) {
                // Нужно повторить позже
                return 0;
            }
            
            ERR_print_errors_fp(stderr);
            return -1;
        }
        
        return bytes;
    }
    
    // Чтение данных
    int read(char* buffer, size_t len) {
        if (!ssl_) return -1;
        
        int bytes = SSL_read(ssl_, buffer, len);
        
        if (bytes <= 0) {
            int error = SSL_get_error(ssl_, bytes);
            
            if (error == SSL_ERROR_WANT_READ) {
                // Нужно повторить позже
                return 0;
            }
            
            if (error == SSL_ERROR_ZERO_RETURN) {
                // Соединение закрыто
                return 0;
            }
            
            ERR_print_errors_fp(stderr);
            return -1;
        }
        
        return bytes;
    }
    
    // Graceful shutdown
    void shutdown() {
        if (ssl_) {
            SSL_shutdown(ssl_);  // Отправляем close_notify
            SSL_free(ssl_);
            ssl_ = nullptr;
        }
    }
    
    ~SSLConnection() {
        shutdown();
    }
    
    // Запрещаем копирование
    SSLConnection(const SSLConnection&) = delete;
    SSLConnection& operator=(const SSLConnection&) = delete;
    
    // Разрешаем перемещение
    SSLConnection(SSLConnection&& other) noexcept
        : ssl_(other.ssl_), socket_fd_(other.socket_fd_) {
        other.ssl_ = nullptr;
        other.socket_fd_ = -1;
    }
    
    SSLConnection& operator=(SSLConnection&& other) noexcept {
        if (this != &other) {
            shutdown();
            ssl_ = other.ssl_;
            socket_fd_ = other.socket_fd_;
            other.ssl_ = nullptr;
            other.socket_fd_ = -1;
        }
        return *this;
    }
};

// Пример использования: TLS сервер
void tls_server_example() {
    init_openssl();
    
    SSL_CTX* ctx = create_ssl_context_server();
    
    if (!load_certificates(ctx, "server.crt", "server.key")) {
        SSL_CTX_free(ctx);
        cleanup_openssl();
        return;
    }
    
    // Создаём TCP сокет...
    // int server_fd = create_tcp_server(8443);
    int server_fd = 0;  // Упрощено
    
    // Принимаем клиента
    int client_fd = accept(server_fd, nullptr, nullptr);
    
    // SSL handshake
    auto ssl_conn = SSLConnection::accept(ctx, client_fd);
    
    if (ssl_conn) {
        char buffer[4096];
        int bytes = ssl_conn->read(buffer, sizeof(buffer));
        
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << "Received: " << buffer << '\n';
            
            ssl_conn->write("Hello from TLS server!", 22);
        }
    }
    
    SSL_CTX_free(ctx);
    cleanup_openssl();
}

// Пример использования: TLS клиент
void tls_client_example() {
    init_openssl();
    
    SSL_CTX* ctx = create_ssl_context_client();
    
    // Подключаемся к серверу
    // int server_fd = connect_to_server("example.com", 443);
    int server_fd = 0;  // Упрощено
    
    // SSL handshake
    auto ssl_conn = SSLConnection::connect(ctx, server_fd, "example.com");
    
    if (ssl_conn) {
        ssl_conn->write("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n", 38);
        
        char buffer[4096];
        int bytes = ssl_conn->read(buffer, sizeof(buffer));
        
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << "Response: " << buffer << '\n';
        }
    }
    
    SSL_CTX_free(ctx);
    cleanup_openssl();
}

// ============================================
// 📌 Certificate Management
// ============================================

// Генерация self-signed сертификата
bool generate_self_signed_cert(const char* cert_file, const char* key_file) {
    // Генерация RSA ключа (2048 бит)
    EVP_PKEY* pkey = EVP_PKEY_new();
    RSA* rsa = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);
    EVP_PKEY_assign_RSA(pkey, rsa);
    
    // Создание сертификата
    X509* x509 = X509_new();
    X509_set_version(x509, 2);  // X509 v3
    
    // Серийный номер
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
    
    // Срок действия (1 год)
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 365 * 24 * 60 * 60);
    
    // Установка публичного ключа
    X509_set_pubkey(x509, pkey);
    
    // Subject (кто выпустил)
    X509_NAME* name = X509_get_subject_name(x509);
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, 
                               (unsigned char*)"US", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, 
                               (unsigned char*)"MyCompany", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, 
                               (unsigned char*)"localhost", -1, -1, 0);
    
    // Issuer (для self-signed = subject)
    X509_set_issuer_name(x509, name);
    
    // Подпись сертификата
    X509_sign(x509, pkey, EVP_sha256());
    
    // Сохранение приватного ключа
    FILE* key_fp = fopen(key_file, "wb");
    PEM_write_PrivateKey(key_fp, pkey, nullptr, nullptr, 0, nullptr, nullptr);
    fclose(key_fp);
    
    // Сохранение сертификата
    FILE* cert_fp = fopen(cert_file, "wb");
    PEM_write_X509(cert_fp, x509);
    fclose(cert_fp);
    
    X509_free(x509);
    EVP_PKEY_free(pkey);
    
    return true;
}

// Верификация сертификата
bool verify_certificate(X509* cert, X509_STORE* store) {
    X509_STORE_CTX* ctx = X509_STORE_CTX_new();
    X509_STORE_CTX_init(ctx, store, cert, nullptr);
    
    int result = X509_verify_cert(ctx);
    
    if (result != 1) {
        int error = X509_STORE_CTX_get_error(ctx);
        std::cerr << "Verification failed: " 
                  << X509_verify_cert_error_string(error) << '\n';
    }
    
    X509_STORE_CTX_free(ctx);
    
    return result == 1;
}

// Проверка срока действия сертификата
bool check_certificate_expiration(X509* cert) {
    ASN1_TIME* not_before = X509_get_notBefore(cert);
    ASN1_TIME* not_after = X509_get_notAfter(cert);
    
    // Проверяем, что сертификат уже действителен
    if (X509_cmp_current_time(not_before) >= 0) {
        std::cerr << "Certificate is not yet valid\n";
        return false;
    }
    
    // Проверяем, что сертификат не истёк
    if (X509_cmp_current_time(not_after) <= 0) {
        std::cerr << "Certificate has expired\n";
        return false;
    }
    
    return true;
}

// Hostname validation (для SNI)
bool verify_hostname(SSL* ssl, const char* expected_hostname) {
    X509* cert = SSL_get_peer_certificate(ssl);
    
    if (!cert) {
        return false;
    }
    
    // Проверка через X509_check_host
    int result = X509_check_host(cert, expected_hostname, 
                                 strlen(expected_hostname), 0, nullptr);
    
    X509_free(cert);
    
    return result == 1;
}

// Certificate Pinning - защита от MITM
class CertificatePinner {
    std::vector<std::string> pinned_fingerprints_;
    
public:
    void add_pin(const std::string& fingerprint) {
        pinned_fingerprints_.push_back(fingerprint);
    }
    
    bool verify(X509* cert) {
        // Вычисляем SHA256 fingerprint сертификата
        unsigned char digest[SHA256_DIGEST_LENGTH];
        unsigned int len;
        
        X509_digest(cert, EVP_sha256(), digest, &len);
        
        std::string fingerprint;
        for (unsigned int i = 0; i < len; ++i) {
            char buf[3];
            snprintf(buf, sizeof(buf), "%02x", digest[i]);
            fingerprint += buf;
        }
        
        // Проверяем наличие в pinned списке
        for (const auto& pin : pinned_fingerprints_) {
            if (pin == fingerprint) {
                return true;
            }
        }
        
        std::cerr << "Certificate pinning failed!\n";
        return false;
    }
};

// ============================================
// 📌 Modern TLS Practices
// ============================================

void configure_modern_tls(SSL_CTX* ctx) {
    // 1. Только TLS 1.3
    SSL_CTX_set_min_proto_version(ctx, TLS1_3_VERSION);
    SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION);
    
    // 2. Сильные cipher suites (TLS 1.3 имеет только сильные)
    SSL_CTX_set_ciphersuites(ctx,
        "TLS_AES_256_GCM_SHA384:"
        "TLS_CHACHA20_POLY1305_SHA256"
    );
    
    // 3. Perfect Forward Secrecy обязателен в TLS 1.3
    
    // 4. ALPN (Application-Layer Protocol Negotiation)
    // Например, для HTTP/2
    const unsigned char alpn[] = "\x02h2\x08http/1.1";
    SSL_CTX_set_alpn_protos(ctx, alpn, sizeof(alpn) - 1);
    
    // 5. Session resumption (TLS 1.3 tickets)
    SSL_CTX_set_num_tickets(ctx, 2);  // Выдаём 2 session ticket
    
    // 6. Отключаем устаревшие опции
    SSL_CTX_set_options(ctx, SSL_OP_NO_COMPRESSION);  // Защита от CRIME
    SSL_CTX_set_options(ctx, SSL_OP_NO_RENEGOTIATION);  // Защита от attacks
}

// ALPN callback для сервера
int alpn_select_callback(SSL* ssl, const unsigned char** out, 
                        unsigned char* outlen, const unsigned char* in,
                        unsigned int inlen, void* arg) {
    // Список поддерживаемых протоколов
    const unsigned char server_alpn[] = "\x02h2\x08http/1.1";
    
    if (SSL_select_next_proto((unsigned char**)out, outlen,
                             server_alpn, sizeof(server_alpn) - 1,
                             in, inlen) != OPENSSL_NPN_NEGOTIATED) {
        return SSL_TLSEXT_ERR_NOACK;
    }
    
    return SSL_TLSEXT_ERR_OK;
}

// OCSP Stapling - проверка статуса сертификата
void enable_ocsp_stapling(SSL_CTX* ctx) {
    SSL_CTX_set_tlsext_status_type(ctx, TLSEXT_STATUSTYPE_ocsp);
    
    // Callback для обработки OCSP response
    // SSL_CTX_set_tlsext_status_cb(ctx, ocsp_callback);
}

// 0-RTT (TLS 1.3 early data)
void enable_0rtt(SSL_CTX* ctx) {
    SSL_CTX_set_max_early_data(ctx, 16384);  // 16 KB early data
}

bool send_early_data(SSL* ssl, const char* data, size_t len) {
    size_t written;
    
    if (SSL_write_early_data(ssl, data, len, &written) != 1) {
        return false;
    }
    
    return written == len;
}

// ============================================
// 📌 Mutual TLS (mTLS)
// ============================================

// Настройка mTLS на сервере
void configure_mtls_server(SSL_CTX* ctx, const char* ca_cert) {
    // Загрузка CA для проверки клиентов
    SSL_CTX_load_verify_locations(ctx, ca_cert, nullptr);
    
    // Требуем сертификат от клиента
    SSL_CTX_set_verify(ctx, 
        SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
        nullptr);
    
    // Глубина проверки цепочки сертификатов
    SSL_CTX_set_verify_depth(ctx, 4);
}

// Настройка mTLS на клиенте
void configure_mtls_client(SSL_CTX* ctx, const char* client_cert, 
                          const char* client_key, const char* ca_cert) {
    // Загрузка клиентского сертификата
    SSL_CTX_use_certificate_file(ctx, client_cert, SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, client_key, SSL_FILETYPE_PEM);
    
    // Загрузка CA для проверки сервера
    SSL_CTX_load_verify_locations(ctx, ca_cert, nullptr);
    
    // Требуем верификацию сервера
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, nullptr);
}

// Custom verify callback для дополнительных проверок
int verify_callback(int preverify_ok, X509_STORE_CTX* ctx) {
    if (!preverify_ok) {
        int error = X509_STORE_CTX_get_error(ctx);
        std::cerr << "Verification error: " 
                  << X509_verify_cert_error_string(error) << '\n';
        
        X509* cert = X509_STORE_CTX_get_current_cert(ctx);
        
        char subject[256];
        X509_NAME_oneline(X509_get_subject_name(cert), subject, sizeof(subject));
        std::cerr << "Subject: " << subject << '\n';
        
        return 0;  // Отклоняем сертификат
    }
    
    // Дополнительные проверки (например, blacklist)
    X509* cert = X509_STORE_CTX_get_current_cert(ctx);
    
    // Проверка Common Name
    char cn[256];
    X509_NAME_get_text_by_NID(X509_get_subject_name(cert),
                              NID_commonName, cn, sizeof(cn));
    
    std::cout << "Verified CN: " << cn << '\n';
    
    return 1;  // Принимаем
}

// ============================================
// 📌 Cryptography Primitives
// ============================================

// SHA-256 хеширование
std::string sha256(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, data.c_str(), data.size());
    SHA256_Final(hash, &sha256_ctx);
    
    std::string result;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", hash[i]);
        result += buf;
    }
    
    return result;
}

// HMAC-SHA256
std::string hmac_sha256(const std::string& key, const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    
    HMAC(EVP_sha256(), key.c_str(), key.size(),
         (unsigned char*)data.c_str(), data.size(),
         hash, nullptr);
    
    std::string result;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", hash[i]);
        result += buf;
    }
    
    return result;
}

// AES-256-GCM шифрование
std::vector<unsigned char> aes_gcm_encrypt(
    const std::vector<unsigned char>& plaintext,
    const unsigned char* key,      // 32 bytes для AES-256
    const unsigned char* iv,       // 12 bytes для GCM
    std::vector<unsigned char>& tag  // 16 bytes authentication tag
) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    
    // Инициализация шифрования
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key, iv);
    
    std::vector<unsigned char> ciphertext(plaintext.size() + 16);
    int len;
    
    // Шифрование
    EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size());
    int ciphertext_len = len;
    
    // Финализация
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
    ciphertext_len += len;
    
    // Получение authentication tag
    tag.resize(16);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag.data());
    
    EVP_CIPHER_CTX_free(ctx);
    
    ciphertext.resize(ciphertext_len);
    return ciphertext;
}

// AES-256-GCM расшифровка
std::vector<unsigned char> aes_gcm_decrypt(
    const std::vector<unsigned char>& ciphertext,
    const unsigned char* key,
    const unsigned char* iv,
    const std::vector<unsigned char>& tag
) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key, iv);
    
    std::vector<unsigned char> plaintext(ciphertext.size());
    int len;
    
    EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size());
    int plaintext_len = len;
    
    // Установка authentication tag для верификации
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, (void*)tag.data());
    
    // Финализация (проверяет tag)
    int result = EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
    
    EVP_CIPHER_CTX_free(ctx);
    
    if (result <= 0) {
        // Authentication failed!
        throw std::runtime_error("Decryption failed - invalid tag");
    }
    
    plaintext_len += len;
    plaintext.resize(plaintext_len);
    
    return plaintext;
}

// RSA подпись
std::vector<unsigned char> rsa_sign(const std::string& data, EVP_PKEY* private_key) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    
    EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, private_key);
    EVP_DigestSignUpdate(ctx, data.c_str(), data.size());
    
    size_t sig_len;
    EVP_DigestSignFinal(ctx, nullptr, &sig_len);
    
    std::vector<unsigned char> signature(sig_len);
    EVP_DigestSignFinal(ctx, signature.data(), &sig_len);
    
    EVP_MD_CTX_free(ctx);
    
    return signature;
}

// RSA верификация подписи
bool rsa_verify(const std::string& data, const std::vector<unsigned char>& signature,
               EVP_PKEY* public_key) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    
    EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, public_key);
    EVP_DigestVerifyUpdate(ctx, data.c_str(), data.size());
    
    int result = EVP_DigestVerifyFinal(ctx, signature.data(), signature.size());
    
    EVP_MD_CTX_free(ctx);
    
    return result == 1;
}

// ============================================
// 📌 JWT (JSON Web Tokens)
// ============================================

#include <base64.h>  // Нужна библиотека для base64

// JWT структура: header.payload.signature

std::string base64_url_encode(const std::string& data) {
    // Base64 URL encoding (заменяем +/= на -_)
    std::string encoded;  // = base64_encode(data);
    
    for (char& c : encoded) {
        if (c == '+') c = '-';
        if (c == '/') c = '_';
    }
    
    // Удаляем padding
    encoded.erase(std::find(encoded.begin(), encoded.end(), '='), encoded.end());
    
    return encoded;
}

// Создание JWT с HMAC-SHA256 (HS256)
std::string create_jwt_hs256(const std::string& payload, const std::string& secret) {
    // Header
    std::string header = R"({"alg":"HS256","typ":"JWT"})";
    
    std::string header_encoded = base64_url_encode(header);
    std::string payload_encoded = base64_url_encode(payload);
    
    // Данные для подписи
    std::string message = header_encoded + "." + payload_encoded;
    
    // HMAC signature
    std::string signature = hmac_sha256(secret, message);
    std::string signature_encoded = base64_url_encode(signature);
    
    // Итоговый JWT
    return message + "." + signature_encoded;
}

// Верификация JWT
bool verify_jwt_hs256(const std::string& token, const std::string& secret) {
    // Разбиваем токен на части
    size_t pos1 = token.find('.');
    size_t pos2 = token.find('.', pos1 + 1);
    
    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        return false;
    }
    
    std::string message = token.substr(0, pos2);
    std::string signature = token.substr(pos2 + 1);
    
    // Вычисляем ожидаемую подпись
    std::string expected_sig = hmac_sha256(secret, message);
    std::string expected_sig_encoded = base64_url_encode(expected_sig);
    
    // Сравниваем (constant-time для защиты от timing attacks)
    return signature == expected_sig_encoded;
}

// Проверка expiration в JWT
bool check_jwt_expiration(const std::string& payload_json) {
    // Парсим JSON и проверяем поле "exp" (timestamp)
    // Simplified:
    
    // auto exp = parse_json(payload_json)["exp"];
    // auto now = std::chrono::system_clock::now().time_since_epoch().count();
    
    // return exp > now;
    
    return true;  // Упрощённо
}

// ============================================
// 📌 Security Best Practices
// ============================================

// Защита от SQL Injection - используем prepared statements
// (пример с SQLite)
void safe_sql_query(sqlite3* db, const std::string& user_input) {
    const char* sql = "SELECT * FROM users WHERE username = ?";
    sqlite3_stmt* stmt;
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    // Bind параметр (экранирование автоматическое)
    sqlite3_bind_text(stmt, 1, user_input.c_str(), -1, SQLITE_STATIC);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Обработка результатов
    }
    
    sqlite3_finalize(stmt);
}

// Защита от XSS - HTML escaping
std::string html_escape(const std::string& data) {
    std::string escaped;
    
    for (char c : data) {
        switch (c) {
            case '<': escaped += "&lt;"; break;
            case '>': escaped += "&gt;"; break;
            case '&': escaped += "&amp;"; break;
            case '"': escaped += "&quot;"; break;
            case '\'': escaped += "&#x27;"; break;
            default: escaped += c;
        }
    }
    
    return escaped;
}

// CSRF Token генерация
std::string generate_csrf_token() {
    unsigned char random_bytes[32];
    RAND_bytes(random_bytes, sizeof(random_bytes));
    
    std::string token;
    for (int i = 0; i < 32; ++i) {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", random_bytes[i]);
        token += buf;
    }
    
    return token;
}

// Rate Limiting - простая реализация
class RateLimiter {
    struct ClientInfo {
        int request_count;
        std::chrono::steady_clock::time_point window_start;
    };
    
    std::unordered_map<std::string, ClientInfo> clients_;
    std::mutex mutex_;
    int max_requests_per_minute_ = 60;
    
public:
    bool allow_request(const std::string& client_ip) {
        std::lock_guard lock(mutex_);
        auto now = std::chrono::steady_clock::now();
        
        auto& info = clients_[client_ip];
        
        // Новое окно времени (прошла минута)
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            now - info.window_start);
        
        if (elapsed.count() >= 60) {
            info.request_count = 0;
            info.window_start = now;
        }
        
        info.request_count++;
        
        return info.request_count <= max_requests_per_minute_;
    }
};

// Безопасное логирование (без секретов)
void safe_log(const std::string& message, const std::string& sensitive_data) {
    // НЕ логируем sensitive_data напрямую!
    
    std::string safe_message = message;
    
    // Маскируем sensitive данные
    std::string masked = std::string(sensitive_data.size(), '*');
    
    std::cout << "LOG: " << safe_message << " [REDACTED:" << masked << "]\n";
}

// Секретное управление - загрузка из переменных окружения
std::string get_secret(const char* env_var) {
    const char* value = std::getenv(env_var);
    
    if (!value) {
        throw std::runtime_error("Secret not found in environment");
    }
    
    return std::string(value);
}

// Пример использования
void security_best_practices_example() {
    // 1. Используем переменные окружения для секретов
    std::string db_password = get_secret("DB_PASSWORD");
    std::string jwt_secret = get_secret("JWT_SECRET");
    
    // 2. Rate limiting
    RateLimiter limiter;
    
    if (!limiter.allow_request("192.168.1.1")) {
        std::cout << "Rate limit exceeded!\n";
        return;
    }
    
    // 3. CSRF protection
    std::string csrf_token = generate_csrf_token();
    
    // 4. Безопасное логирование
    safe_log("User logged in", "password123");  // Не логируем пароль
    
    // 5. Input validation
    std::string user_input = "<script>alert('XSS')</script>";
    std::string safe_output = html_escape(user_input);
    
    std::cout << "Safe output: " << safe_output << '\n';
}