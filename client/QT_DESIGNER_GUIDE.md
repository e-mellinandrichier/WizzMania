# Using Qt Designer with WizzMania Client

## UI Files Location

The UI files are located in: `client/ui/`
- `LoginWindow.ui` - Login/Register window
- `ChatWindow.ui` - Chat interface window

## Opening UI Files in Qt Designer

### Method 1: From Qt Creator
1. Open the project in Qt Creator (open `client/CMakeLists.txt`)
2. In the project tree, navigate to `ui/` folder
3. Double-click on `LoginWindow.ui` or `ChatWindow.ui`
4. Qt Designer will open in the editor

### Method 2: Standalone Qt Designer
1. Open Qt Designer (usually installed with Qt)
2. File → Open
3. Navigate to `client/ui/LoginWindow.ui` or `client/ui/ChatWindow.ui`
4. Click Open

## Editing UI Files

### Important Notes:
- **Object Names**: The object names in the UI file (like `serverInput`, `usernameInput`, etc.) must match what the code expects
- **Widget Types**: Don't change widget types (QLineEdit, QPushButton, etc.) without updating the code
- **Layouts**: You can rearrange widgets, but keep the layout structure

### Current Object Names:

**LoginWindow.ui:**
- `serverInput` - Server URL input
- `usernameInput` - Username input
- `passwordInput` - Password input
- `registerButton` - Register button
- `loginButton` - Login button
- `statusLabel` - Status message label

**ChatWindow.ui:**
- `statusLabel` - Connection status label
- `chatDisplay` - Message display area (QTextEdit)
- `messageInput` - Message input field
- `sendButton` - Send button

## After Editing

1. Save the `.ui` file
2. Rebuild the project in Qt Creator (Build → Rebuild Project)
3. The generated header files (`ui_LoginWindow.h`, `ui_ChatWindow.h`) will be automatically updated
4. Run the application to see your changes

## Tips

- Use the Property Editor to change colors, fonts, sizes, etc.
- Use the Layout Editor to adjust spacing and alignment
- Preview your design with Form → Preview (Ctrl+R / Cmd+R)
- Don't delete or rename widgets that are referenced in the C++ code
