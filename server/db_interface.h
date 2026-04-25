#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include "defines.h"

#include <QSqlDatabase>
#include <QObject>

class DBInterface : public QObject
{
    Q_OBJECT

public:
    explicit DBInterface(QObject *parent = nullptr);

signals:
    void addLog(QString log, MESSAGE_TYPE type);

private:
    QSqlDatabase m_userInfoDB;
    QSqlDatabase m_userGameInfoDB;
};

#endif // DBINTERFACE_H
