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
#include <QComboBox>
#include <QColorDialog>
#include <QPropertyAnimation>
#include <QApplication>

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
    , m_myTextColor(Qt::black)
    , m_myFontSizePx(12)
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
        "    border-left: 10px solid #1AA7FF;"
        "    border-right: 10px solid #1AA7FF;"
        "    border-radius: 10px;"
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

    // Emoji button: open simple emoji picker (only emojis defined in applyEmojiShortcuts)
    connect(ui->emojiButton, &QPushButton::clicked, this, [this]() {
        QDialog dialog(this);
        dialog.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        dialog.setModal(true);

        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        layout->setContentsMargins(4, 4, 4, 4);
        layout->setSpacing(4);

        QGridLayout *grid = new QGridLayout();
        grid->setSpacing(2);

        QString emojisDirPath = QDir::cleanPath(
            QCoreApplication::applicationDirPath() + "/../assets/emojis");

        // Keep emoji list in sync with applyEmojiShortcuts
        static const QList<QPair<QString, QString>> shortcutToFile = {
            {":D",   "teeth_smile.png"},
            {";)",   "03.png"},
            {":)",   "001.png"},
            {":-O",  "omg_smile.png"},
            {":P",   "tongue_smile.png"},
            {"(H)",  "shades_smile.png"},
            {":@",   "angry_smile.png"},
            {":S",   "confused_smile.png"},
            {":$",   "red_smile.png"},
            {":(",   "sad_smile.png"},
            {":'(",  "surprised_smile.png"}, // missing file?
            {":|",   "what_face.png"},
            {"(A)",  "angel_smile.png"},
            {"8o|",  "48_48.png"},
            {"8-|",  "49_49.png"},
            {"+o(",  "52_52.png"},
            {"|-)",  "77_77.png"},
            {"*-)",  "72_72.png"},
            {":-#",  "048.png"},
            {":-*",  "51_51.png"},
            {"^o)",  "50_50.png"},
            {"8-)",  "71_71.png"},
            {"(L)",  "015.png"},
            {"(U)",  "016.png"},
            {"(M)",  "017.png"},
            {"(@)",  "018.png"},
            {"(&)",  "019.png"},
            {"(sn)", "054.png"},
            {"(bah)","071.png"},
            {"(S)",  "020.png"},
            {"(*)",  "021.png"},
            {"(#)",  "046.png"},
            {"(R)",  "047.png"},
            {"({)",  "035.png"},
            {"(})",  "036.png"},
            {"(K)",  "kiss.png"},
            {"(F)",  "025.png"},
        };

        int row = 0;
        int col = 0;
        const int maxCols = 8;

        for (const auto &pair : shortcutToFile) {
            const QString &shortcut = pair.first;
            const QString &file     = pair.second;

            QString path = QDir::cleanPath(emojisDirPath + "/" + file);
            QPixmap pix(path);
            if (pix.isNull())
                continue;

            QIcon icon(pix);
            QPushButton *btn = new QPushButton(&dialog);
            btn->setIcon(icon);
            btn->setIconSize(QSize(19, 19));
            btn->setFixedSize(24, 24);
            btn->setFlat(true);

            QObject::connect(btn, &QPushButton::clicked, &dialog, [this, shortcut, &dialog]() {
                // Insert shortcut at cursor position
                QString current = ui->messageInput->text();
                int pos = ui->messageInput->cursorPosition();
                current.insert(pos, shortcut);
                ui->messageInput->setText(current);
                ui->messageInput->setCursorPosition(pos + shortcut.length());
                dialog.accept();
            });

            grid->addWidget(btn, row, col);
            ++col;
            if (col >= maxCols) {
                col = 0;
                ++row;
            }
        }

        layout->addLayout(grid);
        dialog.exec();
    });

    // Wizz button
    connect(ui->wizzButton, &QPushButton::clicked, this, [this]() {
        if (!m_webSocketClient->isConnected() || !m_identified) {
            ui->statusLabel->setText("Not connected or not identified!");
            return;
        }
        if (m_currentChatTarget.isEmpty()) {
            ui->statusLabel->setText("Select a user first");
            return;
        }
        sendWizz(m_currentChatTarget);
        ui->chatDisplay->append("<p style='color:#666; font-style: italic;'><b>(Wizz)</b> You sent a wizz.</p>");
        triggerWizz();
    });

    // Home button: return to vintage home view
    connect(ui->homeButton, &QPushButton::clicked, this, [this]() {
        showHomeView();
    });

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
            loadAvatar(m_avatarFilename);

            // Refresh profile view to show updated info
            showProfileView();
        }
    });

    // Install event filter for Enter key
    ui->messageInput->installEventFilter(this);

    // Configure chat display to accept HTML and allow emoji images to load
    ui->chatDisplay->setAcceptRichText(true);
    QString emojisDirPath = QDir::cleanPath(
        QCoreApplication::applicationDirPath() + "/../assets/emojis");
    ui->chatDisplay->document()->setBaseUrl(QUrl::fromLocalFile(emojisDirPath + "/"));

    // Initialize font size dropdown
    ui->fontSizeCombo->addItems({"10", "11", "12", "14", "16", "18", "20"});
    ui->fontSizeCombo->setCurrentText(QString::number(m_myFontSizePx));
    connect(ui->fontSizeCombo, &QComboBox::currentTextChanged, this, [this](const QString &t) {
        bool ok = false;
        int px = t.toInt(&ok);
        if (ok && px >= 8 && px <= 48) {
            m_myFontSizePx = px;
        }
    });

    // Text color button
    connect(ui->textColorButton, &QPushButton::clicked, this, [this]() {
        QColor chosen = QColorDialog::getColor(m_myTextColor, this, "Choose text color");
        if (!chosen.isValid()) return;
        m_myTextColor = chosen;
        ui->textColorButton->setStyleSheet(
            ui->textColorButton->styleSheet() +
            QString("QPushButton#textColorButton { color: %1; }").arg(m_myTextColor.name())
        );
    });

    // Style Wizz button like Send
    QString wizzButtonStyle = sendButtonStyle;
    wizzButtonStyle.replace("QPushButton#sendButton", "QPushButton#wizzButton");
    ui->wizzButton->setStyleSheet(wizzButtonStyle);

    // Configure auth client for profile updates
    m_authClient->setServerUrl(m_serverUrl);

    // Show initial home view (no message input)
    showHomeView();

    // Load and display the current user's avatar in the header.
    loadAvatar(m_avatarFilename);

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
    sendPrivateMessage(m_currentChatTarget, message, m_myTextColor, m_myFontSizePx);
    ui->messageInput->clear();
    
    // Add sent message to conversation
    addMessageToConversation(m_currentChatTarget, message, true, m_myTextColor, m_myFontSizePx);
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

void ChatWindow::sendPrivateMessage(const QString &to, const QString &text, const QColor &color, int fontSizePx)
{
    QJsonObject msg;
    msg["type"] = "private_msg";
    msg["to"] = to;
    msg["text"] = text;
    if (color.isValid()) {
        msg["color"] = color.name(QColor::HexRgb);
    }
    if (fontSizePx > 0) {
        msg["size"] = fontSizePx;
    }
    
    QJsonDocument doc(msg);
    QString jsonStr = doc.toJson(QJsonDocument::Compact);
    qDebug() << "[CLIENT] Sending private message:" << jsonStr;
    m_webSocketClient->sendMessage(jsonStr);
}

void ChatWindow::sendWizz(const QString &to)
{
    QJsonObject msg;
    msg["type"] = "wizz";
    msg["to"] = to;

    QJsonDocument doc(msg);
    QString jsonStr = doc.toJson(QJsonDocument::Compact);
    qDebug() << "[CLIENT] Sending wizz:" << jsonStr;
    m_webSocketClient->sendMessage(jsonStr);
}

void ChatWindow::updateUserList(const QJsonArray &users)
{
    ui->userList->clear();

    qDebug() << "Updating user list, current username:" << m_username;

    // Rebuild our local list of online users (excluding self)
    m_onlineUsers.clear();
    m_loginToDisplayName.clear();
    m_loginToAvatar.clear();
    m_loginToStatus.clear();

    for (const QJsonValue &value : users) {
        QJsonObject obj = value.toObject();
        QString login = obj["login"].toString();
        QString name  = obj["name"].toString().trimmed();
        QString picture = obj["picture"].toString().trimmed();
        QString status = obj["status"].toString().trimmed();
        if (name.isEmpty()) {
            name = login;
        }
        if (picture.isEmpty()) {
            picture = "default.png";
        }

        qDebug() << "  - User login:" << login << "name:" << name;

        if (login != m_username) {  // Don't show yourself in the list
            m_onlineUsers.append(name);
            m_loginToDisplayName[login] = name;
            m_loginToAvatar[login] = picture;
            m_loginToStatus[login] = status;

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
    } else {
        // If we're in a conversation, refresh header/avatar in case the user's profile changed
        const QString displayName = m_loginToDisplayName.value(m_currentChatTarget, m_currentChatTarget);
        ui->chatWithLabel->setText("Chat with: " + displayName);
        const QString partnerAvatar = m_loginToAvatar.value(m_currentChatTarget, QStringLiteral("default.png"));
        loadAvatar(partnerAvatar);
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
        QColor color;
        int sizePx = 0;
        if (json.contains("color")) {
            color = QColor(json["color"].toString());
        }
        if (json.contains("size")) {
            sizePx = json["size"].toInt();
        }
        
        // Add message to the conversation with this user
        addMessageToConversation(from, text, false, color, sizePx);
        
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
    else if (type == "wizz") {
        QString from = json["from"].toString();
        QString displayName = m_loginToDisplayName.value(from, from);
        ui->chatDisplay->append(
            "<p style='color:#666; font-style: italic;'><b>(Wizz)</b> "
            + displayName.toHtmlEscaped() + " sent you a wizz.</p>"
        );
        triggerWizz();
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

void ChatWindow::triggerWizz()
{
    // Classic MSN-ish: shake window and beep
    QApplication::beep();

    const QPoint basePos = pos();
    const int amplitude = 10;
    const int shakes = 12;
    const int durationMs = 350;

    QPropertyAnimation *anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(durationMs);
    anim->setLoopCount(1);

    for (int i = 0; i <= shakes; ++i) {
        const qreal t = static_cast<qreal>(i) / shakes;
        int dx = (i % 2 == 0) ? amplitude : -amplitude;
        int dy = ((i / 2) % 2 == 0) ? amplitude / 2 : -amplitude / 2;
        anim->setKeyValueAt(t, basePos + QPoint(dx, dy));
    }
    anim->setKeyValueAt(1.0, basePos);

    connect(anim, &QPropertyAnimation::finished, anim, &QObject::deleteLater);
    anim->start();
}

void ChatWindow::loadAvatar(const QString &filename)
{
    ui->avatarLabel->clear();

    if (filename.isEmpty()) {
        return;
    }

    QPixmap avatarPixmap;
    const QString resourcePath = ":/assets/avatar/" + filename;
    QString filePath = QDir::cleanPath(
        QCoreApplication::applicationDirPath() + "/../assets/avatar/" + filename);

    if (avatarPixmap.load(resourcePath) || avatarPixmap.load(filePath)) {
        ui->avatarLabel->setPixmap(avatarPixmap);
    }
}

QString ChatWindow::emojiImageTag(const QString &filename) const
{
    // Use relative path; chat display document baseUrl is set to emojis folder
    return QString("<img src=\"%1\" width=\"19\" height=\"19\" />")
        .arg(filename.toHtmlEscaped());
}

QString ChatWindow::applyEmojiShortcuts(const QString &text,
                                        const QString &username,
                                        bool isFromMe) const
{
    Q_UNUSED(username);
    Q_UNUSED(isFromMe);

    // Example mapping table. Add more shortcuts/files here.
    static const QList<QPair<QString, QString>> shortcutToFile = {
        {":D",  "teeth_smile.png"},
        {";)",  "03.png"},
        {":)",  "001.png"},
        {":-O",  "omg_smile.png"},
        {":P",  "tongue_smile.png"},
        {"(H)",  "shades_smile.png"},
        {":@",  "angry_smile.png"},
        {":S",  "confused_smile.png"},
        {":$",  "red_smile.png"},
        {":(",  "sad_smile.png"},
        {":'(",  "surprised_smile.png"}, //missing
        {":|",  "what_face.png"},
        {"(A)",  "angel_smile.png"},
        {"8o|",  "48_48.png"},
        {"8-|",  "49_49.png"},
        {"+o(",  "52_52.png"},
        {"|-)",  "77_77.png"},
        {"*-)",  "72_72.png"},
        {":-#",  "048.png"},
        {":-*",  "51_51.png"},
        {"^o)",  "50_50.png"},
        {"8-)",  "71_71.png"},
        {"(L)",  "015.png"},
        {"(U)",  "016.png"},
        {"(M)",  "017.png"},
        {"(@)",  "018.png"},
        {"(&)",  "019.png"},
        {"(sn)",  "054.png"},
        {"(bah)",  "071.png"},
        {"(S)",  "020.png"},
        {"(*)",  "021.png"},
        {"(#)",  "046.png"},
        {"(R)",  "047.png"},
        {"({)",  "035.png"},
        {"(})",  "036.png"},
        {"(K)",  "kiss.png"},
        {"(F)",  "025.png"},


    };

    // Replace shortcuts with placeholders so they survive HTML escaping
    QString result = text;
    for (const auto &pair : shortcutToFile) {
        const QString &shortcut = pair.first;
        const QString &file     = pair.second;
        QString placeholder = QString("{{EMOJI:%1}}").arg(file);
        result.replace(shortcut, placeholder);
    }

    result = result.toHtmlEscaped();

    // Replace placeholders with img tags (relative path; baseUrl set on chat document)
    for (const auto &pair : shortcutToFile) {
        const QString &file = pair.second;
        QString placeholder = QString("{{EMOJI:%1}}").arg(file);
        result.replace(placeholder.toHtmlEscaped(), emojiImageTag(file));
    }

    return result;
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
    ui->emojiButton->setVisible(false);
    ui->fontSizeCombo->setVisible(false);
    ui->textColorButton->setVisible(false);
    ui->wizzButton->setVisible(false);

    // Show current user's avatar in home view
    loadAvatar(m_avatarFilename);

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
    ui->emojiButton->setVisible(false);
    ui->fontSizeCombo->setVisible(false);
    ui->textColorButton->setVisible(false);
    ui->wizzButton->setVisible(false);

    // Show current user's avatar in profile view
    loadAvatar(m_avatarFilename);

    // Initialize content with a more vintage profile layout, showing the picture
    ui->chatDisplay->clear();

    const QString avatarFile = m_avatarFilename.isEmpty()
        ? QStringLiteral("default.png")
        : m_avatarFilename;
    const QString avatarPath = QDir::cleanPath(
        QCoreApplication::applicationDirPath() + "/../assets/avatar/" + avatarFile);
    const QString avatarUrl = QUrl::fromLocalFile(avatarPath).toString();

    QString html =
        "<div style='font-family: Tahoma, Arial, sans-serif; font-size: 11px;'>"
        "<table cellpadding='6' cellspacing='0'>"
        "<tr>"
        "<td valign='top' style='border:1px solid #7F9DB9; background-color:#FFFFFF;'>"
        "<img src=\"" + avatarUrl + "\" width='96' height='96' "
        "style='display:block;' />"
        "</td>"
        "<td valign='top' style='padding-left:8px;'>"
        "<p style='color:#003B75; font-weight:bold; margin:0 0 6px 0;'>Your profile</p>"
        "<p style='margin:0 0 4px 0;'><b>Display name:</b> "
        + m_displayName.toHtmlEscaped() + "</p>"
        "<p style='margin:0 0 4px 0;'><b>Username:</b> "
        + m_username.toHtmlEscaped() + "</p>"
        "<p style='margin:0;'><b>Status:</b> "
        + m_status.toHtmlEscaped() + "</p>"
        "</td>"
        "</tr>"
        "</table>"
        "</div>";

    ui->chatDisplay->insertHtml(html);
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

    // Show avatar of the user we're chatting with (fallback to default)
    const QString partnerAvatar = m_loginToAvatar.value(username, QStringLiteral("default.png"));
    loadAvatar(partnerAvatar);

    // Ensure message input is visible in chat mode
    ui->messageInput->setVisible(true);
    ui->sendButton->setVisible(true);
    ui->emojiButton->setVisible(true);
    ui->fontSizeCombo->setVisible(true);
    ui->textColorButton->setVisible(true);
    ui->wizzButton->setVisible(true);
    
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

void ChatWindow::addMessageToConversation(const QString &username,
                                         const QString &text,
                                         bool isFromMe,
                                         const QColor &color,
                                         int fontSizePx)
{
    QString htmlMessage;
    QString style;
    if (color.isValid()) {
        style += QString("color:%1;").arg(color.name(QColor::HexRgb));
    }
    if (fontSizePx > 0) {
        style += QString("font-size:%1px;").arg(fontSizePx);
    }
    const QString bodyHtmlRaw = applyEmojiShortcuts(text, username, isFromMe);
    const QString bodyHtml = style.isEmpty()
        ? bodyHtmlRaw
        : QString("<span style=\"%1\">%2</span>").arg(style, bodyHtmlRaw);

    if (isFromMe) {
        QString statusText = m_status.trimmed();
        QString nameLabel = m_displayName.toHtmlEscaped();
        if (!statusText.isEmpty()) {
            nameLabel += " (" + statusText.toHtmlEscaped() + ")";
        }
        QString prefix = QString("(%1) dit :").arg(nameLabel);
        htmlMessage = QString("<p><b style='color: #0066CC;'>%1</b> %2</p>")
                      .arg(prefix)
                      .arg(bodyHtml);
    } else {
        QString displayName = m_loginToDisplayName.value(username, username);
        QString statusText = m_loginToStatus.value(username).trimmed();
        QString nameLabel = displayName.toHtmlEscaped();
        if (!statusText.isEmpty()) {
            nameLabel += " (" + statusText.toHtmlEscaped() + ")";
        }
        QString prefix = QString("(%1) dit :").arg(nameLabel);
        htmlMessage = QString("<p><b style='color: #CC0066;'>%1</b> %2</p>")
                      .arg(prefix)
                      .arg(bodyHtml);
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
