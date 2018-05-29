#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QString>
#include <iostream>

using namespace std;

class Database
{
public:
    Database(QString);
    bool connect(void); // Connect to database
    QString requestRFID(QString, QString *); // Request RFID from database
    QString requestVoice(void); // Request cod_voice from database
    QString requestPattern(void); // Request pattern from database
    bool insertEntry(void); // Insert entry in database
    void disconnect(void); // Disconnect from database

private:
    unsigned int cod_user;
    QSqlDatabase db; // Database handler
};

#endif // DATABASE_H
