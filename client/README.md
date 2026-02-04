# WizzMania Client

Qt desktop client application for the WizzMania chat server.

## Requirements

- Qt6 (Core, Widgets, WebSockets, Network)
- CMake 3.16+
- C++17 compiler

## Building

From the `client/` directory:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running

1. Make sure the server is running (from `server/build/`):
   ```bash
   ./wizzmania_server
   ```

2. Run the client (from `client/build/`):
   ```bash
   ./wizzmania_client
   ```

## Usage

1. **Login Window**:
   - Enter server URL (default: `http://localhost:18080`)
   - Enter username and password
   - Click "Register" to create a new account
   - Click "Login" to connect with existing credentials

2. **Chat Window**:
   - After successful login, the chat window opens
   - Type messages in the input field
   - Press Enter or click "Send" to send messages
   - Messages from all users appear in the chat display

## Testing

To test with multiple clients:
1. Start the server
2. Run multiple instances of the client
3. Register/login with different usernames
4. Send messages - they should appear in all connected clients
