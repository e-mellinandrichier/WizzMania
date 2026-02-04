#include "WebSocketClient.h"
#include <QDebug>

WebSocketClient::WebSocketClient(QObject *parent)
    : QObject(parent)
    , m_webSocket(new QWebSocket("", QWebSocketProtocol::VersionLatest, this))
{
    connect(m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &WebSocketClient::onError);
}

WebSocketClient::~WebSocketClient()
{
    disconnect();
}

void WebSocketClient::connectToServer(const QUrl &url)
{
    m_webSocket->open(url);
}

void WebSocketClient::disconnect()
{
    if (m_webSocket->state() == QAbstractSocket::ConnectedState) {
        m_webSocket->close();
    }
}

void WebSocketClient::sendMessage(const QString &message)
{
    if (m_webSocket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "[WEBSOCKET] Sending message:" << message;
        m_webSocket->sendTextMessage(message);
    } else {
        qDebug() << "[WEBSOCKET] Cannot send message - not connected. State:" << m_webSocket->state();
    }
}

bool WebSocketClient::isConnected() const
{
    return m_webSocket->state() == QAbstractSocket::ConnectedState;
}

void WebSocketClient::onConnected()
{
    qDebug() << "[WEBSOCKET] Connected to server";
    emit connected();
}

void WebSocketClient::onDisconnected()
{
    emit disconnected();
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    qDebug() << "[WEBSOCKET] Received message:" << message;
    emit messageReceived(message);
}

void WebSocketClient::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    emit this->error(m_webSocket->errorString());
}
