#ifndef GUIUPDATER_H
#define GUIUPDATER_H
#include <QtCore>
#include <mqueue.h>

#include <iostream>
#include <QSound>
#include <semaphore.h>
#include "wiringPi.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef    SCM_NAME
#define    SCM_NAME    "sem_comp"

#endif
#ifndef    QUI_NAME
#define    QUI_NAME    "/queue_ui"

#endif


using namespace std;


struct buffs_w_ui
{
    char commd[3];
    char msg[100];
    char last_entry[20];
};

using namespace std;

class Worker: public QObject
{
    Q_OBJECT
private:

    QSound *sound_IDMNF;
    QSound *sound_IDMF;
    QSound *sound_WMNF;
    QSound *sound_WMF;
    QSound *sound_SNR;
    QSound *sound_SR;
    QSound *sound_Timeout;
    QSound *sound_ARFID;
    QSound *sound_AWRIST;
    QSound *sound_Three;
    buffs_w_ui msg_ui;
    mqd_t q_ui;
    struct mq_attr msgq_attr;
    unsigned int sender;
    int msg_q;
    char buff_name[60];
    int i;

public:

    explicit Worker(QObject *parent=0);
    sem_t *sem_Compare;
    //void setText(const QString &image);

public slots:
    void process();// Constructor
signals:
    void signal_RFID_MatchFound(const QString &,const QString &);
    void signal_RFID_MatchNotFound();
    void signal_WristMatch(const QString &);
    void signal_Speech_Match(const QString &);
    void signal_Timeout(const QString &);
    void signal_Alert(const QString &);
    void signal_Hide();



};

#endif // GUIUPDATER_H
