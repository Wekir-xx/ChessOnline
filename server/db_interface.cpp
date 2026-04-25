#include "db_interface.h"

#include <QFile>

DBInterface::DBInterface(QObject *parent)
    : QObject{parent}
{
    m_userInfoDB = QSqlDatabase::addDatabase("QSQLITE", "userInfo");
    m_userGameInfoDB = QSqlDatabase::addDatabase("QSQLITE", "userGameInfo");

    m_userInfoDB.setDatabaseName(USER_INFO_DB);
    m_userGameInfoDB.setDatabaseName(USER_GAME_INFO_DB);
}
