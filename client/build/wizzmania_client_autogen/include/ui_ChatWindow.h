/********************************************************************************
** Form generated from reading UI file 'ChatWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWINDOW_H
#define UI_CHATWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWindow
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
    QHBoxLayout *contentLayout;
    QWidget *leftSideRail;
    QWidget *userListWidget;
    QVBoxLayout *userListLayout;
    QPushButton *homeButton;
    QLabel *userListTitleLabel;
    QListWidget *userList;
    QPushButton *profileButton;
    QWidget *chatAreaWidget;
    QVBoxLayout *chatAreaLayout;
    QWidget *chatHeaderWidget;
    QHBoxLayout *chatHeaderLayout;
    QLabel *avatarLabel;
    QLabel *chatWithLabel;
    QTextEdit *chatDisplay;
    QHBoxLayout *inputLayout;
    QLineEdit *messageInput;
    QPushButton *emojiButton;
    QPushButton *sendButton;
    QWidget *rightSideRail;
    QLabel *statusLabel;

    void setupUi(QWidget *ChatWindow)
    {
        if (ChatWindow->objectName().isEmpty())
            ChatWindow->setObjectName("ChatWindow");
        ChatWindow->resize(600, 500);
        mainLayout = new QVBoxLayout(ChatWindow);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        windowContainer = new QWidget(ChatWindow);
        windowContainer->setObjectName("windowContainer");
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
        contentLayout = new QHBoxLayout(contentWidget);
        contentLayout->setSpacing(0);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(0, 0, 0, 0);
        leftSideRail = new QWidget(contentWidget);
        leftSideRail->setObjectName("leftSideRail");
        leftSideRail->setMinimumSize(QSize(10, 0));
        leftSideRail->setMaximumSize(QSize(10, 16777215));

        contentLayout->addWidget(leftSideRail);

        userListWidget = new QWidget(contentWidget);
        userListWidget->setObjectName("userListWidget");
        userListWidget->setMinimumSize(QSize(180, 0));
        userListWidget->setMaximumSize(QSize(180, 16777215));
        userListLayout = new QVBoxLayout(userListWidget);
        userListLayout->setSpacing(5);
        userListLayout->setObjectName("userListLayout");
        userListLayout->setContentsMargins(5, 5, 5, 5);
        homeButton = new QPushButton(userListWidget);
        homeButton->setObjectName("homeButton");
        homeButton->setMinimumSize(QSize(0, 18));
        homeButton->setMaximumSize(QSize(16777215, 18));

        userListLayout->addWidget(homeButton);

        userListTitleLabel = new QLabel(userListWidget);
        userListTitleLabel->setObjectName("userListTitleLabel");

        userListLayout->addWidget(userListTitleLabel);

        userList = new QListWidget(userListWidget);
        userList->setObjectName("userList");

        userListLayout->addWidget(userList);

        profileButton = new QPushButton(userListWidget);
        profileButton->setObjectName("profileButton");
        profileButton->setMinimumSize(QSize(0, 26));

        userListLayout->addWidget(profileButton);


        contentLayout->addWidget(userListWidget);

        chatAreaWidget = new QWidget(contentWidget);
        chatAreaWidget->setObjectName("chatAreaWidget");
        chatAreaLayout = new QVBoxLayout(chatAreaWidget);
        chatAreaLayout->setSpacing(10);
        chatAreaLayout->setObjectName("chatAreaLayout");
        chatAreaLayout->setContentsMargins(5, 5, 5, 5);
        chatHeaderWidget = new QWidget(chatAreaWidget);
        chatHeaderWidget->setObjectName("chatHeaderWidget");
        chatHeaderLayout = new QHBoxLayout(chatHeaderWidget);
        chatHeaderLayout->setSpacing(6);
        chatHeaderLayout->setObjectName("chatHeaderLayout");
        chatHeaderLayout->setContentsMargins(5, 3, 5, 3);
        avatarLabel = new QLabel(chatHeaderWidget);
        avatarLabel->setObjectName("avatarLabel");
        avatarLabel->setMinimumSize(QSize(32, 32));
        avatarLabel->setMaximumSize(QSize(32, 32));
        avatarLabel->setAlignment(Qt::AlignCenter);
        avatarLabel->setScaledContents(true);

        chatHeaderLayout->addWidget(avatarLabel);

        chatWithLabel = new QLabel(chatHeaderWidget);
        chatWithLabel->setObjectName("chatWithLabel");

        chatHeaderLayout->addWidget(chatWithLabel);


        chatAreaLayout->addWidget(chatHeaderWidget);

        chatDisplay = new QTextEdit(chatAreaWidget);
        chatDisplay->setObjectName("chatDisplay");
        chatDisplay->setReadOnly(true);

        chatAreaLayout->addWidget(chatDisplay);

        inputLayout = new QHBoxLayout();
        inputLayout->setSpacing(6);
        inputLayout->setObjectName("inputLayout");
        messageInput = new QLineEdit(chatAreaWidget);
        messageInput->setObjectName("messageInput");
        messageInput->setMinimumSize(QSize(0, 30));

        inputLayout->addWidget(messageInput);

        emojiButton = new QPushButton(chatAreaWidget);
        emojiButton->setObjectName("emojiButton");
        emojiButton->setMinimumSize(QSize(24, 24));
        emojiButton->setMaximumSize(QSize(24, 24));

        inputLayout->addWidget(emojiButton);

        sendButton = new QPushButton(chatAreaWidget);
        sendButton->setObjectName("sendButton");
        sendButton->setMinimumSize(QSize(80, 30));

        inputLayout->addWidget(sendButton);


        chatAreaLayout->addLayout(inputLayout);


        contentLayout->addWidget(chatAreaWidget);

        rightSideRail = new QWidget(contentWidget);
        rightSideRail->setObjectName("rightSideRail");
        rightSideRail->setMinimumSize(QSize(10, 0));
        rightSideRail->setMaximumSize(QSize(10, 16777215));

        contentLayout->addWidget(rightSideRail);


        windowLayout->addWidget(contentWidget);


        mainLayout->addWidget(windowContainer);

        statusLabel = new QLabel(ChatWindow);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(statusLabel);


        retranslateUi(ChatWindow);

        QMetaObject::connectSlotsByName(ChatWindow);
    } // setupUi

    void retranslateUi(QWidget *ChatWindow)
    {
        ChatWindow->setWindowTitle(QCoreApplication::translate("ChatWindow", "Wizz Mania Messenger - Chat", nullptr));
        titleLabel->setText(QCoreApplication::translate("ChatWindow", "Wizz Mania Messenger - Chat", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("ChatWindow", "color: white; font-size: 11px; font-weight: bold; letter-spacing: 0.5px;", nullptr));
        minimizeButton->setText(QCoreApplication::translate("ChatWindow", "_", nullptr));
        maximizeButton->setText(QCoreApplication::translate("ChatWindow", "\342\226\241", nullptr));
        closeButton->setText(QCoreApplication::translate("ChatWindow", "\303\227", nullptr));
        contentWidget->setStyleSheet(QCoreApplication::translate("ChatWindow", "background-color: white;", nullptr));
        leftSideRail->setStyleSheet(QCoreApplication::translate("ChatWindow", "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"    stop:0 #001899,\n"
"    stop:0.05 #001FAA,\n"
"    stop:0.20 #0028BB,\n"
"    stop:0.50 #0036D3,\n"
"    stop:0.75 #0044DD,\n"
"    stop:0.88 #0055EE,\n"
"    stop:0.94 #0066FF,\n"
"    stop:0.97 #0D8FFF,\n"
"    stop:1.0 #1AA7FF);", nullptr));
        userListWidget->setStyleSheet(QCoreApplication::translate("ChatWindow", "background-color: #E3EBF6;\n"
"border-right: 1px solid #7F9DB9;", nullptr));
        homeButton->setText(QCoreApplication::translate("ChatWindow", "< Home", nullptr));
        homeButton->setStyleSheet(QCoreApplication::translate("ChatWindow", "QPushButton#homeButton {\n"
"    border: none;\n"
"    background: transparent;\n"
"    color: #003B75;\n"
"    font-size: 10px;\n"
"    font-weight: bold;\n"
"    text-align: left;\n"
"    padding: 0 2px;\n"
"}\n"
"QPushButton#homeButton:hover {\n"
"    text-decoration: underline;\n"
"}", nullptr));
        userListTitleLabel->setText(QCoreApplication::translate("ChatWindow", "Online", nullptr));
        userListTitleLabel->setStyleSheet(QCoreApplication::translate("ChatWindow", "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #FFFFFF,\n"
"        stop:0.4 #DCE7F7,\n"
"        stop:1 #C0D4F2);\n"
"font-size: 11px;\n"
"font-weight: bold;\n"
"color: #003B75;\n"
"padding: 4px 6px;\n"
"border: 1px solid #7F9DB9;", nullptr));
        userList->setStyleSheet(QCoreApplication::translate("ChatWindow", "QListWidget {\n"
"    border: 1px solid #7F9DB9;\n"
"    border-top: none;\n"
"    font-family: Tahoma, Arial, sans-serif;\n"
"    font-size: 11px;\n"
"    background-color: #FFFFFF;\n"
"}\n"
"QListWidget::item {\n"
"    padding: 2px 4px;\n"
"}\n"
"QListWidget::item:selected {\n"
"    background: #0A64D0;\n"
"    color: white;\n"
"}\n"
"QListWidget::item:hover {\n"
"    background: #CDE4FF;\n"
"}", nullptr));
        profileButton->setText(QCoreApplication::translate("ChatWindow", "Profile", nullptr));
        profileButton->setStyleSheet(QCoreApplication::translate("ChatWindow", "font-weight: bold;", nullptr));
        chatAreaWidget->setStyleSheet(QCoreApplication::translate("ChatWindow", "QWidget#chatAreaWidget {\n"
"    background-color: #FFFFFF;\n"
"    border: 1px solid #7F9DB9;\n"
"    border-left: none;\n"
"}", nullptr));
        chatWithLabel->setText(QCoreApplication::translate("ChatWindow", "Chat with: None", nullptr));
        chatWithLabel->setStyleSheet(QCoreApplication::translate("ChatWindow", "font-size: 11px; font-weight: bold; color: #003B75; padding: 5px;", nullptr));
        chatDisplay->setPlaceholderText(QCoreApplication::translate("ChatWindow", "Select a user to start chatting...", nullptr));
        chatDisplay->setStyleSheet(QCoreApplication::translate("ChatWindow", "border: 1px solid #7F9DB9; padding: 5px; font-family: Tahoma, Arial, sans-serif; font-size: 12px;", nullptr));
        messageInput->setPlaceholderText(QCoreApplication::translate("ChatWindow", "Type your message...", nullptr));
        messageInput->setStyleSheet(QCoreApplication::translate("ChatWindow", "border: 1px solid #7F9DB9; padding: 2px 5px; font-family: Tahoma, Arial, sans-serif; font-size: 12px;", nullptr));
        emojiButton->setText(QCoreApplication::translate("ChatWindow", "\342\230\272", nullptr));
#if QT_CONFIG(tooltip)
        emojiButton->setToolTip(QCoreApplication::translate("ChatWindow", "Insert emoticon", nullptr));
#endif // QT_CONFIG(tooltip)
        emojiButton->setStyleSheet(QCoreApplication::translate("ChatWindow", "QPushButton#emojiButton {\n"
"    border: 1px solid #7F9DB9;\n"
"    background-color: #E3EBF6;\n"
"    font-size: 12px;\n"
"    padding: 0;\n"
"}\n"
"QPushButton#emojiButton:hover {\n"
"    background-color: #F5FAFF;\n"
"}", nullptr));
        sendButton->setText(QCoreApplication::translate("ChatWindow", "Send", nullptr));
        sendButton->setStyleSheet(QCoreApplication::translate("ChatWindow", "font-weight: bold;", nullptr));
        rightSideRail->setStyleSheet(QCoreApplication::translate("ChatWindow", "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"    stop:0 #001899,\n"
"    stop:0.05 #001FAA,\n"
"    stop:0.20 #0028BB,\n"
"    stop:0.50 #0036D3,\n"
"    stop:0.75 #0044DD,\n"
"    stop:0.88 #0055EE,\n"
"    stop:0.94 #0066FF,\n"
"    stop:0.97 #0D8FFF,\n"
"    stop:1.0 #1AA7FF);", nullptr));
        statusLabel->setText(QCoreApplication::translate("ChatWindow", "Connected", nullptr));
        statusLabel->setStyleSheet(QCoreApplication::translate("ChatWindow", "font-size: 9px; color: white; text-shadow: 1px 1px 2px rgba(0,0,0,0.7); padding: 8px;", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatWindow: public Ui_ChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWINDOW_H
