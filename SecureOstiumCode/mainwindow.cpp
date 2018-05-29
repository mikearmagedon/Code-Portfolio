#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->hide();
    ui->label->hide();

    QThread *thread = new QThread();
    Worker *worker = new Worker();
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(signal_RFID_MatchFound(const QString &,const QString &)),this,SLOT(slot_RFID_MatchFound(QString,QString)));
    connect(worker, SIGNAL(signal_RFID_MatchNotFound()),this,SLOT(slot_RFID_MatchNotFound()));
    connect(worker, SIGNAL(signal_WristMatch(const QString &)),this,SLOT(slot_WristMatch(QString)));
    connect(worker,SIGNAL(signal_Speech_Match(QString)),this,SLOT(slot_Speech_Match(QString)) );
    connect(worker,SIGNAL(signal_Timeout(QString)),this,SLOT(slot_Timeout(QString)) );
    connect(worker,SIGNAL(signal_Alert(QString)),this,SLOT(slot_Alert(QString)) );
    connect(worker,SIGNAL(signal_Hide()),this,SLOT(slot_Hide()));

    thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**DISPLAY MATCH FOUND RESULT REGARDING THE RFID STEP*/
void MainWindow::slot_RFID_MatchFound(QString text, QString entry)
{
    ui->textBrowser->show();
    ui->label->show();

    ui->textBrowser->clear();
    ui->label->clear();
    ui->textBrowser->setTextColor(QColor("orange"));

    ui->label->setText("Last entry:"+entry);
    ui->textBrowser->append("Welcome "+text);
    ui->textBrowser->append("Please, press the button to start recording");

}

/**DISPLAY MATCH NOT FOUND RESULT REGARDING THE RFID STEP*/
void MainWindow::slot_RFID_MatchNotFound()
{
    ui->textBrowser->show();
    ui->textBrowser->clear();
    ui->textBrowser->setTextColor(QColor("red"));
    ui->textBrowser->append("Card's ID not identified!!");
    ui->textBrowser->append("Please, pass your card again!");
}

/**DISPLAY RESULT REGARDING THE WRIST MOVEMENTE STEP*/
void MainWindow::slot_WristMatch(QString text)
{
    ui->textBrowser->show();
    if (text=="WMF")
    {
        ui->textBrowser->clear();
        ui->textBrowser->setTextColor(QColor("orange"));
        ui->textBrowser->append("Well Done!!");
        ui->textBrowser->append("Please, push the door!");
        ui->label->hide();
    }else if (text=="WMNF")
    {
        ui->textBrowser->clear();
        ui->textBrowser->setTextColor(QColor("red"));
        ui->textBrowser->append("Match not found!!");
        ui->textBrowser->append("Please, put your arm in the default position and wait for the LED to turn on!");
    }
}

/**DISPLAY RESULT REGARDING THE SPEECH STEP*/
void MainWindow::slot_Speech_Match(QString text)
{
    ui->textBrowser->show();
    if (text == "SR")
    {
        ui->textBrowser->clear();
        ui->textBrowser->setTextColor(QColor("orange"));
        ui->textBrowser->append("Word Recognized!");
        ui->textBrowser->append("Please, put your wrist in the default position and wait for the LED to turn on!");
    }else if (text== "SNR")
    {
        ui->textBrowser->clear();
        ui->textBrowser->setTextColor(QColor("red"));
        ui->textBrowser->append("Word not Recognized!!");
        ui->textBrowser->append("Please, when you're ready press the record button again!");
    }
}

void MainWindow::slot_Timeout(QString text)
{
    ui->textBrowser->show();
    if (text=="TIMOUT")
    {
        ui->textBrowser->clear();
        ui->textBrowser->setTextColor(QColor("red"));
        ui->textBrowser->append("A Timeout has been given!");
    }else if (text=="THREE")
    {
        ui->textBrowser->clear();
        ui->textBrowser->setTextColor(QColor("red"));
        ui->textBrowser->append("You've ran out of tries!");
    }
}

void MainWindow::slot_Alert(QString text)
{
    ui->textBrowser->show();
    if (text=="RFID")
    {
        ui->textBrowser->clear();
        ui->textBrowser->setTextColor(QColor("red"));
        ui->textBrowser->append("RFID step: Approach your card!");

    }else if (text=="WRIST")
    {
        ui->textBrowser->clear();
        ui->textBrowser->setTextColor(QColor("red"));
        ui->textBrowser->append("Wrist step: Put your arm in the default position!");
    }
}

void MainWindow::slot_Hide()
{
    ui->label->hide();
    ui->textBrowser->hide();
}



