// ============================================
// 📌 JSON Serialization
// ============================================

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>
#include <iostream>

// Manual JSON Building - создание JSON вручную

class JSON {
public:
    using Object = std::unordered_map<std::string, JSON>;
    using Array = std::vector<JSON>;
    using Value = std::variant<std::nullptr_t, bool, int64_t, double, 
                               std::string, Array, Object>;
    
private:
    Value value_;
    
public:
    // Конструкторы для разных типов
    JSON() : value_(nullptr) {}
    JSON(std::nullptr_t) : value_(nullptr) {}
    JSON(bool b) : value_(b) {}
    JSON(int i) : value_(static_cast<int64_t>(i)) {}
    JSON(int64_t i) : value_(i) {}
    JSON(double d) : value_(d) {}
    JSON(const char* s) : value_(std::string(s)) {}
    JSON(const std::string& s) : value_(s) {}
    JSON(const Array& a) : value_(a) {}
    JSON(const Object& o) : value_(o) {}
    
    // Доступ к объектам
    JSON& operator[](const std::string& key) {
        if (!std::holds_alternative<Object>(value_)) {
            value_ = Object{};
        }
        return std::get<Object>(value_)[key];
    }
    
    // Доступ к массивам
    JSON& operator[](size_t index) {
        if (!std::holds_alternative<Array>(value_)) {
            value_ = Array{};
        }
        auto& arr = std::get<Array>(value_);
        if (index >= arr.size()) {
            arr.resize(index + 1);
        }
        return arr[index];
    }
    
    // Сериализация в строку
    std::string to_string(bool pretty = false, int indent = 0) const {
        std::ostringstream oss;
        
        std::visit([&](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            
            if constexpr (std::is_same_v<T, std::nullptr_t>) {
                oss << "null";
            }
            else if constexpr (std::is_same_v<T, bool>) {
                oss << (v ? "true" : "false");
            }
            else if constexpr (std::is_same_v<T, int64_t>) {
                oss << v;
            }
            else if constexpr (std::is_same_v<T, double>) {
                oss << v;
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                oss << '"' << escape_string(v) << '"';
            }
            else if constexpr (std::is_same_v<T, Array>) {
                oss << '[';
                if (pretty && !v.empty()) oss << '\n';
                
                for (size_t i = 0; i < v.size(); ++i) {
                    if (pretty) {
                        oss << std::string((indent + 1) * 2, ' ');
                    }
                    oss << v[i].to_string(pretty, indent + 1);
                    if (i < v.size() - 1) oss << ',';
                    if (pretty) oss << '\n';
                }
                
                if (pretty && !v.empty()) {
                    oss << std::string(indent * 2, ' ');
                }
                oss << ']';
            }
            else if constexpr (std::is_same_v<T, Object>) {
                oss << '{';
                if (pretty && !v.empty()) oss << '\n';
                
                size_t count = 0;
                for (const auto& [key, value] : v) {
                    if (pretty) {
                        oss << std::string((indent + 1) * 2, ' ');
                    }
                    oss << '"' << key << '"' << ':';
                    if (pretty) oss << ' ';
                    oss << value.to_string(pretty, indent + 1);
                    if (++count < v.size()) oss << ',';
                    if (pretty) oss << '\n';
                }
                
                if (pretty && !v.empty()) {
                    oss << std::string(indent * 2, ' ');
                }
                oss << '}';
            }
        }, value_);
        
        return oss.str();
    }
    
private:
    static std::string escape_string(const std::string& s) {
        std::string result;
        for (char c : s) {
            switch (c) {
                case '"': result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\b': result += "\\b"; break;
                case '\f': result += "\\f"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default:
                    if (c < 0x20) {
                        char buf[7];
                        snprintf(buf, sizeof(buf), "\\u%04x", c);
                        result += buf;
                    } else {
                        result += c;
                    }
            }
        }
        return result;
    }
};

// Использование Manual JSON Builder
void manual_json_example() {
    JSON user;
    user["id"] = 123;
    user["name"] = "Alice";
    user["email"] = "alice@example.com";
    user["active"] = true;
    
    // Вложенный объект
    user["address"]["city"] = "New York";
    user["address"]["zip"] = "10001";
    
    // Массив
    user["tags"][0] = "admin";
    user["tags"][1] = "moderator";
    
    std::cout << "Compact: " << user.to_string() << '\n';
    std::cout << "Pretty:\n" << user.to_string(true) << '\n';
}

// JSON Parsing - простой парсер
class JSONParser {
    std::string_view input_;
    size_t pos_ = 0;
    
public:
    JSONParser(std::string_view input) : input_(input) {}
    
    JSON parse() {
        skip_whitespace();
        return parse_value();
    }
    
private:
    void skip_whitespace() {
        while (pos_ < input_.size() && std::isspace(input_[pos_])) {
            pos_++;
        }
    }
    
    JSON parse_value() {
        skip_whitespace();
        
        if (pos_ >= input_.size()) {
            throw std::runtime_error("Unexpected end of input");
        }
        
        char c = input_[pos_];
        
        if (c == '{') return parse_object();
        if (c == '[') return parse_array();
        if (c == '"') return parse_string();
        if (c == 't' || c == 'f') return parse_bool();
        if (c == 'n') return parse_null();
        if (c == '-' || std::isdigit(c)) return parse_number();
        
        throw std::runtime_error("Invalid JSON");
    }
    
    JSON parse_object() {
        JSON::Object obj;
        pos_++;  // Пропускаем '{'
        
        skip_whitespace();
        if (input_[pos_] == '}') {
            pos_++;
            return JSON(obj);
        }
        
        while (true) {
            skip_whitespace();
            
            // Парсим ключ
            std::string key = std::get<std::string>(parse_string().value_);
            
            skip_whitespace();
            if (input_[pos_] != ':') {
                throw std::runtime_error("Expected ':'");
            }
            pos_++;
            
            // Парсим значение
            obj[key] = parse_value();
            
            skip_whitespace();
            if (input_[pos_] == '}') {
                pos_++;
                break;
            }
            
            if (input_[pos_] != ',') {
                throw std::runtime_error("Expected ',' or '}'");
            }
            pos_++;
        }
        
        return JSON(obj);
    }
    
    JSON parse_array() {
        JSON::Array arr;
        pos_++;  // Пропускаем '['
        
        skip_whitespace();
        if (input_[pos_] == ']') {
            pos_++;
            return JSON(arr);
        }
        
        while (true) {
            arr.push_back(parse_value());
            
            skip_whitespace();
            if (input_[pos_] == ']') {
                pos_++;
                break;
            }
            
            if (input_[pos_] != ',') {
                throw std::runtime_error("Expected ',' or ']'");
            }
            pos_++;
        }
        
        return JSON(arr);
    }
    
    JSON parse_string() {
        pos_++;  // Пропускаем '"'
        std::string result;
        
        while (pos_ < input_.size() && input_[pos_] != '"') {
            if (input_[pos_] == '\\') {
                pos_++;
                if (pos_ >= input_.size()) {
                    throw std::runtime_error("Unterminated string");
                }
                
                switch (input_[pos_]) {
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    default:
                        throw std::runtime_error("Invalid escape");
                }
            } else {
                result += input_[pos_];
            }
            pos_++;
        }
        
        if (pos_ >= input_.size()) {
            throw std::runtime_error("Unterminated string");
        }
        
        pos_++;  // Пропускаем закрывающую '"'
        return JSON(result);
    }
    
    JSON parse_number() {
        size_t start = pos_;
        
        if (input_[pos_] == '-') pos_++;
        
        while (pos_ < input_.size() && std::isdigit(input_[pos_])) {
            pos_++;
        }
        
        bool is_float = false;
        
        if (pos_ < input_.size() && input_[pos_] == '.') {
            is_float = true;
            pos_++;
            while (pos_ < input_.size() && std::isdigit(input_[pos_])) {
                pos_++;
            }
        }
        
        if (pos_ < input_.size() && (input_[pos_] == 'e' || input_[pos_] == 'E')) {
            is_float = true;
            pos_++;
            if (input_[pos_] == '+' || input_[pos_] == '-') pos_++;
            while (pos_ < input_.size() && std::isdigit(input_[pos_])) {
                pos_++;
            }
        }
        
        std::string number_str(input_.substr(start, pos_ - start));
        
        if (is_float) {
            return JSON(std::stod(number_str));
        } else {
            return JSON(std::stoll(number_str));
        }
    }
    
    JSON parse_bool() {
        if (input_.substr(pos_, 4) == "true") {
            pos_ += 4;
            return JSON(true);
        }
        if (input_.substr(pos_, 5) == "false") {
            pos_ += 5;
            return JSON(false);
        }
        throw std::runtime_error("Invalid boolean");
    }
    
    JSON parse_null() {
        if (input_.substr(pos_, 4) == "null") {
            pos_ += 4;
            return JSON(nullptr);
        }
        throw std::runtime_error("Invalid null");
    }
};

// Пример парсинга JSON
void json_parsing_example() {
    std::string json_str = R"({
        "name": "John",
        "age": 30,
        "city": "New York",
        "hobbies": ["reading", "gaming"],
        "active": true
    })";
    
    JSONParser parser(json_str);
    JSON data = parser.parse();
    
    std::cout << "Parsed JSON:\n" << data.to_string(true) << '\n';
}

// ============================================
// 📌 Popular Libraries - nlohmann/json
// ============================================

// #include <nlohmann/json.hpp>
// using json = nlohmann::json;

// Automatic serialization с nlohmann/json
struct User {
    std::string name;
    int age;
    std::vector<std::string> hobbies;
    
    // NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, name, age, hobbies)
};

void nlohmann_json_example() {
    // User user{"Alice", 25, {"reading", "coding"}};
    
    // // Сериализация
    // json j = user;
    // std::string serialized = j.dump(2);  // Pretty print с indent 2
    
    // // Десериализация
    // User user2 = j.get<User>();
    
    // // JSON Pointers
    // j["/address/city"_json_pointer] = "New York";
    
    std::cout << "nlohmann/json: высокоуровневая C++ библиотека\n";
}

// ============================================
// 📌 Protocol Buffers
// ============================================

// Proto Definition (.proto файл):
//
// syntax = "proto3";
//
// message Person {
//   string name = 1;
//   int32 id = 2;
//   string email = 3;
//   
//   enum PhoneType {
//     MOBILE = 0;
//     HOME = 1;
//     WORK = 2;
//   }
//   
//   message PhoneNumber {
//     string number = 1;
//     PhoneType type = 2;
//   }
//   
//   repeated PhoneNumber phones = 4;
//   
//   google.protobuf.Timestamp last_updated = 5;
// }
//
// message AddressBook {
//   repeated Person people = 1;
// }

void protobuf_example() {
    // После компиляции .proto -> person.pb.h / person.pb.cc
    
    // Person person;
    // person.set_name("John Doe");
    // person.set_id(1234);
    // person.set_email("john@example.com");
    
    // // Добавление телефона
    // Person::PhoneNumber* phone = person.add_phones();
    // phone->set_number("+1-555-0100");
    // phone->set_type(Person::MOBILE);
    
    // // Сериализация в бинарный формат
    // std::string serialized;
    // person.SerializeToString(&serialized);
    
    // // Десериализация
    // Person person2;
    // person2.ParseFromString(serialized);
    
    // std::cout << "Name: " << person2.name() << '\n';
    // std::cout << "Phones: " << person2.phones_size() << '\n';
    
    std::cout << "Protocol Buffers: компактный бинарный формат от Google\n";
}

// Varint encoding в Protobuf
// Пример: число 300 кодируется как [0xAC, 0x02]
// - Первый байт: 10101100 (младшие 7 бит: 0101100, MSB=1 продолжение)
// - Второй байт: 00000010 (младшие 7 бит: 0000010, MSB=0 конец)
// - Результат: 0000010 0101100 = 300

// Wire types в Protobuf:
// 0: Varint (int32, int64, bool, enum)
// 1: 64-bit (fixed64, double)
// 2: Length-delimited (string, bytes, embedded messages, repeated)
// 5: 32-bit (fixed32, float)

// ============================================
// 📌 MessagePack
// ============================================

#include <cstdint>

// MessagePack - бинарная альтернатива JSON
class MessagePack {
public:
    // Сериализация целого числа
    static std::vector<uint8_t> pack_int(int64_t value) {
        std::vector<uint8_t> result;
        
        if (value >= 0 && value <= 127) {
            // positive fixint
            result.push_back(static_cast<uint8_t>(value));
        }
        else if (value >= -32 && value < 0) {
            // negative fixint
            result.push_back(static_cast<uint8_t>(value));
        }
        else if (value >= 0 && value <= UINT8_MAX) {
            result.push_back(0xcc);  // uint 8
            result.push_back(static_cast<uint8_t>(value));
        }
        else if (value >= INT8_MIN && value <= INT8_MAX) {
            result.push_back(0xd0);  // int 8
            result.push_back(static_cast<uint8_t>(value));
        }
        // ... другие размеры (16, 32, 64 bit)
        
        return result;
    }
    
    // Сериализация строки
    static std::vector<uint8_t> pack_string(const std::string& str) {
        std::vector<uint8_t> result;
        
        if (str.size() <= 31) {
            // fixstr
            result.push_back(0xa0 | static_cast<uint8_t>(str.size()));
        }
        else if (str.size() <= UINT8_MAX) {
            // str 8
            result.push_back(0xd9);
            result.push_back(static_cast<uint8_t>(str.size()));
        }
        else if (str.size() <= UINT16_MAX) {
            // str 16
            result.push_back(0xda);
            result.push_back(static_cast<uint8_t>(str.size() >> 8));
            result.push_back(static_cast<uint8_t>(str.size() & 0xFF));
        }
        
        result.insert(result.end(), str.begin(), str.end());
        return result;
    }
    
    // Сериализация массива
    static std::vector<uint8_t> pack_array_header(size_t size) {
        std::vector<uint8_t> result;
        
        if (size <= 15) {
            // fixarray
            result.push_back(0x90 | static_cast<uint8_t>(size));
        }
        else if (size <= UINT16_MAX) {
            // array 16
            result.push_back(0xdc);
            result.push_back(static_cast<uint8_t>(size >> 8));
            result.push_back(static_cast<uint8_t>(size & 0xFF));
        }
        
        return result;
    }
};

void messagepack_example() {
    // Пример: ["hello", 42, true]
    std::vector<uint8_t> packed;
    
    // Заголовок массива (3 элемента)
    auto header = MessagePack::pack_array_header(3);
    packed.insert(packed.end(), header.begin(), header.end());
    
    // Строка "hello"
    auto str = MessagePack::pack_string("hello");
    packed.insert(packed.end(), str.begin(), str.end());
    
    // Число 42
    auto num = MessagePack::pack_int(42);
    packed.insert(packed.end(), num.begin(), num.end());
    
    // Bool true
    packed.push_back(0xc3);
    
    std::cout << "MessagePack размер: " << packed.size() << " bytes\n";
    
    // MessagePack более компактен чем JSON:
    // JSON: ["hello", 42, true] = 20 bytes
    // MessagePack: [0x93, 0xa5, 'h','e','l','l','o', 0x2a, 0xc3] = 9 bytes
}

// ============================================
// 📌 Binary Serialization (custom)
// ============================================

#include <cstring>

class BinarySerializer {
    std::vector<uint8_t> buffer_;
    
public:
    // Запись с учётом endianness (little-endian)
    void write_uint32(uint32_t value) {
        buffer_.push_back(static_cast<uint8_t>(value & 0xFF));
        buffer_.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
        buffer_.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
        buffer_.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
    }
    
    void write_string(const std::string& str) {
        write_uint32(static_cast<uint32_t>(str.size()));
        buffer_.insert(buffer_.end(), str.begin(), str.end());
    }
    
    // Запись struct с padding awareness
    template<typename T>
    void write_struct(const T& data) {
        static_assert(std::is_trivially_copyable_v<T>,
                     "Type must be trivially copyable");
        
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&data);
        buffer_.insert(buffer_.end(), ptr, ptr + sizeof(T));
    }
    
    const std::vector<uint8_t>& data() const { return buffer_; }
};

class BinaryDeserializer {
    const uint8_t* data_;
    size_t size_;
    size_t pos_ = 0;
    
public:
    BinaryDeserializer(const uint8_t* data, size_t size)
        : data_(data), size_(size) {}
    
    uint32_t read_uint32() {
        if (pos_ + 4 > size_) throw std::runtime_error("Out of bounds");
        
        uint32_t value = data_[pos_]
                       | (data_[pos_ + 1] << 8)
                       | (data_[pos_ + 2] << 16)
                       | (data_[pos_ + 3] << 24);
        pos_ += 4;
        return value;
    }
    
    std::string read_string() {
        uint32_t len = read_uint32();
        if (pos_ + len > size_) throw std::runtime_error("Out of bounds");
        
        std::string result(reinterpret_cast<const char*>(data_ + pos_), len);
        pos_ += len;
        return result;
    }
    
    template<typename T>
    T read_struct() {
        static_assert(std::is_trivially_copyable_v<T>);
        
        if (pos_ + sizeof(T) > size_) throw std::runtime_error("Out of bounds");
        
        T result;
        std::memcpy(&result, data_ + pos_, sizeof(T));
        pos_ += sizeof(T);
        return result;
    }
};

// Versioning в бинарных протоколах
struct MessageHeader {
    uint32_t magic;      // 0xDEADBEEF - идентификатор формата
    uint16_t version;    // Версия протокола
    uint16_t type;       // Тип сообщения
    uint32_t length;     // Длина payload
};

void binary_serialization_example() {
    BinarySerializer ser;
    
    // Header
    MessageHeader header{0xDEADBEEF, 1, 100, 0};
    ser.write_struct(header);
    
    // Payload
    ser.write_string("Hello, binary world!");
    ser.write_uint32(42);
    
    // Можно отправить по сети
    const auto& data = ser.data();
    std::cout << "Binary size: " << data.size() << " bytes\n";
    
    // Десериализация
    BinaryDeserializer deser(data.data(), data.size());
    auto h = deser.read_struct<MessageHeader>();
    
    std::cout << "Version: " << h.version << '\n';
}

// ============================================
// 📌 Performance Comparison
// ============================================

void serialization_performance_comparison() {
    // Сравнение различных форматов:
    
    // JSON (текстовый):
    // + Человекочитаемый
    // + Широкая поддержка
    // - Большой размер
    // - Медленный парсинг
    // Размер: ~100-200% от данных
    
    // Protocol Buffers (бинарный):
    // + Компактный размер
    // + Быстрая сериализация
    // + Схема с версионированием
    // - Требует компиляции .proto
    // Размер: ~20-50% от JSON
    
    // MessagePack (бинарный JSON):
    // + Компактнее JSON
    // + Не требует схемы
    // + Совместим с JSON
    // - Менее компактен чем Protobuf
    // Размер: ~40-70% от JSON
    
    // FlatBuffers (zero-copy):
    // + Zero-copy десериализация
    // + Очень быстрый доступ
    // + Подходит для игр
    // - Сложнее использовать
    // Размер: ~30-60% от JSON
    
    // Cap'n Proto (zero-copy):
    // + Zero-copy сериализация/десериализация
    // + Встроенный RPC
    // + Максимальная производительность
    // - Менее популярен
    
    std::cout << "Выбор формата зависит от требований:\n";
    std::cout << "- REST API: JSON\n";
    std::cout << "- Микросервисы: Protocol Buffers\n";
    std::cout << "- Real-time игры: FlatBuffers\n";
    std::cout << "- Логирование: MessagePack\n";
}
// • JSON vs Protocol Buffers vs MessagePack
// • Serialization speed
// • Deserialization speed
// • Size overhead
// • Use case recommendations

// ============================================
// 📌 Schema Evolution
// ============================================
// • Adding fields
// • Removing fields
// • Renaming fields
// • Changing types
// • Versioning strategies