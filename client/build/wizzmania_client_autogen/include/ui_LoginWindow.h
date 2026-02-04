/********************************************************************************
** Form generated from reading UI file 'LoginWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QVBoxLayout *mainLayout;
    QWidget *windowContainer;
    QVBoxLayout *windowLayout;
    QWidget *titleBar;
    QHBoxLayout *titleBarLayout;
    QLabel *titleLabel;
    QSpacerItem *titleSpacer;
    QPushButton *minimizeButton;
    QPushButton *maximizeButton;
    QPushButton *closeButton;
    QWidget *contentWidget;
    QVBoxLayout *contentLayout;
    QWidget *headerWidget;
    QVBoxLayout *headerLayout;
    QLabel *iconLabel;
    QLabel *titleHeaderLabel;
    QWidget *formWidget;
    QVBoxLayout *formLayout;
    QLabel *emailLabel;
    QLineEdit *usernameInput;
    QLabel *passwordLabel;
    QLineEdit *passwordInput;
    QLabel *displayNameLabel;
    QLineEdit *displayNameInput;
    QLabel *statusLabelField;
    QLineEdit *statusInput;
    QLabel *avatarLabel;
    QLineEdit *avatarInput;
    QWidget *rememberMeWidget;
    QHBoxLayout *rememberMeLayout;
    QCheckBox *rememberMeCheckBox;
    QWidget *buttonsWidget;
    QHBoxLayout *buttonsLayout;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QWidget *linksWidget;
    QVBoxLayout *linksLayout;
    QWidget *footerWidget;
    QVBoxLayout *footerLayout;
    QLabel *statusLabel;

    void setupUi(QWidget *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName("LoginWindow");
        LoginWindow->resize(380, 500);
        mainLayout = new QVBoxLayout(LoginWindow);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        windowContainer = new QWidget(LoginWindow);
        windowContainer->setObjectName("windowContainer");
        windowContainer->setMinimumSize(QSize(380, 0));
        windowContainer->setMaximumSize(QSize(380, 16777215));
        windowLayout = new QVBoxLayout(windowContainer);
        windowLayout->setSpacing(0);
        windowLayout->setObjectName("windowLayout");
        windowLayout->setContentsMargins(0, 0, 0, 0);
        titleBar = new QWidget(windowContainer);
        titleBar->setObjectName("titleBar");
        titleBar->setMinimumSize(QSize(0, 28));
        titleBar->setMaximumSize(QSize(16777215, 28));
        titleBarLayout = new QHBoxLayout(titleBar);
        titleBarLayout->setSpacing(4);
        titleBarLayout->setObjectName("titleBarLayout");
        titleBarLayout->setContentsMargins(8, 0, 2, 0);
        titleLabel = new QLabel(titleBar);
        titleLabel->setObjectName("titleLabel");

        titleBarLayout->addWidget(titleLabel);

        titleSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        titleBarLayout->addItem(titleSpacer);

        minimizeButton = new QPushButton(titleBar);
        minimizeButton->setObjectName("minimizeButton");
        minimizeButton->setMinimumSize(QSize(21, 19));
        minimizeButton->setMaximumSize(QSize(21, 19));

        titleBarLayout->addWidget(minimizeButton);

        maximizeButton = new QPushButton(titleBar);
        maximizeButton->setObjectName("maximizeButton");
        maximizeButton->setMinimumSize(QSize(21, 19));
        maximizeButton->setMaximumSize(QSize(21, 19));

        titleBarLayout->addWidget(maximizeButton);

        closeButton = new QPushButton(titleBar);
        closeButton->setObjectName("closeButton");
        closeButton->setMinimumSize(QSize(21, 19));
        closeButton->setMaximumSize(QSize(21, 19));

        titleBarLayout->addWidget(closeButton);


        windowLayout->addWidget(titleBar);

        contentWidget = new QWidget(windowContainer);
        contentWidget->setObjectName("contentWidget");
        contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->setSpacing(0);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(0, 0, 0, 0);
        headerWidget = new QWidget(contentWidget);
        headerWidget->setObjectName("headerWidget");
        headerWidget->setMinimumSize(QSize(0, 100));
        headerLayout = new QVBoxLayout(headerWidget);
        headerLayout->setSpacing(8);
        headerLayout->setObjectName("headerLayout");
        headerLayout->setContentsMargins(24, 24, 24, 16);
        iconLabel = new QLabel(headerWidget);
        iconLabel->setObjectName("iconLabel");
        iconLabel->setMinimumSize(QSize(48, 48));
        iconLabel->setMaximumSize(QSize(48, 48));
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setScaledContents(true);

        headerLayout->addWidget(iconLabel);

        titleHeaderLabel = new QLabel(headerWidget);
        titleHeaderLabel->setObjectName("titleHeaderLabel");
        titleHeaderLabel->setAlignment(Qt::AlignCenter);

        headerLayout->addWidget(titleHeaderLabel);


        contentLayout->addWidget(headerWidget);

        formWidget = new QWidget(contentWidget);
        formWidget->setObjectName("formWidget");
        formLayout = new QVBoxLayout(formWidget);
        formLayout->setSpacing(12);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(20, 16, 20, 16);
        emailLabel = new QLabel(formWidget);
        emailLabel->setObjectName("emailLabel");

        formLayout->addWidget(emailLabel);

        usernameInput = new QLineEdit(formWidget);
        usernameInput->setObjectName("usernameInput");
        usernameInput->setMinimumSize(QSize(0, 22));
        usernameInput->setMaximumSize(QSize(16777215, 22));

        formLayout->addWidget(usernameInput);

        passwordLabel = new QLabel(formWidget);
        passwordLabel->setObjectName("passwordLabel");

        formLayout->addWidget(passwordLabel);

        passwordInput = new QLineEdit(formWidget);
        passwordInput->setObjectName("passwordInput");
        passwordInput->setEchoMode(QLineEdit::Password);
        passwordInput->setMinimumSize(QSize(0, 22));
        passwordInput->setMaximumSize(QSize(16777215, 22));

        formLayout->addWidget(passwordInput);

        displayNameLabel = new QLabel(formWidget);
        displayNameLabel->setObjectName("displayNameLabel");

        formLayout->addWidget(displayNameLabel);

        displayNameInput = new QLineEdit(formWidget);
        displayNameInput->setObjectName("displayNameInput");
        displayNameInput->setMinimumSize(QSize(0, 22));
        displayNameInput->setMaximumSize(QSize(16777215, 22));

        formLayout->addWidget(displayNameInput);

        statusLabelField = new QLabel(formWidget);
        statusLabelField->setObjectName("statusLabelField");

        formLayout->addWidget(statusLabelField);

        statusInput = new QLineEdit(formWidget);
        statusInput->setObjectName("statusInput");
        statusInput->setMinimumSize(QSize(0, 22));
        statusInput->setMaximumSize(QSize(16777215, 22));

        formLayout->addWidget(statusInput);

        avatarLabel = new QLabel(formWidget);
        avatarLabel->setObjectName("avatarLabel");

        formLayout->addWidget(avatarLabel);

        avatarInput = new QLineEdit(formWidget);
        avatarInput->setObjectName("avatarInput");
        avatarInput->setMinimumSize(QSize(0, 22));
        avatarInput->setMaximumSize(QSize(16777215, 22));

        formLayout->addWidget(avatarInput);

        rememberMeWidget = new QWidget(formWidget);
        rememberMeWidget->setObjectName("rememberMeWidget");
        rememberMeLayout = new QHBoxLayout(rememberMeWidget);
        rememberMeLayout->setSpacing(4);
        rememberMeLayout->setObjectName("rememberMeLayout");
        rememberMeLayout->setContentsMargins(0, 0, 0, 0);
        rememberMeCheckBox = new QCheckBox(rememberMeWidget);
        rememberMeCheckBox->setObjectName("rememberMeCheckBox");

        rememberMeLayout->addWidget(rememberMeCheckBox);


        formLayout->addWidget(rememberMeWidget);

        buttonsWidget = new QWidget(formWidget);
        buttonsWidget->setObjectName("buttonsWidget");
        buttonsLayout = new QHBoxLayout(buttonsWidget);
        buttonsLayout->setSpacing(8);
        buttonsLayout->setObjectName("buttonsLayout");
        buttonsLayout->setContentsMargins(0, 0, 0, 0);
        loginButton = new QPushButton(buttonsWidget);
        loginButton->setObjectName("loginButton");
        loginButton->setMinimumSize(QSize(0, 24));
        loginButton->setMaximumSize(QSize(16777215, 24));

        buttonsLayout->addWidget(loginButton);

        registerButton = new QPushButton(buttonsWidget);
        registerButton->setObjectName("registerButton");
        registerButton->setMinimumSize(QSize(0, 24));
        registerButton->setMaximumSize(QSize(16777215, 24));

        buttonsLayout->addWidget(registerButton);


        formLayout->addWidget(buttonsWidget);

        linksWidget = new QWidget(formWidget);
        linksWidget->setObjectName("linksWidget");
        linksLayout = new QVBoxLayout(linksWidget);
        linksLayout->setSpacing(4);
        linksLayout->setObjectName("linksLayout");
        linksLayout->setContentsMargins(0, 0, 0, 0);

        formLayout->addWidget(linksWidget);


        contentLayout->addWidget(formWidget);

        footerWidget = new QWidget(contentWidget);
        footerWidget->setObjectName("footerWidget");
        footerWidget->setMinimumSize(QSize(0, 50));
        footerLayout = new QVBoxLayout(footerWidget);
        footerLayout->setSpacing(4);
        footerLayout->setObjectName("footerLayout");
        footerLayout->setContentsMargins(16, 8, 16, 8);

        contentLayout->addWidget(footerWidget);


        windowLayout->addWidget(contentWidget);


        mainLayout->addWidget(windowContainer);

        statusLabel = new QLabel(LoginWindow);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setWordWrap(true);

        mainLayout->addWidget(statusLabel);


        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QWidget *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "Wizz Mania Messenger", nullptr));
        titleLabel->setText(QCoreApplication::translate("LoginWindow", "Wizz Mania Messenger", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("LoginWindow", "color: white; font-size: 11px; font-weight: bold; letter-spacing: 0.5px;", nullptr));
        minimizeButton->setText(QCoreApplication::translate("LoginWindow", "_", nullptr));
        maximizeButton->setText(QCoreApplication::translate("LoginWindow", "\342\226\241", nullptr));
        closeButton->setText(QCoreApplication::translate("LoginWindow", "\303\227", nullptr));
        contentWidget->setStyleSheet(QCoreApplication::translate("LoginWindow", "background-color: white;", nullptr));
        iconLabel->setText(QString());
        titleHeaderLabel->setText(QCoreApplication::translate("LoginWindow", "Wizz Mania Messenger", nullptr));
        titleHeaderLabel->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 18px; font-weight: bold; color: #003B75; letter-spacing: 1px;", nullptr));
        emailLabel->setText(QCoreApplication::translate("LoginWindow", "Username:", nullptr));
        emailLabel->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; font-weight: bold; color: #000000;", nullptr));
        usernameInput->setPlaceholderText(QCoreApplication::translate("LoginWindow", "username", nullptr));
        usernameInput->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; font-family: Tahoma, Arial, sans-serif; border: 1px solid #7F9DB9; padding: 2px 3px;", nullptr));
        passwordLabel->setText(QCoreApplication::translate("LoginWindow", "Password:", nullptr));
        passwordLabel->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; font-weight: bold; color: #000000;", nullptr));
        passwordInput->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; font-family: Tahoma, Arial, sans-serif; border: 1px solid #7F9DB9; padding: 2px 3px;", nullptr));
        displayNameLabel->setText(QCoreApplication::translate("LoginWindow", "Display name:", nullptr));
        displayNameLabel->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; font-weight: bold; color: #000000;", nullptr));
        displayNameInput->setPlaceholderText(QCoreApplication::translate("LoginWindow", "How your friends see you (optional)", nullptr));
        displayNameInput->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; font-family: Tahoma, Arial, sans-serif; border: 1px solid #7F9DB9; padding: 2px 3px;", nullptr));
        statusLabelField->setText(QCoreApplication::translate("LoginWindow", "Status message:", nullptr));
        statusLabelField->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; font-weight: bold; color: #000000;", nullptr));
        statusInput->setPlaceholderText(QCoreApplication::translate("LoginWindow", "What's on your mind? (optional)", nullptr));
        statusInput->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; font-family: Tahoma, Arial, sans-serif; border: 1px solid #7F9DB9; padding: 2px 3px;", nullptr));
        avatarLabel->setText(QCoreApplication::translate("LoginWindow", "Avatar filename:", nullptr));
        avatarLabel->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; font-weight: bold; color: #000000;", nullptr));
        avatarInput->setPlaceholderText(QCoreApplication::translate("LoginWindow", "e.g. default.png (optional)", nullptr));
        avatarInput->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; font-family: Tahoma, Arial, sans-serif; border: 1px solid #7F9DB9; padding: 2px 3px;", nullptr));
        rememberMeCheckBox->setText(QCoreApplication::translate("LoginWindow", "Remember my password", nullptr));
        rememberMeCheckBox->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 11px; color: #000000;", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginWindow", "Sign In", nullptr));
        registerButton->setText(QCoreApplication::translate("LoginWindow", "Sign Up", nullptr));
        statusLabel->setText(QString());
        statusLabel->setStyleSheet(QCoreApplication::translate("LoginWindow", "font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7); padding: 8px;", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
