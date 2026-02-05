// Crow-based WebSocket chat server for WizzMania
//
// Requirements:
// - Crow installed and available as "crow.h" in your include path
// - A C++17 compiler
//
// Build (from the server/ directory, if using the provided CMakeLists.txt):
//   mkdir build && cd build
//   cmake .. && cmake --build .
//
// Run:
//   ./wizzmania_server
//
// WebSocket test (e.g. Postman, Insomnia, or browser/WebSocket client):
//   URL: ws://localhost:18080/chat
//
// HTTP test route (easy to try in Postman using plain HTTP GET):
//   GET http://localhost:18080/health
//   → returns: 200 OK with body "OK"

#include <set>
#include <mutex>
#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include "crow.h"  // Make sure this header is available in your include path
#include <sqlite3.h>

int main() {
    crow::SimpleApp app;

    // Store active WebSocket connections mapped to usernames
    std::map<crow::websocket::connection*, std::string> connection_to_user;
    std::map<std::string, crow::websocket::connection*> user_to_connection;
    std::mutex connections_mutex;

    // SQLite database handle for users
    sqlite3* db = nullptr;
    std::mutex auth_mutex;

    const char* DB_PATH = "wizzmania.db";

    // Open (or create) SQLite database
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
        std::cerr << "Failed to open SQLite database at " << DB_PATH
                  << ": " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Create users table if it does not exist yet
    const char* create_table_sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "  id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  login    TEXT UNIQUE NOT NULL,"
        "  password TEXT NOT NULL,"
        "  name     TEXT,"
        "  picture  TEXT,"
        "  status   TEXT"
        ");";

    {
        char* err_msg = nullptr;
        if (sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
            std::cerr << "Failed to create users table: " << err_msg << std::endl;
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return 1;
        }
    }

    // Simple HTTP route to verify that the server is running.
    // Test with Postman:
    //   Method: GET
    //   URL:    http://localhost:18080/health
    CROW_ROUTE(app, "/health")
    ([] {
        return "OK";
    });

    // Authentication: Register a new user (stored in SQLite)
    // POST /auth/register
    // Body (JSON): {
    //   "username": "alice",
    //   "password": "secret123",
    //   "name": "Alice Smith",        // optional
    //   "picture": "/img/alice.png",  // optional
    //   "status": "Hey there!"        // optional
    // }
    // Returns: {"success": true, "message": "User registered"} or error
    CROW_ROUTE(app, "/auth/register")
        .methods("POST"_method)
        ([&](const crow::request& req) {
            try {
                auto json = crow::json::load(req.body);
                if (!json || !json.has("username") || !json.has("password")) {
                    crow::response res(400);
                    res.body = R"({"success": false, "error": "Missing username or password"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                std::string username = json["username"].s();
                std::string password = json["password"].s();

                // Optional fields
                std::string name;
                // Default profile picture filename
                std::string picture = "default.png";
                std::string status;
                if (json.has("name")) {
                    name = json["name"].s();
                }
                if (json.has("picture")) {
                    picture = json["picture"].s();
                }
                if (json.has("status")) {
                    status = json["status"].s();
                }

                if (username.empty() || password.empty()) {
                    crow::response res(400);
                    res.body = R"({"success": false, "error": "Username and password cannot be empty"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                std::lock_guard<std::mutex> lock(auth_mutex);

                sqlite3_stmt* stmt = nullptr;
                const char* sql =
                    "INSERT INTO users (login, password, name, picture, status) "
                    "VALUES (?, ?, ?, ?, ?);";

                if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
                    crow::response res(500);
                    res.body = R"({"success": false, "error": "Failed to prepare statement"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 3, name.c_str(),     -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 4, picture.c_str(),  -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 5, status.c_str(),   -1, SQLITE_TRANSIENT);

                int rc = sqlite3_step(stmt);
                sqlite3_finalize(stmt);

                if (rc == SQLITE_CONSTRAINT) {
                    crow::response res(409);  // Conflict
                    res.body = R"({"success": false, "error": "Username already exists"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }
                if (rc != SQLITE_DONE) {
                    crow::response res(500);
                    res.body = R"({"success": false, "error": "Failed to insert user"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                crow::response res(201);
                res.body = R"({"success": true, "message": "User registered successfully"})";
                res.set_header("Content-Type", "application/json");
                return res;
            }
            catch (...) {
                crow::response res(500);
                res.body = R"({"success": false, "error": "Internal server error"})";
                res.set_header("Content-Type", "application/json");
                return res;
            }
        });

    // Authentication: Login
    // POST /auth/login
    // Body (JSON): {"username": "alice", "password": "secret123"}
    // Returns on success:
    //   {
    //     "success": true,
    //     "username": "alice",
    //     "name": "Alice Smith",
    //     "picture": "/img/alice.png",
    //     "status": "Hey there!"
    //   }
    CROW_ROUTE(app, "/auth/login")
        .methods("POST"_method)
        ([&](const crow::request& req) {
            try {
                auto json = crow::json::load(req.body);
                if (!json || !json.has("username") || !json.has("password")) {
                    crow::response res(400);
                    res.body = R"({"success": false, "error": "Missing username or password"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                std::string username = json["username"].s();
                std::string password = json["password"].s();

                std::lock_guard<std::mutex> lock(auth_mutex);

                // Fetch user from DB
                sqlite3_stmt* stmt = nullptr;
                const char* sql =
                    "SELECT password, name, picture, status "
                    "FROM users WHERE login = ?;";

                if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
                    crow::response res(500);
                    res.body = R"({"success": false, "error": "Internal server error"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

                int rc = sqlite3_step(stmt);
                if (rc != SQLITE_ROW) {
                    sqlite3_finalize(stmt);
                    crow::response res(401);  // Unauthorized
                    res.body = R"({"success": false, "error": "Invalid username or password"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                std::string stored_password =
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                std::string name =
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1) ? sqlite3_column_text(stmt, 1) : reinterpret_cast<const unsigned char*>(""));
                std::string picture =
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2) ? sqlite3_column_text(stmt, 2) : reinterpret_cast<const unsigned char*>(""));
                std::string status =
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3) ? sqlite3_column_text(stmt, 3) : reinterpret_cast<const unsigned char*>(""));

                sqlite3_finalize(stmt);

                if (stored_password != password) {
                    crow::response res(401);  // Unauthorized
                    res.body = R"({"success": false, "error": "Invalid username or password"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                crow::response res(200);
                res.body = R"({"success": true, "username": ")" + username +
                           R"(", "name": ")" + name +
                           R"(", "picture": ")" + picture +
                           R"(", "status": ")" + status + R"("})";
                res.set_header("Content-Type", "application/json");
                return res;
            }
            catch (...) {
                crow::response res(500);
                res.body = R"({"success": false, "error": "Internal server error"})";
                res.set_header("Content-Type", "application/json");
                return res;
            }
        });

    // Authentication: Update profile (name, picture, status)
    // POST /auth/update
    // Body (JSON): {
    //   "username": "alice",
    //   "name": "New Name",
    //   "picture": "NewAvatar.png",
    //   "status": "New status"
    // }
    // Returns: {"success": true} or error
    CROW_ROUTE(app, "/auth/update")
        .methods("POST"_method)
        ([&](const crow::request& req) {
            try {
                auto json = crow::json::load(req.body);
                if (!json || !json.has("username")) {
                    crow::response res(400);
                    res.body = R"({"success": false, "error": "Missing username"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                std::string username = json["username"].s();

                std::string name;
                if (json.has("name")) {
                    name = json["name"].s();
                }

                std::string picture;
                if (json.has("picture")) {
                    picture = json["picture"].s();
                }

                std::string status;
                if (json.has("status")) {
                    status = json["status"].s();
                }

                std::lock_guard<std::mutex> lock(auth_mutex);

                sqlite3_stmt* stmt = nullptr;
                const char* sql =
                    "UPDATE users SET name = ?, picture = ?, status = ? WHERE login = ?;";

                if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
                    crow::response res(500);
                    res.body = R"({"success": false, "error": "Failed to prepare statement"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                sqlite3_bind_text(stmt, 1, name.c_str(),    -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 2, picture.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 3, status.c_str(),  -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 4, username.c_str(),-1, SQLITE_TRANSIENT);

                int rc = sqlite3_step(stmt);
                sqlite3_finalize(stmt);

                if (rc != SQLITE_DONE) {
                    crow::response res(500);
                    res.body = R"({"success": false, "error": "Failed to update user"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                // Check that at least one row was updated
                if (sqlite3_changes(db) == 0) {
                    crow::response res(404);
                    res.body = R"({"success": false, "error": "User not found"})";
                    res.set_header("Content-Type", "application/json");
                    return res;
                }

                crow::response res(200);
                res.body = R"({"success": true})";
                res.set_header("Content-Type", "application/json");
                return res;
            }
            catch (...) {
                crow::response res(500);
                res.body = R"({"success": false, "error": "Internal server error"})";
                res.set_header("Content-Type", "application/json");
                return res;
            }
        });

    // Get list of all registered users
    // GET /auth/users
    // Returns: {"success": true, "users": [{"login": "...", "name": "..."}, ...]} or error
    CROW_ROUTE(app, "/auth/users")
        .methods("GET"_method)
        ([&](const crow::request& req) {
            std::lock_guard<std::mutex> lock(auth_mutex);

            sqlite3_stmt* stmt = nullptr;
            const char* sql = "SELECT login, name FROM users ORDER BY login ASC;";

            if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
                crow::response res(500);
                res.body = R"({"success": false, "error": "Internal server error"})";
                res.set_header("Content-Type", "application/json");
                return res;
            }

            std::string json_response = R"({"success": true, "users": [)";
            bool first = true;

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string login =
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                const unsigned char* name_text = sqlite3_column_text(stmt, 1);
                std::string name = name_text
                    ? reinterpret_cast<const char*>(name_text)
                    : "";

                // Fallback: if no display name stored, use login as name.
                if (name.empty()) {
                    name = login;
                }

                if (!first) json_response += ",";
                json_response += R"({"login": ")" + login + R"(", "name": ")" + name + R"("})";
                first = false;
            }

            sqlite3_finalize(stmt);

            json_response += "]}";

            crow::response res(200);
            res.body = json_response;
            res.set_header("Content-Type", "application/json");
            return res;
        });

    // Helper function to broadcast user list to all connected users
    // Sends: {"type":"user_list","users":[{"login":"alice123","name":"Alice"}, ...]}
    auto broadcast_user_list = [&]() {
        std::string json_users = "[";
        bool first = true;

        // Prepare statement once per broadcast
        sqlite3_stmt* stmt = nullptr;
        const char* sql = "SELECT name FROM users WHERE login = ?;";

        for (const auto& [username, conn] : user_to_connection) {
            std::string display_name = username;

            if (db) {
                if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
                    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
                    int rc = sqlite3_step(stmt);
                    if (rc == SQLITE_ROW) {
                        const unsigned char* name_text = sqlite3_column_text(stmt, 0);
                        if (name_text) {
                            std::string fetched_name =
                                reinterpret_cast<const char*>(name_text);
                            if (!fetched_name.empty()) {
                                display_name = fetched_name;
                            }
                        }
                    }
                    sqlite3_finalize(stmt);
                    stmt = nullptr;
                }
            }

            if (!first) json_users += ",";
            json_users += R"({"login": ")" + username +
                          R"(", "name": ")" + display_name + R"("})";
            first = false;
        }

        json_users += "]";

        std::string message = R"({"type": "user_list", "users": )" + json_users + "}";
        std::cout << "[SERVER] Broadcasting user list: " << message << std::endl;

        for (const auto& [conn, username] : connection_to_user) {
            if (conn) {
                conn->send_text(message);
            }
        }
    };

    // Basic WebSocket chat endpoint with private messaging support
    //
    // WebSocket URL: ws://localhost:18080/chat
    //
    // Message Protocol:
    //   - Identify: {"type": "identify", "username": "alice"}
    //   - Private:   {"type": "private_msg", "to": "bob", "text": "Hello!"}
    //   - Public:    {"type": "public_msg", "text": "Hello everyone!"}
    //
    // Server Responses:
    //   - User List: {"type": "user_list", "users": ["alice", "bob"]}
    //   - Private:   {"type": "private_msg", "from": "alice", "text": "Hello!"}
    //   - Public:    {"type": "public_msg", "from": "alice", "text": "Hello!"}
    CROW_WEBSOCKET_ROUTE(app, "/chat")
        .onopen([&](crow::websocket::connection& conn) {
            // Connection opened, but user not identified yet
            // User will identify themselves with an "identify" message
            std::cout << "[SERVER] New WebSocket connection opened" << std::endl;
        })
        .onclose([&](crow::websocket::connection& conn,
            const std::string& /*reason*/,
            unsigned short /*code*/) {
            std::lock_guard<std::mutex> lock(connections_mutex);
            
            // Remove user from maps
            auto it = connection_to_user.find(&conn);
            if (it != connection_to_user.end()) {
                std::string username = it->second;
                std::cout << "[SERVER] User " << username << " disconnected" << std::endl;
                user_to_connection.erase(username);
                connection_to_user.erase(it);
                
                // Broadcast updated user list to remaining users
                broadcast_user_list();
            } else {
                std::cout << "[SERVER] Connection closed (user was not identified)" << std::endl;
            }
        })
        .onmessage([&](crow::websocket::connection& conn,
                       const std::string& data,
                       bool is_binary) {
            std::lock_guard<std::mutex> lock(connections_mutex);

            if (is_binary) {
                std::cout << "[SERVER] Received binary message (ignored)" << std::endl;
                return;
            }

            std::cout << "[SERVER] Received message: " << data << std::endl;

            try {
                auto json = crow::json::load(data);
                if (!json || !json.has("type")) {
                    std::cout << "[SERVER] Invalid JSON or missing 'type' field. Message was: " << data << std::endl;
                    std::cout << "[SERVER] Expected JSON format like: {\"type\": \"identify\", \"username\": \"alice\"}" << std::endl;
                    
                    // Check if this is an old-format message (e.g., "[username]: message")
                    // If so, we need to identify the sender first
                    auto conn_it = connection_to_user.find(&conn);
                    if (conn_it == connection_to_user.end()) {
                        std::cout << "[SERVER] User not identified, cannot process old-format message" << std::endl;
                        std::string error = R"({"type": "error", "message": "Please identify yourself first with: {\"type\": \"identify\", \"username\": \"yourname\"}"})";
                        conn.send_text(error);
                        return;
                    }
                    
                    // Old format message - try to parse it
                    // Format: "[username]: message" - but we already know the sender
                    std::string from_username = conn_it->second;
                    std::string error_msg = R"({"type": "error", "message": "Invalid message format. Use: {\"type\": \"private_msg\", \"to\": \"username\", \"text\": \"message\"}"})";
                    conn.send_text(error_msg);
                    return;
                }

                std::string msg_type = json["type"].s();
                std::cout << "[SERVER] Message type: " << msg_type << std::endl;

                // Handle user identification
                if (msg_type == "identify") {
                    if (!json.has("username")) {
                        std::cout << "[SERVER] Identify message missing username" << std::endl;
                        return;
                    }
                    std::string username = json["username"].s();
                    std::cout << "[SERVER] User identifying as: " << username << std::endl;
                    
                    // Check if username is already connected
                    if (user_to_connection.find(username) != user_to_connection.end()) {
                        // Username already in use, send error
                        std::string error = R"({"type": "error", "message": "Username already in use"})";
                        std::cout << "[SERVER] Username already in use: " << username << std::endl;
                        conn.send_text(error);
                        return;
                    }
                    
                    // Add user to maps
                    connection_to_user[&conn] = username;
                    user_to_connection[username] = &conn;
                    std::cout << "[SERVER] User " << username << " added. Total users: " << user_to_connection.size() << std::endl;
                    
                    // Send confirmation
                    std::string confirm = R"({"type": "identified", "username": ")" + username + R"("})";
                    std::cout << "[SERVER] Sending confirmation: " << confirm << std::endl;
                    conn.send_text(confirm);
                    
                    // Broadcast updated user list to all users
                    broadcast_user_list();
                    return;
                }

                // Check if user is identified
                auto conn_it = connection_to_user.find(&conn);
                if (conn_it == connection_to_user.end()) {
                    // User not identified yet
                    std::string error = R"({"type": "error", "message": "Please identify yourself first"})";
                    conn.send_text(error);
                    return;
                }

                std::string from_username = conn_it->second;

                // Handle private message
                if (msg_type == "private_msg") {
                    if (!json.has("to") || !json.has("text")) {
                        std::cout << "[SERVER] Private message missing 'to' or 'text' field" << std::endl;
                        return;
                    }
                    std::string to_username = json["to"].s();
                    std::string text = json["text"].s();
                    std::cout << "[SERVER] Private message from " << from_username << " to " << to_username << ": " << text << std::endl;
                    
                    // Find target user's connection
                    auto target_it = user_to_connection.find(to_username);
                    if (target_it != user_to_connection.end() && target_it->second) {
                        // Send private message to target user
                        // Escape quotes in text for JSON
                        std::string escaped_text = text;
                        size_t pos = 0;
                        while ((pos = escaped_text.find("\"", pos)) != std::string::npos) {
                            escaped_text.replace(pos, 1, "\\\"");
                            pos += 2;
                        }
                        while ((pos = escaped_text.find("\n", pos)) != std::string::npos) {
                            escaped_text.replace(pos, 1, "\\n");
                            pos += 2;
                        }
                        
                        std::string msg_str = R"({"type": "private_msg", "from": ")" + from_username + 
                                             R"(", "text": ")" + escaped_text + R"("})";
                        std::cout << "[SERVER] Sending private message: " << msg_str << std::endl;
                        target_it->second->send_text(msg_str);
                        
                        // Also send confirmation to sender
                        std::string confirm = R"({"type": "private_sent", "to": ")" + to_username + R"("})";
                        conn.send_text(confirm);
                    } else {
                        // User not found
                        std::cout << "[SERVER] User not found: " << to_username << std::endl;
                        std::string error = R"({"type": "error", "message": "User not found or offline"})";
                        conn.send_text(error);
                    }
                    return;
                }

                // Handle public message (broadcast to all)
                if (msg_type == "public_msg") {
                    if (!json.has("text")) {
                        return;
                    }
                    std::string text = json["text"].s();
                    
                    // Broadcast to all connected users
                    // Escape quotes in text for JSON
                    std::string escaped_text = text;
                    size_t pos = 0;
                    while ((pos = escaped_text.find("\"", pos)) != std::string::npos) {
                        escaped_text.replace(pos, 1, "\\\"");
                        pos += 2;
                    }
                    while ((pos = escaped_text.find("\n", pos)) != std::string::npos) {
                        escaped_text.replace(pos, 1, "\\n");
                        pos += 2;
                    }
                    
                    std::string public_msg = R"({"type": "public_msg", "from": ")" + from_username + 
                                           R"(", "text": ")" + escaped_text + R"("})";
                    
                    for (const auto& [target_conn, target_user] : connection_to_user) {
                        if (target_conn) {
                            target_conn->send_text(public_msg);
                        }
                    }
                    return;
                }
            }
            catch (const std::exception& e) {
                // Invalid JSON or parsing error
                std::cout << "[SERVER] Error parsing message: " << e.what() << std::endl;
                std::string error = R"({"type": "error", "message": "Invalid message format"})";
                conn.send_text(error);
            }
            catch (...) {
                // Invalid JSON or parsing error
                std::cout << "[SERVER] Unknown error parsing message" << std::endl;
                std::string error = R"({"type": "error", "message": "Invalid message format"})";
                conn.send_text(error);
            }
        });

    // Start the server on port 18080 with multiple worker threads.
    // You can change the port if needed.
    app.port(18080)
        .multithreaded()
        .run();

    if (db) {
        sqlite3_close(db);
    }
    return 0;
}

