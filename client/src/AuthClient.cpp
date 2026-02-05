#include "AuthClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QDebug>

AuthClient::AuthClient(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_serverUrl("http://localhost:18080")
    , m_registerReply(nullptr)
    , m_loginReply(nullptr)
    , m_updateReply(nullptr)
{
}

void AuthClient::setServerUrl(const QString &url)
{
    m_serverUrl = url;
}

void AuthClient::registerUser(const QString &username,
                              const QString &password,
                              const QString &name,
                              const QString &picture,
                              const QString &status)
{
    QUrl url(m_serverUrl + "/auth/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    if (!name.isEmpty()) {
        json["name"] = name;
    }
    if (!picture.isEmpty()) {
        json["picture"] = picture;
    }
    if (!status.isEmpty()) {
        json["status"] = status;
    }

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    m_registerReply = m_networkManager->post(request, data);
    connect(m_registerReply, &QNetworkReply::finished, this, &AuthClient::onRegisterFinished);
}

void AuthClient::login(const QString &username, const QString &password)
{
    QUrl url(m_serverUrl + "/auth/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    m_loginReply = m_networkManager->post(request, data);
    connect(m_loginReply, &QNetworkReply::finished, this, &AuthClient::onLoginFinished);
}

void AuthClient::updateProfile(const QString &username,
                               const QString &name,
                               const QString &picture,
                               const QString &status)
{
    QUrl url(m_serverUrl + "/auth/update");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["name"] = name;
    json["picture"] = picture;
    json["status"] = status;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    if (m_updateReply) {
        m_updateReply->deleteLater();
        m_updateReply = nullptr;
    }
    m_updateReply = m_networkManager->post(request, data);
    connect(m_updateReply, &QNetworkReply::finished, this, [this]() {
        if (!m_updateReply) return;
        QByteArray response = m_updateReply->readAll();
        int statusCode = m_updateReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        m_updateReply->deleteLater();
        m_updateReply = nullptr;

        if (statusCode == 200) {
            emit profileUpdated();
        } else {
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject obj = doc.object();
            QString error = obj["error"].toString();
            if (error.isEmpty()) {
                error = "Profile update failed";
            }
            emit profileUpdateFailed(error);
        }
    });
}

void AuthClient::onRegisterFinished()
{
    if (!m_registerReply) return;

    QByteArray response = m_registerReply->readAll();
    int statusCode = m_registerReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    m_registerReply->deleteLater();
    m_registerReply = nullptr;

    if (statusCode == 201) {
        emit registerSuccess();
    } else {
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();
        QString error = obj["error"].toString();
        if (error.isEmpty()) {
            error = "Registration failed";
        }
        emit registerFailed(error);
    }
}

void AuthClient::onLoginFinished()
{
    if (!m_loginReply) return;

    QByteArray response = m_loginReply->readAll();
    int statusCode = m_loginReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    m_loginReply->deleteLater();
    m_loginReply = nullptr;

    if (statusCode == 200) {
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();
        QString username = obj["username"].toString();
        QString displayName = obj["name"].toString().trimmed();
        if (displayName.isEmpty()) {
            displayName = username;
        }
        QString avatarFilename = obj["picture"].toString().trimmed();
        if (avatarFilename.isEmpty()) {
            avatarFilename = "default.png";
        }
        QString status = obj["status"].toString().trimmed();
        emit loginSuccess(username, displayName, avatarFilename, status);
    } else {
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();
        QString error = obj["error"].toString();
        if (error.isEmpty()) {
            error = "Login failed";
        }
        emit loginFailed(error);
    }
}
