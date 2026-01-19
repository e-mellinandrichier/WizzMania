<a id="en"></a>

<div align="center">
  <a href="#en">EN</a> · 
  <a href="#fr">FR</a>

# WIZZ Mania

***MSN is back!***
</div>

This project aims to create a real-time chat application using **C++** and the **TCP/IP** protocol. 

It will have both a server and a client application, as well as a user-friendly interface. 

## overview of project components 

1. **server application**
    - **role**: manage multiple client connections & handle message routing
    - **responsibilities**: 
        - accept incoming connections from multiple clients 
        - keep track of connected clients
        - route messages from one client to the intended recipient(s)

2. **client application**
    - **role**: connect to the server to send & receive messages 
    - **responsibilities**: 
        - establish a connection to the server
        - send messages to other clients through the server
        - receive messages & display them to the user

3. **UI (user interface)**
    - **role**: improve the user experience!
    - **tools**: we could use **Qt**?
    - **features**:
        - display a chat window where messages can be read & sent 
        - include buttons for sending messages, possible other interactive elements for added fuctionality?

## project plan

1. set up development environment (& learn how to use Qt framework!)
    - C++ compiler GCC, Clang 
    - IDE (Qt Creator, VS Code, Cursor)
    - Qt framework for creating user interface
2. design architecture
    - decide model 
    - create flow diagram or overview of how messages are sent from/to client/server
3. develop the server
    - implement TCP connection
    - use `socket()`, `bind()`, `listen` & `accept()` to manage connections
    - handle and route incoming messages 
4. develop the client 
    - implement client-side TCP connection
    - implement to send and receive messages through the server
5. build the user interface
    - use Qt to create a simple chat window:
        - include text input for entering messages
        - a display area for showing messages
        - buttons for sending messages
6. extra features and tests
    - implement extra features, integrated apps (like games) if time
    - write tests:
        - test connection
        - test sending and receiving messages 
7. documentation
    - how to build & run the server and client applications
    - how to run tests

## technologies & libraries

- c++ standard library for core functionalities
- Qt for creating the graphical user interface
- testing frameworks (Google Test, Catch2, DocTest, Qt Test?)

## things to consider 

- concurrency in the server - ensuring it can handle mulitiple clients at the same time
- error handling for network issues and user input
- measures to secure data transmission?

<a id="fr"></a>

<div align="center">
  <a href="#en">EN</a> · 
  <a href="#fr">FR</a>

# WIZZ Mania

***MSN est de retour !***
</div>

Ce projet vise à créer une application de chat en temps réel en utilisant **C++** et le protocole **TCP/IP**.

Elle comportera à la fois une application serveur et une application client, ainsi qu'une interface conviviale.

## aperçu des composants du projet

1. **application serveur**
    - **rôle**: gérer plusieurs connexions clients et gérer l'acheminement des messages
    - **responsabilités**: 
        - accepter les connexions entrantes de plusieurs clients
        - garder une trace des clients connectés
        - acheminer les messages d'un client vers le(s) destinataire(s) prévu(s)

2. **application client**
    - **rôle**: se connecter au serveur pour envoyer et recevoir des messages
    - **responsabilités**: 
        - établir une connexion au serveur
        - envoyer des messages à d'autres clients via le serveur
        - recevoir des messages et les afficher à l'utilisateur

3. **interface utilisateur (UI)**
    - **rôle**: améliorer l'expérience utilisateur !
    - **outils**: nous pourrions utiliser **Qt** ?
    - **fonctionnalités**:
        - afficher une fenêtre de chat où les messages peuvent être lus et envoyés
        - inclure des boutons pour envoyer des messages, d'autres éléments interactifs possibles pour plus de fonctionnalités ?

## plan du projet

1. mettre en place l'environnement de développement (& apprendre à utiliser le framework Qt !)
    - compilateur C++ GCC, Clang
    - IDE (Qt Creator, VS Code, Cursor)
    - framework Qt pour créer l'interface utilisateur
2. concevoir l'architecture
    - décider du modèle
    - créer un diagramme de flux ou un aperçu de la façon dont les messages sont envoyés depuis/vers le client/serveur
3. développer le serveur
    - implémenter la connexion TCP
    - utiliser `socket()`, `bind()`, `listen` et `accept()` pour gérer les connexions
    - gérer et acheminer les messages entrants
4. développer le client
    - implémenter la connexion TCP côté client
    - implémenter l'envoi et la réception de messages via le serveur
5. construire l'interface utilisateur
    - utiliser Qt pour créer une fenêtre de chat simple :
        - inclure une entrée de texte pour saisir les messages
        - une zone d'affichage pour montrer les messages
        - des boutons pour envoyer les messages
6. fonctionnalités supplémentaires et tests
    - implémenter des fonctionnalités supplémentaires, des applications intégrées (comme des jeux) si le temps le permet
    - écrire des tests :
        - tester la connexion
        - tester l'envoi et la réception de messages
7. documentation
    - comment construire et exécuter les applications serveur et client
    - comment exécuter les tests

## technologies et bibliothèques

- bibliothèque standard C++ pour les fonctionnalités principales
- Qt pour créer l'interface utilisateur graphique
- frameworks de test (Google Test, Catch2, DocTest, Qt Test ?)

## points à considérer

- la concurrence dans le serveur - s'assurer qu'il peut gérer plusieurs clients en même temps
- gestion des erreurs pour les problèmes réseau et l'entrée utilisateur
- mesures pour sécuriser la transmission des données ?

