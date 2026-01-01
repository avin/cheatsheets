// ============================================
// 📌 GraphQL Basics
// ============================================

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <variant>

// GraphQL - язык запросов для API
// Преимущества:
// • Клиент запрашивает только нужные поля
// • Один endpoint вместо множества REST маршрутов
// • Сильная типизация
// • Introspection - самодокументирование

// ============================================
// 📌 Schema Definition
// ============================================

// GraphQL Schema Definition Language (SDL)
const char* schema_sdl = R"(
type User {
  id: ID!
  name: String!
  email: String!
  posts: [Post!]!
  createdAt: String!
}

type Post {
  id: ID!
  title: String!
  content: String!
  author: User!
  published: Boolean!
}

input CreateUserInput {
  name: String!
  email: String!
}

type Query {
  user(id: ID!): User
  users(limit: Int, offset: Int): [User!]!
  post(id: ID!): Post
}

type Mutation {
  createUser(input: CreateUserInput!): User!
  updateUser(id: ID!, name: String): User
  deleteUser(id: ID!): Boolean!
}

type Subscription {
  userCreated: User!
  postPublished: Post!
}
)";

// GraphQL запрос пример:
/*
query GetUserWithPosts {
  user(id: "123") {
    id
    name
    email
    posts {
      id
      title
      published
    }
  }
}

mutation CreateNewUser {
  createUser(input: { name: "Alice", email: "alice@example.com" }) {
    id
    name
  }
}

subscription OnUserCreated {
  userCreated {
    id
    name
  }
}
*/

// ============================================
// 📌 C++ GraphQL Server
// ============================================

using GraphQLValue = std::variant<int, std::string, bool, std::nullptr_t>;

// Resolver context
struct ResolverContext {
    std::unordered_map<std::string, GraphQLValue> variables;
    std::string auth_token;
};

// User модель
struct User {
    std::string id;
    std::string name;
    std::string email;
    std::vector<std::string> post_ids;
    
    std::string to_json() const {
        return "{\"id\":\"" + id + "\",\"name\":\"" + name + 
               "\",\"email\":\"" + email + "\"}";
    }
};

struct Post {
    std::string id;
    std::string title;
    std::string content;
    std::string author_id;
    bool published;
};

// Resolvers - функции для получения данных
class GraphQLResolvers {
private:
    std::unordered_map<std::string, User> users;
    std::unordered_map<std::string, Post> posts;
    
public:
    // Query: user(id: ID!)
    std::optional<User> resolve_user(const std::string& id, const ResolverContext& ctx) {
        auto it = users.find(id);
        return it != users.end() ? std::optional(it->second) : std::nullopt;
    }
    
    // Query: users(limit: Int, offset: Int)
    std::vector<User> resolve_users(int limit, int offset, const ResolverContext& ctx) {
        std::vector<User> result;
        int count = 0;
        int idx = 0;
        
        for (const auto& [id, user] : users) {
            if (idx++ < offset) continue;
            if (count++ >= limit) break;
            result.push_back(user);
        }
        
        return result;
    }
    
    // User.posts resolver - вложенное поле
    std::vector<Post> resolve_user_posts(const User& user, const ResolverContext& ctx) {
        std::vector<Post> result;
        for (const auto& post_id : user.post_ids) {
            auto it = posts.find(post_id);
            if (it != posts.end()) {
                result.push_back(it->second);
            }
        }
        return result;
    }
    
    // Mutation: createUser
    User resolve_create_user(const std::string& name, const std::string& email,
                            const ResolverContext& ctx) {
        User user;
        user.id = std::to_string(users.size() + 1);
        user.name = name;
        user.email = email;
        
        users[user.id] = user;
        return user;
    }
    
    // Subscription handler
    void on_user_created(std::function<void(const User&)> callback) {
        // Подписка на события
    }
};

// ============================================
// 📌 Query Execution
// ============================================

// Упрощенный GraphQL executor
class GraphQLExecutor {
private:
    GraphQLResolvers resolvers;
    
public:
    std::string execute_query(const std::string& query, 
                            const std::unordered_map<std::string, GraphQLValue>& variables) {
        // Парсинг query
        // Вызов resolvers
        // Формирование JSON ответа
        
        // Пример ответа:
        return R"(
        {
          "data": {
            "user": {
              "id": "123",
              "name": "John Doe",
              "email": "john@example.com",
              "posts": [
                {"id": "1", "title": "First Post", "published": true}
              ]
            }
          }
        }
        )";
    }
};
// • Request execution
// • Response generation

// --- Resolver Implementation ---
class UserResolver {
    // • resolve_user()
    // • resolve_users()
    // • Field resolvers (posts)
    // • Lazy loading
};

// ============================================
// 📌 Query Execution
// ============================================
// • Query parsing
// • Query validation
// • Field resolution
// • N+1 problem solution (DataLoader)
// • Batching
// • Caching

// ============================================
// 📌 Mutations
// ============================================
// • Input validation
// • Side effects
// • Optimistic updates
// • Error handling
// • Transaction support

// ============================================
// 📌 Subscriptions
// ============================================
// • WebSocket transport
// • Event publishing
// • Subscription filtering
// • Real-time updates
// • Scalability

// ============================================
// 📌 Authentication & Authorization
// ============================================
// • Context injection
// • Field-level permissions
// • Directive-based auth (@auth)
// • Role-based access

// ============================================
// 📌 Performance Optimization
// ============================================
// • Query complexity analysis
// • Depth limiting
// • DataLoader pattern
// • Caching strategies
// • Persistent queries

// ============================================
// 📌 GraphQL vs REST
// ============================================
// • Over-fetching / Under-fetching
// • Single endpoint
// • Versioning
// • Caching differences
// • When to use which