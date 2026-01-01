// ============================================
// 📌 GraphQL Basics
// ============================================
// • Schema definition (SDL)
// • Types (Scalar, Object, Interface, Union, Enum)
// • Queries
// • Mutations
// • Subscriptions
// • Resolvers

// ============================================
// 📌 Schema Definition
// ============================================

/*
type User {
  id: ID!
  name: String!
  email: String!
  posts: [Post!]!
}

type Post {
  id: ID!
  title: String!
  content: String!
  author: User!
}

type Query {
  user(id: ID!): User
  users(limit: Int): [User!]!
}

type Mutation {
  createUser(input: CreateUserInput!): User!
}

type Subscription {
  userCreated: User!
}
*/

// ============================================
// 📌 C++ GraphQL Server
// ============================================

// --- Using graphqlservice library ---
// • Schema parsing
// • Resolver implementation
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