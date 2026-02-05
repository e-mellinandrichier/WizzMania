#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include "AuthClient.h"

namespace Ui {
    class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void loginSuccessful(const QString &username,
                         const QString &displayName,
                         const QString &avatarFilename,
                         const QString &status,
                         const QString &serverUrl);

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onRegisterSuccess();
    void onRegisterFailed(const QString &error);
    void onLoginSuccess(const QString &username,
                        const QString &displayName,
                        const QString &avatarFilename,
                        const QString &status);
    void onLoginFailed(const QString &error);

private:
    Ui::LoginWindow *ui;
    AuthClient *m_authClient;
    QString m_serverUrl;
    QPoint m_dragPosition;
    bool m_dragging;
};

#endif // LOGINWINDOW_H
