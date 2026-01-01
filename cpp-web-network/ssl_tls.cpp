// ============================================
// 📌 SSL/TLS Basics
// ============================================
// • TLS 1.2 vs TLS 1.3
// • Handshake process
// • Cipher suites
// • Certificate chain
// • Public/Private keys
// • Certificate authorities (CA)

// ============================================
// 📌 OpenSSL Integration
// ============================================

// --- SSL Context ---
// • SSL_CTX_new()
// • Protocol version selection
// • Certificate loading
// • Private key loading
// • CA certificate loading
// • Cipher suite configuration

// --- SSL Connection ---
// • SSL_new()
// • BIO objects
// • SSL_connect() / SSL_accept()
// • SSL_read() / SSL_write()
// • Shutdown и cleanup

// --- RAII Wrapper ---
class SSLConnection {
    // • Automatic cleanup
    // • Exception safety
    // • Move semantics
};

// ============================================
// 📌 Certificate Management
// ============================================
// • Certificate generation (self-signed)
// • CSR (Certificate Signing Request)
// • Certificate verification
// • Chain validation
// • Expiration checking
// • Hostname validation (SNI)
// • Certificate pinning

// ============================================
// 📌 Modern TLS Practices
// ============================================
// • TLS 1.3 only
// • Perfect Forward Secrecy (PFS)
// • Disable weak ciphers
// • ALPN (Application-Layer Protocol Negotiation)
// • OCSP stapling
// • Session resumption
// • 0-RTT (TLS 1.3)

// ============================================
// 📌 Mutual TLS (mTLS)
// ============================================
// • Client certificates
// • Two-way authentication
// • Certificate-based auth
// • Verification callback
// • Use cases (microservices, IoT)

// ============================================
// 📌 Cryptography Primitives
// ============================================

// --- Hashing ---
// • SHA-256, SHA-512
// • HMAC
// • Password hashing (bcrypt, scrypt, argon2)

// --- Encryption ---
// • AES (GCM, CBC modes)
// • ChaCha20-Poly1305
// • RSA encryption
// • Symmetric vs Asymmetric

// --- Signing ---
// • RSA signatures
// • ECDSA
// • Ed25519
// • Verification

// ============================================
// 📌 JWT (JSON Web Tokens)
// ============================================
// • Token structure
// • HMAC signing (HS256)
// • RSA signing (RS256)
// • Claims validation
// • Expiration checking
// • Token refresh

// ============================================
// 📌 Security Best Practices
// ============================================
// • Input validation
// • SQL injection prevention
// • XSS prevention
// • CSRF protection
// • Rate limiting
// • Secret management
// • Logging (without secrets)