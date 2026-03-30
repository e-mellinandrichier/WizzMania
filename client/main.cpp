#include <QApplication>
#include <QFont>
#include "src/LoginWindow.h"
#include "src/ChatWindow.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Global retro font: Tahoma everywhere in the client
    QFont appFont("Tahoma");
    appFont.setStyleHint(QFont::SansSerif);
    app.setFont(appFont);

    LoginWindow loginWindow;
    loginWindow.show();

    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful,
                     [&](const QString &username,
                         const QString &displayName,
                         const QString &avatarFilename,
                         const QString &status,
                         const QString &serverUrl) {
        loginWindow.hide();
        
        ChatWindow *chatWindow = new ChatWindow(username, displayName, avatarFilename, status, serverUrl);
        chatWindow->show();
        
        QObject::connect(chatWindow, &ChatWindow::destroyed, &app, &QApplication::quit);
    });

    return app.exec();
}
