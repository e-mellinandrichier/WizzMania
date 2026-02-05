#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <QIcon>
#include <QPixmap>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
    , m_authClient(new AuthClient(this))
    , m_dragging(false)
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
    
    // Apply QSS stylesheet
    if (!qssStyle.isEmpty()) {
        this->setStyleSheet(qssStyle);
    }
    
    // Ensure vintage gradients are applied (fallback if QSS doesn't load)
    QString gradientStyle = 
        "QWidget#LoginWindow {"
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
        "QWidget#headerWidget {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FFFFFF,"
        "        stop:0.15 #F0F8FF,"
        "        stop:0.30 #E0F0FF,"
        "        stop:0.50 #D0E8FF,"
        "        stop:0.70 #C0E0FF,"
        "        stop:0.85 #B0D8FF,"
        "        stop:1.0 #A0D0FF);"
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
        "}"
        "QLabel#titleLabel {"
        "    color: white;"
        "    font-size: 11px;"
        "    font-weight: bold;"
        "    letter-spacing: 0.5px;"
        "}"
        "QLabel#titleHeaderLabel {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #003B75;"
        "    letter-spacing: 1px;"
        "}";
    
    // Append gradient styles to existing stylesheet
    QString finalStyle = qssStyle + "\n" + gradientStyle;
    this->setStyleSheet(finalStyle);
    
    // Set default server URL (hardcoded for now, can be made configurable)
    m_serverUrl = "http://localhost:18080";
    
    // Connect title bar buttons
    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->minimizeButton, &QPushButton::clicked, this, &QWidget::showMinimized);
    
    // Style title bar buttons (fallback if QSS doesn't load)
    // Red - Close button
    ui->closeButton->setStyleSheet(
        "QPushButton#closeButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FF5F57, stop:1 #E0443E);"
        "    border: 1px solid #CC3A35;"
        "    border-radius: 2px;"
        "    color: white;"
        "    font-size: 16px;"
        "}"
        "QPushButton#closeButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FF6F67, stop:1 #F0544E);"
        "}"
    );
    
    // Yellow - Minimize button
    ui->minimizeButton->setStyleSheet(
        "QPushButton#minimizeButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FFBD2E, stop:1 #E6A826);"
        "    border: 1px solid #CC8F20;"
        "    border-radius: 2px;"
        "    color: white;"
        "    font-size: 11px;"
        "}"
        "QPushButton#minimizeButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #FFCD3E, stop:1 #F0B836);"
        "}"
    );
    
    // Green - Maximize button
    ui->maximizeButton->setStyleSheet(
        "QPushButton#maximizeButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #28CA42, stop:1 #22A836);"
        "    border: 1px solid #1D8A2C;"
        "    border-radius: 2px;"
        "    color: white;"
        "    font-size: 11px;"
        "}"
        "QPushButton#maximizeButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 #38DA52, stop:1 #32B846);"
        "}"
    );
    
    // Ensure Sign In button has Windows XP style gradient (fallback if QSS doesn't load)
    ui->loginButton->setStyleSheet(
        "QPushButton#loginButton {"
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
        "QPushButton#loginButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 rgb(255, 210, 90),"
        "        stop:0.5 rgb(255, 175, 10),"
        "        stop:1 rgb(240, 150, 5));"
        "    border-top: 1px solid #FFE55C;"
        "    border-left: 1px solid #FFB84D;"
        "}"
        "QPushButton#loginButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 rgb(220, 140, 0),"
        "        stop:0.5 rgb(200, 120, 0),"
        "        stop:1 rgb(180, 110, 0));"
        "    border: 1px inset #B87300;"
        "    border-top: 1px solid #996600;"
        "    border-left: 1px solid #996600;"
        "}"
    );
    
    // Style Sign Up button with same orange gradient as Sign In
    ui->registerButton->setStyleSheet(
        "QPushButton#registerButton {"
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
        "QPushButton#registerButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 rgb(255, 210, 90),"
        "        stop:0.5 rgb(255, 175, 10),"
        "        stop:1 rgb(240, 150, 5));"
        "    border-top: 1px solid #FFE55C;"
        "    border-left: 1px solid #FFB84D;"
        "}"
        "QPushButton#registerButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "        stop:0 rgb(220, 140, 0),"
        "        stop:0.5 rgb(200, 120, 0),"
        "        stop:1 rgb(180, 110, 0));"
        "    border: 1px inset #B87300;"
        "    border-top: 1px solid #996600;"
        "    border-left: 1px solid #996600;"
        "}"
    );
    
    // Connect form signals
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(m_authClient, &AuthClient::registerSuccess, this, &LoginWindow::onRegisterSuccess);
    connect(m_authClient, &AuthClient::registerFailed, this, &LoginWindow::onRegisterFailed);
    connect(m_authClient, &AuthClient::loginSuccess, this, &LoginWindow::onLoginSuccess);
    connect(m_authClient, &AuthClient::loginFailed, this, &LoginWindow::onLoginFailed);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::onLoginClicked()
{
    QString username = ui->usernameInput->text().trimmed();
    QString password = ui->passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->statusLabel->setText("Please enter both username and password");
        ui->statusLabel->setStyleSheet("font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7);");
        return;
    }

    m_authClient->setServerUrl(m_serverUrl);
    ui->statusLabel->setText("Logging in...");
    ui->loginButton->setEnabled(false);
    ui->registerButton->setEnabled(false);
    m_authClient->login(username, password);
}

void LoginWindow::onRegisterClicked()
{
    QString username = ui->usernameInput->text().trimmed();
    QString password = ui->passwordInput->text();
    QString displayName = ui->displayNameInput ? ui->displayNameInput->text().trimmed() : QString();
    QString statusText  = ui->statusInput ? ui->statusInput->text().trimmed() : QString();
    QString avatarFile  = ui->avatarInput ? ui->avatarInput->text().trimmed() : QString();

    if (username.isEmpty() || password.isEmpty()) {
        ui->statusLabel->setText("Please enter both username and password");
        ui->statusLabel->setStyleSheet("font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7);");
        return;
    }

    m_authClient->setServerUrl(m_serverUrl);
    ui->statusLabel->setText("Registering...");
    ui->loginButton->setEnabled(false);
    ui->registerButton->setEnabled(false);
    m_authClient->registerUser(username, password, displayName, avatarFile, statusText);
}

void LoginWindow::onRegisterSuccess()
{
    ui->statusLabel->setText("Registration successful! You can now login.");
    ui->statusLabel->setStyleSheet("font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7);");
    ui->loginButton->setEnabled(true);
    ui->registerButton->setEnabled(true);
}

void LoginWindow::onRegisterFailed(const QString &error)
{
    ui->statusLabel->setText("Registration failed: " + error);
    ui->statusLabel->setStyleSheet("font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7);");
    ui->loginButton->setEnabled(true);
    ui->registerButton->setEnabled(true);
}

void LoginWindow::onLoginSuccess(const QString &username,
                                 const QString &displayName,
                                 const QString &avatarFilename,
                                 const QString &status)
{
    emit loginSuccessful(username, displayName, avatarFilename, status, m_serverUrl);
}

void LoginWindow::onLoginFailed(const QString &error)
{
    ui->statusLabel->setText("Login failed: " + error);
    ui->statusLabel->setStyleSheet("font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7);");
    ui->loginButton->setEnabled(true);
    ui->registerButton->setEnabled(true);
}

void LoginWindow::mousePressEvent(QMouseEvent *event)
{
    // Allow dragging from the title bar area
    if (event->button() == Qt::LeftButton) {
        QWidget *titleBar = findChild<QWidget*>("titleBar");
        if (titleBar && titleBar->geometry().contains(event->pos())) {
            m_dragging = true;
            m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            event->accept();
            return;
        }
    }
    QWidget::mousePressEvent(event);
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
        return;
    }
    QWidget::mouseMoveEvent(event);
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}
