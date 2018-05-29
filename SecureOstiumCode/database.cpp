#include "database.h"

Database::Database(QString dbName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
}

bool Database::connect(void)
{
    if (!db.open()) {
        cerr << "Failed to open database" << endl;
        return false;
    }
    else return true;
}

//Function to retrieve the user's name, id and last_entry
//Input QString rfid, QString* last_entry
//Output QString user name
QString Database::requestRFID(QString rfid, QString * last_entry)
{
    QSqlQuery query;
    QString name="";
    query.prepare("SELECT users.name, users.cod_usr FROM users, rfid WHERE rfid.id_rfid = '" + rfid + "' AND users.cod_usr = rfid.cod_usr");
    if (query.exec())
    {
        while (query.next())
        {
            name = query.value(0).toString(); // users.name
            cod_user = query.value(1).toInt(); // users.cod_usr
        }
        if (name != "")
        {
            query.prepare("SELECT MAX(entries.time) FROM users, entries WHERE users.cod_usr = " + QString::number(cod_user) + " AND entries.cod_usr = users.cod_usr");
            if (query.exec())
            {
                while (query.next())
                {
                    *last_entry = query.value(0).toString(); // entries.time
                    if (*last_entry == "")
                        *last_entry = "None";
                }
            }
        }
        return name;
    }
    return "Error in query";
}

//Function to retrieve the voice id
//Input unsigned int user id
//Output QString voice id
QString Database::requestVoice()
{
    QSqlQuery query;
    QString cod_voice="";
    query.prepare("SELECT speaker_rec.cod_voice FROM users, speaker_rec WHERE users.cod_usr = " + QString::number(cod_user) + " AND users.cod_usr = speaker_rec.cod_usr");
    if (query.exec())
    {
        while (query.next())
        {
            cod_voice = query.value(0).toString(); // speaker_rec.cod_voice
        }
        return cod_voice;
    }
    return "Error in query";
}

//Function to retrieve the movement pattern hystereses
//Input unsigned int user id
//Output QString movement pattern
QString Database::requestPattern()
{
    QSqlQuery query;
    QString pattern="";
    query.prepare("SELECT accel_pattern.pattern FROM users, accel_pattern WHERE users.cod_usr = " + QString::number(cod_user) + " AND users.cod_usr = accel_pattern.cod_usr");
    if (query.exec())
    {
        while (query.next())
        {
            pattern = query.value(0).toString(); // accel_pattern.pattern
        }
        return pattern;
    }
    return "Error in query";
}

bool Database::insertEntry(void)
{
    QSqlQuery query;
    query.prepare("INSERT INTO entries VALUES(NULL, " + QString::number(cod_user) + ", CURRENT_TIMESTAMP);");
    return query.exec();
}

void Database::disconnect(void)
{
    db.close();
}
