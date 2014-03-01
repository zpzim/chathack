#include "database.h"

Database::Database(QString host, int port, QString dbname, QString user, QString pass)
{
    connection.setHostName(host);
    connection.setPort(port);
    connection.setDatabaseName(dbname);
    connection.setUserName(user);
    connection.setPassword(pass);
}

Database::~Database()
{
    //
}

QList<QString> Database::getChannelMembers(QString name)
{
    QList<QString> members;
    QString qry1(
                "select users.name"
                 "from channel"
                 "left join users on channel.uid = users.uid"
                 "where channel.name = '" + name + "';"
                );
    QSqlQuery qry( qry1, connection );
    while ( qry.next() )
    {
         QString user = qry.value(0).toString();
         members.push_back(user);
     }
    return members;
}
