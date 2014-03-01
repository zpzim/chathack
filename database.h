#ifndef DATABASE_H
#define DATABASE_H

#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>

class Database
{
public:
    Database(QString host, int port, QString dbname, QString user, QString pass);
    ~Database();
    QList<QString> getChannelMembers(QString name);
private:
    QSqlDatabase connection;
};

#endif // DATABASE_H
