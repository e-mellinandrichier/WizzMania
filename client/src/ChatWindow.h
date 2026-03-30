#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QMap>
#include <QStringList>
#include "WebSocketClient.h"
#include "AuthClient.h"

namespace Ui {
    class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(const QString &username,
                        const QString &displayName,
                        const QString &avatarFilename,
                        const QString &status,
                        const QString &serverUrl,
                        QWidget *parent = nullptr);
    ~ChatWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onSendClicked();
    void onMessageReceived(const QString &message);
    void onConnected();
    void onDisconnected();
    void onError(const QString &error);
    void onUserSelected();

private:
    void sendIdentify();
    void sendPrivateMessage(const QString &to, const QString &text);
    void updateUserList(const QJsonArray &users);
    void handleServerMessage(const QJsonObject &json);
    void showHomeView();
    void showProfileView();
    void switchToConversation(const QString &username);
    void addMessageToConversation(const QString &from, const QString &message, bool isFromMe = false);
    void loadAvatar(const QString &filename);
    QString emojiImageTag(const QString &filename) const;
    QString applyEmojiShortcuts(const QString &text, const QString &username, bool isFromMe) const;

    Ui::ChatWindow *ui;
    QString m_username;
    QString m_displayName;
    QString m_avatarFilename;
    QString m_status;
    QString m_serverUrl;
    WebSocketClient *m_webSocketClient;
    AuthClient *m_authClient;
    QPoint m_dragPosition;
    bool m_dragging;
    QString m_currentChatTarget;
    bool m_identified;
    // Store messages per conversation: username (login) -> list of HTML messages
    QMap<QString, QStringList> m_conversations;
    // Map login -> display name, avatar filename, and status
    QMap<QString, QString> m_loginToDisplayName;
    QMap<QString, QString> m_loginToAvatar;
    QMap<QString, QString> m_loginToStatus;
    // Track current list of other connected users (display names), for profile view
    QStringList m_onlineUsers;
};

#endif // CHATWINDOW_H
