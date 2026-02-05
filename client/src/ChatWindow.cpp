#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include <QUrl>
#include <QDateTime>
#include <QKeyEvent>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QListWidgetItem>
#include <QTimer>
#include <QColor>
#include <QCoreApplication>
#include <QDir>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>

ChatWindow::ChatWindow(const QString &username,
                       const QString &displayName,
                       const QString &avatarFilename,
                       const QString &status,
                       const QString &serverUrl,
                       QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)
    , m_username(username)
    , m_displayName(displayName)
    , m_avatarFilename(avatarFilename)
    , m_status(status)
    , m_serverUrl(serverUrl)
    , m_webSocketClient(new WebSocketClient(this))
    , m_authClient(new AuthClient(this))
    , m_dragging(false)
    , m_identified(false)
    , m_conversations()
    , m_loginToDisplayName()
    , m_onlineUsers()
{
    ui->setupUi(this);
    
    // Remove native macOS title bar - make window frameless
    setWindowFlags(Qt::FramelessWindowHint);

    // Load QSS stylesheet
    QString qssStyle;
    QFile styleFile(":/styles/msn_xp.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        qssStyle = QLatin1String(styleFile.readAll());
        styleFile.close();
    } else {
        // Fallback: try loading from file system
        QFile fsStyleFile("styles/msn_xp.qss");
        if (fsStyleFile.open(QFile::ReadOnly)) {
            qssStyle = QLatin1String(fsStyleFile.readAll());
            fsStyleFile.close();
        }
    }
    
    // Ensure vintage gradients are applied (fallback if QSS doesn't load)
    QString gradientStyle = 
        "QWidget#ChatWindow {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #6B9BC7,"
        "        stop:0.3 #5A8BB7,"
        "        stop:0.7 #4A7BA7,"
        "        stop:1 #3A6B97);"
        "}"
        "QWidget#titleBar {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #1AA7FF,"
        "        stop:0.05 #0D8FFF,"
        "        stop:0.10 #0066FF,"
        "        stop:0.15 #0055EE,"
        "        stop:0.20 #0044DD,"
        "        stop:0.50 #0036D3,"
        "        stop:0.80 #0028BB,"
        "        stop:0.95 #001FAA,"
        "        stop:1.0 #001899);"
        "}"
        "QLabel#statusLabel {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #1AA7FF,"
        "        stop:0.05 #0D8FFF,"
        "        stop:0.10 #0066FF,"
        "        stop:0.15 #0055EE,"
        "        stop:0.20 #0044DD,"
        "        stop:0.50 #0036D3,"
        "        stop:0.80 #0028BB,"
        "        stop:0.95 #001FAA,"
        "        stop:1.0 #001899);"
        "    padding: 8px;"
        "    border-top: 1px solid #3A6BC1;"
        "}";
    
    this->setStyleSheet(qssStyle + "\n" + gradientStyle);

    // Style title bar buttons (fallback)
    const QString closeButtonStyle =
        "QPushButton#closeButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FF5F57, stop:1 #E0443E);"
        "    border: 1px solid #CC3A35;"
        "    border-radius: 2px; color: white; font-size: 16px;"
        "}"
        "QPushButton#closeButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FF6F67, stop:1 #F0544E);"
        "}";
    ui->closeButton->setStyleSheet(closeButtonStyle);
    
    ui->minimizeButton->setStyleSheet(
        "QPushButton#minimizeButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FFBD2E, stop:1 #E6A826);"
        "    border: 1px solid #CC8F20;"
        "    border-radius: 2px; color: white; font-size: 11px;"
        "}"
        "QPushButton#minimizeButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FFCD3E, stop:1 #F0B836);"
        "}"
    );
    
    ui->maximizeButton->setStyleSheet(
        "QPushButton#maximizeButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #28CA42, stop:1 #22A836);"
        "    border: 1px solid #1D8A2C;"
        "    border-radius: 2px; color: white; font-size: 11px;"
        "}"
        "QPushButton#maximizeButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #38DA52, stop:1 #32B846);"
        "}"
    );

    // Style Send button
    const QString sendButtonStyle =
        "QPushButton#sendButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 rgb(255, 200, 80),"
        "        stop:0.5 rgb(252, 165, 3),"
        "        stop:1 rgb(220, 140, 0));"
        "    border: 1px solid #B87300;"
        "    border-top: 1px solid #FFD700;"
        "    border-left: 1px solid #FFA500;"
        "    border-radius: 3px;"
        "    color: #FFFFFF;"
        "    font-size: 11px;"
        "    font-weight: bold;"
        "    font-family: 'Tahoma', 'Arial', sans-serif;"
        "    padding: 4px;"
        "}"
        "QPushButton#sendButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 rgb(255, 210, 90),"
        "        stop:0.5 rgb(255, 175, 10),"
        "        stop:1 rgb(240, 150, 5));"
        "}"
        "QPushButton#sendButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 rgb(220, 140, 0),"
        "        stop:0.5 rgb(200, 120, 0),"
        "        stop:1 rgb(180, 110, 0));"
        "    border: 1px inset #B87300;"
        "}"
    ;
    ui->sendButton->setStyleSheet(sendButtonStyle);

    // Style Profile button the same as Send button (different object name)
    QString profileButtonStyle = sendButtonStyle;
    profileButtonStyle.replace("QPushButton#sendButton", "QPushButton#profileButton");
    ui->profileButton->setStyleSheet(profileButtonStyle);

    // Style retro dialog buttons based on send button style (no object name selector)
    QString dialogButtonStyle = sendButtonStyle;
    dialogButtonStyle.replace("QPushButton#sendButton", "QPushButton");

    // Connect title bar buttons
    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->minimizeButton, &QPushButton::clicked, this, &QWidget::showMinimized);

    // Profile button: show dedicated profile screen + MSN-style picture dialog
    connect(ui->profileButton, &QPushButton::clicked, this, [this, dialogButtonStyle, closeButtonStyle]() {
        // First show profile screen
        showProfileView();

        QDialog dialog(this);
        dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        dialog.setModal(true);

        QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        // Custom title bar matching main window style
        QWidget *titleBar = new QWidget(&dialog);
        titleBar->setObjectName("titleBar");
        titleBar->setMinimumHeight(28);
        QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
        titleLayout->setContentsMargins(8, 0, 8, 0);
        QLabel *titleLabel = new QLabel("My Display Picture", titleBar);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(
            "color: white; font-size: 11px; font-weight: bold; letter-spacing: 0.5px;"
        );
        titleLayout->addWidget(titleLabel);
        titleLayout->addStretch();

        // Red close button like main window
        QPushButton *closeButton = new QPushButton(titleBar);
        closeButton->setObjectName("closeButton");
        closeButton->setText("×");
        // Slightly larger so it visually matches the main window close button
        closeButton->setFixedSize(23, 21);
        closeButton->setStyleSheet(closeButtonStyle);
        titleLayout->addWidget(closeButton);

        mainLayout->addWidget(titleBar);

        // Content area
        QWidget *contentWidget = new QWidget(&dialog);
        contentWidget->setStyleSheet("background-color: #ECE9D8;"); // retro XP beige
        QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->setContentsMargins(12, 12, 12, 12);
        contentLayout->setSpacing(10);

        // Top form for name + status (retro text fields)
        QFormLayout *topForm = new QFormLayout();
        QLineEdit *nameEdit = new QLineEdit(contentWidget);
        QLineEdit *statusEdit = new QLineEdit(contentWidget);
        nameEdit->setText(m_displayName);
        statusEdit->setText(m_status);
        const QString retroLineEditStyle =
            "QLineEdit {"
            "  border: 1px solid #7F9DB9;"
            "  padding: 2px 3px;"
            "  background-color: white;"
            "  font-family: Tahoma, Arial, sans-serif;"
            "  font-size: 11px;"
            "}";
        nameEdit->setStyleSheet(retroLineEditStyle);
        statusEdit->setStyleSheet(retroLineEditStyle);
        topForm->addRow("Display name:", nameEdit);
        topForm->addRow("Status message:", statusEdit);
        contentLayout->addLayout(topForm);

        // Middle area: avatar list (left) + preview (right)
        QHBoxLayout *middleLayout = new QHBoxLayout();

        // Left: label + list of pictures
        QVBoxLayout *leftLayout = new QVBoxLayout();
        QLabel *displayLabel = new QLabel("Display picture", contentWidget);
        leftLayout->addWidget(displayLabel);

        QListWidget *avatarList = new QListWidget(contentWidget);
        avatarList->setViewMode(QListView::ListMode);
        avatarList->setIconSize(QSize(48, 48));
        avatarList->setSpacing(2);
        avatarList->setMinimumWidth(180);
        avatarList->setStyleSheet(
            "QListWidget {"
            "  border: 1px solid #7F9DB9;"
            "  background-color: white;"
            "  font-family: Tahoma, Arial, sans-serif;"
            "  font-size: 11px;"
            "}"
            "QListWidget::item {"
            "  padding: 2px 3px;"
            "}"
            "QListWidget::item:selected {"
            "  background: #0A64D0;"
            "  color: white;"
            "}"
            "QListWidget::item:hover {"
            "  background: #CDE4FF;"
            "}"
        );
        leftLayout->addWidget(avatarList);

        // Populate avatar list from assets/avatar
        QString avatarsDirPath = QDir::cleanPath(
            QCoreApplication::applicationDirPath() + "/../assets/avatar");
        QDir avatarsDir(avatarsDirPath);
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.jpeg" << "*.gif" << "*.webp";
        QFileInfoList files = avatarsDir.entryInfoList(filters, QDir::Files | QDir::Readable, QDir::Name);

        QString currentAvatar = m_avatarFilename;
        if (currentAvatar.isEmpty()) {
            currentAvatar = "default.png";
        }

        int currentRow = -1;
        int row = 0;
        for (const QFileInfo &fi : files) {
            QPixmap pix(fi.absoluteFilePath());
            QIcon icon(pix);
            QString fileName = fi.fileName();
            QString baseName = fi.completeBaseName();

            QListWidgetItem *item = new QListWidgetItem(icon, baseName, avatarList);
            item->setData(Qt::UserRole, fileName);
            avatarList->addItem(item);

            if (fileName.compare(currentAvatar, Qt::CaseInsensitive) == 0) {
                currentRow = row;
            }
            ++row;
        }

        // Right: Preview
        QVBoxLayout *rightLayout = new QVBoxLayout();
        QLabel *previewLabelTitle = new QLabel("Preview", contentWidget);
        rightLayout->addWidget(previewLabelTitle);

        QLabel *previewLabel = new QLabel(contentWidget);
        previewLabel->setFixedSize(128, 128);
        previewLabel->setFrameShape(QFrame::Box);
        previewLabel->setAlignment(Qt::AlignCenter);
        previewLabel->setStyleSheet(
            "background-color: white;"
            "border: 1px solid #7F9DB9;"
        );
        rightLayout->addWidget(previewLabel);
        rightLayout->addStretch();

        middleLayout->addLayout(leftLayout, 2);
        middleLayout->addLayout(rightLayout, 1);
        contentLayout->addLayout(middleLayout);

        // Bottom buttons: only OK (retro style)
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        QPushButton *okButton = new QPushButton("OK", contentWidget);
        okButton->setStyleSheet(dialogButtonStyle);
        buttonLayout->addWidget(okButton);
        contentLayout->addLayout(buttonLayout);

        mainLayout->addWidget(contentWidget);

        // Keep track of selected avatar filename
        QString selectedAvatar = currentAvatar;

        auto updatePreview = [&]() {
            if (selectedAvatar.isEmpty()) {
                previewLabel->setPixmap(QPixmap());
                previewLabel->setText("No picture");
                return;
            }
            QString path = QDir::cleanPath(avatarsDirPath + "/" + selectedAvatar);
            QPixmap pix(path);
            if (!pix.isNull()) {
                previewLabel->setText(QString());
                previewLabel->setPixmap(pix.scaled(previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                previewLabel->setPixmap(QPixmap());
                previewLabel->setText("No picture");
            }
        };

        // Initialize selection
        if (currentRow >= 0) {
            avatarList->setCurrentRow(currentRow);
        }
        updatePreview();

        QObject::connect(avatarList, &QListWidget::currentItemChanged,
                         &dialog,
                         [&](QListWidgetItem *current, QListWidgetItem * /*previous*/) {
            if (current) {
                selectedAvatar = current->data(Qt::UserRole).toString();
                updatePreview();
            }
        });

        // Close via title-bar red X
        QObject::connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::reject);
        QObject::connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

        if (dialog.exec() == QDialog::Accepted) {
            // Collect proposed values
            QString newDisplayName = nameEdit->text().trimmed().isEmpty()
                                        ? m_username
                                        : nameEdit->text().trimmed();
            QString newAvatar = selectedAvatar;
            QString newStatus = statusEdit->text().trimmed();

            // Send update to server
            m_authClient->updateProfile(m_username, newDisplayName, newAvatar, newStatus);

            // Apply changes locally immediately for a snappy UX
            m_displayName = newDisplayName;
            m_avatarFilename = newAvatar;
            m_status = newStatus;

            // Refresh avatar in header
            QPixmap avatarPixmap;
            if (!m_avatarFilename.isEmpty()) {
                const QString resourcePath = ":/assets/avatar/" + m_avatarFilename;
                QString filePath = QDir::cleanPath(
                    QCoreApplication::applicationDirPath() + "/../assets/avatar/" + m_avatarFilename);
                if (avatarPixmap.load(resourcePath) || avatarPixmap.load(filePath)) {
                    ui->avatarLabel->setPixmap(avatarPixmap);
                }
            }

            // Refresh profile view to show updated info
            showProfileView();
        }
    });

    // Install event filter for Enter key
    ui->messageInput->installEventFilter(this);

    // Configure chat display to accept HTML
    ui->chatDisplay->setAcceptRichText(true);

    // Configure auth client for profile updates
    m_authClient->setServerUrl(m_serverUrl);

    // Show initial home view (no message input)
    showHomeView();

    // Load and display the current user's avatar in the header.
    // Try Qt resource first, then path relative to executable (e.g. build/../assets/avatar/).
    QPixmap avatarPixmap;
    if (!m_avatarFilename.isEmpty()) {
        const QString resourcePath = ":/assets/avatar/" + m_avatarFilename;
        QString filePath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../assets/avatar/" + m_avatarFilename);
        if (avatarPixmap.load(resourcePath)) {
            ui->avatarLabel->setPixmap(avatarPixmap);
        } else if (avatarPixmap.load(filePath)) {
            ui->avatarLabel->setPixmap(avatarPixmap);
        }
    }

    // Connect signals
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatWindow::onSendClicked);
    connect(ui->userList, &QListWidget::itemClicked, this, &ChatWindow::onUserSelected);
    connect(m_webSocketClient, &WebSocketClient::messageReceived, this, &ChatWindow::onMessageReceived);
    connect(m_webSocketClient, &WebSocketClient::connected, this, &ChatWindow::onConnected);
    connect(m_webSocketClient, &WebSocketClient::disconnected, this, &ChatWindow::onDisconnected);
    connect(m_webSocketClient, &WebSocketClient::error, this, &ChatWindow::onError);

    // Convert HTTP URL to WebSocket URL
    QString wsUrl = m_serverUrl;
    if (wsUrl.startsWith("http://")) {
        wsUrl.replace("http://", "ws://");
    } else if (wsUrl.startsWith("https://")) {
        wsUrl.replace("https://", "wss://");
    }
    wsUrl += "/chat";

    // Connect to WebSocket
    m_webSocketClient->connectToServer(QUrl(wsUrl));
}

ChatWindow::~ChatWindow()
{
    delete ui;
    if (m_webSocketClient) {
        m_webSocketClient->disconnect();
    }
}

void ChatWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (ui->titleBar->geometry().contains(event->pos())) {
            m_dragging = true;
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
            return;
        }
    }
    QWidget::mousePressEvent(event);
}

void ChatWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
        return;
    }
    QWidget::mouseMoveEvent(event);
}

void ChatWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void ChatWindow::onSendClicked()
{
    QString message = ui->messageInput->text().trimmed();
    if (message.isEmpty()) {
        return;
    }

    if (!m_webSocketClient->isConnected() || !m_identified) {
        ui->statusLabel->setText("Not connected or not identified!");
        ui->chatDisplay->append("<p style='color: red;'>*** Please wait until connected and identified ***</p>");
        return;
    }

    if (m_currentChatTarget.isEmpty()) {
        ui->chatDisplay->append("<p style='color: orange;'>*** Please select a user from the list to chat with ***</p>");
        return;
    }

    // Send private message
    sendPrivateMessage(m_currentChatTarget, message);
    ui->messageInput->clear();
    
    // Add sent message to conversation
    addMessageToConversation(m_currentChatTarget, message, true);
}

void ChatWindow::onMessageReceived(const QString &message)
{
    qDebug() << "Received message:" << message;
    
    // Parse JSON message from server
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &error);
    
    if (error.error != QJsonParseError::NoError || !doc.isObject()) {
        // Not JSON, treat as plain text (backward compatibility)
        QDateTime now = QDateTime::currentDateTime();
        QString timestamp = now.toString("hh:mm:ss");
        ui->chatDisplay->append("<p>[" + timestamp + "] " + message.toHtmlEscaped() + "</p>");
        return;
    }
    
    QJsonObject json = doc.object();
    handleServerMessage(json);
}

void ChatWindow::onUserSelected()
{
    QListWidgetItem *item = ui->userList->currentItem();
    if (!item) {
        return;
    }

    // Always use the stored username from item data, not the label text,
    // so this still works when the label shows "username (new)".
    QString selectedUser = item->data(Qt::UserRole).toString();

    // Don't allow chatting with yourself
    if (selectedUser == m_username) {
        return;
    }
    
    // Switch to this conversation
    switchToConversation(selectedUser);
}

void ChatWindow::sendIdentify()
{
    QJsonObject identify;
    identify["type"] = "identify";
    identify["username"] = m_username;
    
    QJsonDocument doc(identify);
    QString jsonStr = doc.toJson(QJsonDocument::Compact);
    qDebug() << "[CLIENT] Sending identify message:" << jsonStr;
    m_webSocketClient->sendMessage(jsonStr);
}

void ChatWindow::sendPrivateMessage(const QString &to, const QString &text)
{
    QJsonObject msg;
    msg["type"] = "private_msg";
    msg["to"] = to;
    msg["text"] = text;
    
    QJsonDocument doc(msg);
    QString jsonStr = doc.toJson(QJsonDocument::Compact);
    qDebug() << "[CLIENT] Sending private message:" << jsonStr;
    m_webSocketClient->sendMessage(jsonStr);
}

void ChatWindow::updateUserList(const QJsonArray &users)
{
    ui->userList->clear();

    qDebug() << "Updating user list, current username:" << m_username;

    // Rebuild our local list of online users (excluding self)
    m_onlineUsers.clear();
    m_loginToDisplayName.clear();

    for (const QJsonValue &value : users) {
        QJsonObject obj = value.toObject();
        QString login = obj["login"].toString();
        QString name  = obj["name"].toString().trimmed();
        if (name.isEmpty()) {
            name = login;
        }

        qDebug() << "  - User login:" << login << "name:" << name;

        if (login != m_username) {  // Don't show yourself in the list
            m_onlineUsers.append(name);
            m_loginToDisplayName[login] = name;

            QListWidgetItem *item = new QListWidgetItem(name);
            // Store the real username (login) separately so we always know it,
            // even if we change the visible text to "name (new)".
            item->setData(Qt::UserRole, login);
            item->setFlags(item->flags() | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            // Ensure a readable default background (white) for all users
            item->setBackground(QColor("#FFFFFF"));
            ui->userList->addItem(item);
        }
    }

    qDebug() << "User list now has" << ui->userList->count() << "items";

    // If we're on the home view (no active chat), refresh the text
    if (m_currentChatTarget.isEmpty()) {
        showHomeView();
    }
}

void ChatWindow::handleServerMessage(const QJsonObject &json)
{
    QString type = json["type"].toString();
    qDebug() << "Handling server message type:" << type;
    
    if (type == "identified") {
        m_identified = true;
        ui->statusLabel->setText("Connected as " + m_displayName);
        ui->statusLabel->setStyleSheet("font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7); padding: 8px;");
    }
    else if (type == "user_list") {
        QJsonArray users = json["users"].toArray();
        qDebug() << "Received user list with" << users.size() << "users";
        updateUserList(users);
    }
    else if (type == "private_msg") {
        QString from = json["from"].toString();
        QString text = json["text"].toString();
        
        // Add message to the conversation with this user
        addMessageToConversation(from, text, false);
        
        // If this is NOT from the current chat target, highlight the user in the list
        // but don't switch conversations automatically
        if (from != m_currentChatTarget) {
            // Highlight the user in the list to indicate new message
            for (int i = 0; i < ui->userList->count(); ++i) {
                QListWidgetItem *item = ui->userList->item(i);
                if (item && item->data(Qt::UserRole).toString() == from) {
                    // Highlight with a different background color
                    item->setBackground(QColor("#FFE6E6")); // Light red background
                    // Only add the "(new)" suffix once
                    QString label = item->text();
                    if (!label.endsWith(" (new)")) {
                        item->setText(from + " (new)");
                    }
                    break;
                }
            }
        }
    }
    else if (type == "error") {
        QString errorMsg = json["message"].toString();
        ui->chatDisplay->append("<p style='color: red; font-weight: bold;'>*** Error: " + errorMsg.toHtmlEscaped() + " ***</p>");
        ui->statusLabel->setText("Error: " + errorMsg);
    }
    else {
        qDebug() << "Unknown message type:" << type;
    }
}

void ChatWindow::onConnected()
{
    ui->statusLabel->setText("Connected - Identifying...");
    ui->statusLabel->setStyleSheet("font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7); padding: 8px;");
    ui->chatDisplay->append("<p style='color: green; font-weight: bold;'>*** Connected to server ***</p>");
    
    // Wait a tiny bit for connection to be fully established, then send identification
    qDebug() << "[CLIENT] Connected, will identify as:" << m_username;
    QTimer::singleShot(100, this, [this]() {
        if (m_webSocketClient->isConnected()) {
            sendIdentify();
        } else {
            qDebug() << "[CLIENT] Connection lost before identify could be sent";
        }
    });
}

void ChatWindow::onDisconnected()
{
    m_identified = false;
    ui->statusLabel->setText("Disconnected");
    ui->statusLabel->setStyleSheet("font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7); padding: 8px;");
    ui->chatDisplay->append("<p style='color: red; font-weight: bold;'>*** Disconnected from server ***</p>");
    ui->userList->clear();
}

void ChatWindow::onError(const QString &error)
{
    ui->statusLabel->setText("Error: " + error);
    ui->statusLabel->setStyleSheet("font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7); padding: 8px;");
    ui->chatDisplay->append("<p style='color: red; font-weight: bold;'>*** Error: " + error.toHtmlEscaped() + " ***</p>");
}

bool ChatWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->messageInput && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            onSendClicked();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void ChatWindow::showHomeView()
{
    // No active chat target in home view
    m_currentChatTarget.clear();

    // Header: generic home label
    ui->chatWithLabel->setText("Home");

    // Update custom title bar and window title for home view
    {
        const QString titleText = "Wizz Mania Messenger - Home";
        ui->titleLabel->setText(titleText);
        setWindowTitle(titleText);
    }

    // Hide message input area (read-only home screen)
    ui->messageInput->setVisible(false);
    ui->sendButton->setVisible(false);

    // Initialize content: welcome + online users summary, but no profile details
    ui->chatDisplay->clear();
    ui->chatDisplay->append(
        "<p style='color: #003B75; font-weight: bold;'>Welcome to Wizz Mania Messenger, "
        + m_displayName.toHtmlEscaped() + "!</p>"
    );

    if (m_onlineUsers.isEmpty()) {
        ui->chatDisplay->append(
            "<p style='color: #666; font-style: italic;'>You are connected. "
            "No other users are online yet.</p>"
        );
    } else {
        const int count = m_onlineUsers.size();
        const QString list = m_onlineUsers.join(", ");
        ui->chatDisplay->append(
            "<p style='color: #666;'>Users connected (" + QString::number(count) +
            "): " + list.toHtmlEscaped() + "</p>"
        );
        ui->chatDisplay->append(
            "<p style='color: #666; font-style: italic;'>Select a contact on the left "
            "to start a private conversation.</p>"
        );
    }
}

void ChatWindow::showProfileView()
{
    // No active chat target in profile view
    m_currentChatTarget.clear();

    // Header: show that we're on profile for this user
    ui->chatWithLabel->setText("Profile: " + m_displayName);

    // Update custom title bar and window title for profile view
    {
        const QString titleText = "Wizz Mania Messenger - Profile";
        ui->titleLabel->setText(titleText);
        setWindowTitle(titleText);
    }

    // Hide message input area (read-only profile screen)
    ui->messageInput->setVisible(false);
    ui->sendButton->setVisible(false);

    // Initialize content with current user's info (no server info)
    ui->chatDisplay->clear();
    ui->chatDisplay->append(
        "<p style='color: #003B75; font-weight: bold;'>Your profile</p>"
    );
    ui->chatDisplay->append(
        "<p style='margin-top: 8px;'><b>Username:</b> "
        + m_username.toHtmlEscaped() + "<br>"
        "<b>Display name:</b> " + m_displayName.toHtmlEscaped() + "<br>"
        "<b>Avatar:</b> " + (m_avatarFilename.isEmpty()
            ? QString("default.png").toHtmlEscaped()
            : m_avatarFilename.toHtmlEscaped()) + "<br>"
        "<b>Status:</b> " + m_status.toHtmlEscaped() + "</p>"
    );
}

void ChatWindow::switchToConversation(const QString &username)
{
    // Update current target
    m_currentChatTarget = username;
    const QString displayName = m_loginToDisplayName.value(username, username);
    ui->chatWithLabel->setText("Chat with: " + displayName);

    // Update custom title bar and window title with current chat partner
    {
        const QString titleText = "Wizz Mania Messenger - " + displayName;
        ui->titleLabel->setText(titleText);
        setWindowTitle(titleText);
    }

    // Ensure message input is visible in chat mode
    ui->messageInput->setVisible(true);
    ui->sendButton->setVisible(true);
    
    // Clear and display messages for this conversation
    ui->chatDisplay->clear();
    
    // Display all messages for this conversation (if any)
    if (m_conversations.contains(username)) {
        const QStringList &messages = m_conversations[username];
        for (const QString &msg : messages) {
            ui->chatDisplay->append(msg);
        }
    }
    
    // Remove highlight from user list item
    for (int i = 0; i < ui->userList->count(); ++i) {
        QListWidgetItem *item = ui->userList->item(i);
        if (item && item->data(Qt::UserRole).toString() == username) {
            // Reset background to the default white instead of theme-dependent "null"
            item->setBackground(QColor("#FFFFFF"));
            // Remove "(new)" indicator if present
            QString itemText = item->text();
            if (itemText.endsWith(" (new)")) {
                item->setText(username);
            }
            break;
        }
    }
}

void ChatWindow::addMessageToConversation(const QString &username, const QString &text, bool isFromMe)
{
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("hh:mm:ss");
    
    QString htmlMessage;
    if (isFromMe) {
        htmlMessage = QString("<p>[%1] <b style='color: #0066CC;'>You</b>: %2</p>")
                      .arg(timestamp)
                      .arg(text.toHtmlEscaped());
    } else {
        QString displayName = m_loginToDisplayName.value(username, username);
        htmlMessage = QString("<p>[%1] <b style='color: #CC0066;'>%2</b>: %3</p>")
                      .arg(timestamp)
                      .arg(displayName.toHtmlEscaped())
                      .arg(text.toHtmlEscaped());
    }
    
    // Store message in conversation
    if (!m_conversations.contains(username)) {
        m_conversations[username] = QStringList();
    }
    m_conversations[username].append(htmlMessage);
    
    // Only display if this is the current conversation
    if (username == m_currentChatTarget) {
        ui->chatDisplay->append(htmlMessage);
    }
}
