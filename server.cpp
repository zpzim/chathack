#include "server.h"
#include <QTcpServer>
#include <QTcpSocket>

Server::Server(QObject* parent) :
    QTcpServer(parent), log("chathack_daemon.log")
{
    server = new QTcpServer(this);
    svrPort = 6499;
    connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnect()));
    connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(onAcceptError(QAbstractSocket::SocketError)));
    listen();
}

Server::~Server()
{
    delete(socket);
    delete(server);
}

void Server::listen()
{
    // QHostAddress::LocalHost, svrPort);
    server->listen(QHostAddress(QString("169.235.31.7")),svrPort);
    if( server->isListening() )
        log.log("is listening on port " + QString(svrPort) + "...\n");
    else
        log.log("not listening!!\n");
}

void Server::incomingConnection(qintptr sfd)
{
    log.log("incoming connection...\n");
}

bool Server::setup() // initial server setup
{
    return false;
}

bool Server::processRequest() // will spawn a thread to handle client request
{
    return false;
}

void Server::onNewConnect() // handler for new connection from client
{
    log.log("New connection request...\n");
    socket = server->nextPendingConnection();

    if(socket->state() == QTcpSocket::ConnectedState)
    {
        printf("New client connection ocurred.\n");
    }

    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Server::onDisconnect()
{
    log.log("Client disconnection occurred!!\n");
    disconnect(socket, SIGNAL(disconnected()));
    disconnect(socket, SIGNAL(readyRead()));
    socket->deleteLater();
}

void Server::onReadyRead()
{
    read();
}

void Server::onAcceptError(QAbstractSocket::SocketError socketError)
{
    log.log("Accept error occurred...\n");
}

void Server::read()
{
    while(socket->canReadLine())
    {
        QByteArray clientByteArray = socket->readLine();
        if( !clientByteArray.contains("\0") )
        {
            log.log("Read null character from client... Ending read...\n");
            break;
        }
        log.log("server read the following from client: " + QString(clientByteArray.constData()));
    }
    log.log("Server finished reading from client...\n");

    write_c(QString("Hello, client!!"));
}


bool Server::write_c(QString msg)
{
    log.log("Writing to client...\n");
    QString tmp(msg);
    int msgLen = tmp.length();
    tmp[msgLen] = '\n';
    //socket->write(tmp.toStdString().c_str());
    return socket->write(tmp.toStdString().c_str()) != -1;
}
