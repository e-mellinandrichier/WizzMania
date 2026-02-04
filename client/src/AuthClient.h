#ifndef AUTHCLIENT_H
#define AUTHCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class AuthClient : public QObject
{
    Q_OBJECT

public:
    explicit AuthClient(QObject *parent = nullptr);
    void setServerUrl(const QString &url);

public slots:
    void registerUser(const QString &username,
                      const QString &password,
                      const QString &name = QString(),
                      const QString &picture = QString(),
                      const QString &status = QString());
    void login(const QString &username, const QString &password);

signals:
    void registerSuccess();
    void registerFailed(const QString &error);
    void loginSuccess(const QString &username,
                      const QString &displayName,
                      const QString &avatarFilename);
    void loginFailed(const QString &error);

private slots:
    void onRegisterFinished();
    void onLoginFinished();

private:
    QNetworkAccessManager *m_networkManager;
    QString m_serverUrl;
    QNetworkReply *m_registerReply;
    QNetworkReply *m_loginReply;
};

#endif // AUTHCLIENT_H
