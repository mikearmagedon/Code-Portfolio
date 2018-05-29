#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSound>
#include <semaphore.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    /**** Objects of type QSound which have the responsability of reproducing the respective sound ****/
    QSound *sound_IDMNF;
    QSound *sound_IDMF;
    QSound *sound_WMNF;
    QSound *sound_WMF;
    QSound *sound_SNR;
    QSound *sound_SR;

public:
    explicit MainWindow(QWidget *parent = 0);// Constructor
    ~MainWindow();//Destructor

private slots:
    /** Slots of each connected signal **/
    void slot_WristMatch(QString);
    void slot_RFID_MatchFound(QString,QString);
    void slot_RFID_MatchNotFound();
    void slot_Speech_Match(QString);
    void slot_Timeout(QString);
    void slot_Alert(QString);
    void slot_Hide();

private:
    Ui::MainWindow *ui; // Object used for accessing the objects of the UI
};

#endif // MAINWINDOW_H


