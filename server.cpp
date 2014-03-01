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
    server->listen(QHostAddress(QString("169.235.31.7")),svrPort);//QHostAddress::LocalHost, svrPort);
    if( server->isListening() )
        //cout << "is listening on port " << svrPort << "...\n";
        log.log("is listening on port " + QString(svrPort) + "...\n");
    else
        //cout << "not listening!!\n";
        log.log("not listening!!\n");
}

void Server::incomingConnection(qintptr sfd)
{
    //
    //cout << "incoming connection...\n";
    log.log("incoming connection...\n");
}

bool Server::setup() // initial server setup
{
    //
    return false;
}

bool Server::processRequest() // will spawn a thread to handle client request
{
    //
    return false;
}

void Server::onNewConnect() // handler for new connection from client
{
    //cout << "New connection request...\n";
    log.log("New connection request...\n");
    //while(server->hasPendingConnections())
        //(void) new DataReader(server->nextPendingConnection(), this);

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
    //cout << "Client disconnection occurred!!\n";
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
    //cout << "Accept error occurred...\n";
    log.log("Accept error occurred...\n");
}

void Server::read()
{
    /*
    * // Write block to the socket.
    BlockWriter(socket).stream() << QDir("C:/Windows").entryList() << QString("Hello World!");

    ....

    // Now read the block from the socket.
    QStringList infoList;
    QString s;
    BlockReader(socket).stream() >> infoList >> s;
    qDebug() << infoList << s;
    */
    //QStringList infoList;
    //QString s;
    //cout << "Waiting for msg from client...\n";
    //SocketReader(socket).stream() >> infoList >> s;
    //cout << "# " << s.toStdString();

    while(socket->canReadLine())
    {
        QByteArray clientByteArray = socket->readLine();
        if( !clientByteArray.contains("\0") )
        //if( strcmp(c,"\0") == 0 )
        {
            //socket->disconnectFromHost();
            //cout << "Read null character from client... Ending read...\n";
            log.log("Read null character from client... Ending read...\n");
            break;
        }
        //else
        //{
        //    tmp += c;
        //}
        //cout << "# " << clientByteArray.constData();
        //cout << "# " << tmp.toStdString();
        log.log("server read the following from client: " + QString(clientByteArray.constData()));
    }
    //cout << "Server finished reading from client...\n";
    log.log("Server finished reading from client...\n");

    //if( write_c(QString("Wazzup!")) )
        //cout << "Write successful!\n";
    //else
        //cout << "Write unsuccessful!\n";
}


/*bool Server::write_c(QString msg)
{
    cout << "Writing to client...\n";
    int msgLen = msg.length();
    msg[msgLen] = '\n';
    QByteArray qmsg = msg.toUtf8();
    QByteArray data = "MESSAGE " + QByteArray::number(qmsg.size()) + ' ' + qmsg;
    return write(data) == data.size();
}*/
