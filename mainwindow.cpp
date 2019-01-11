#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL( timeout()), this, SLOT(updateTimerTimeout()));
    updateTimer->start(1000);

    ntpProcess.setProgram("/usr/bin/ntpstat");

    eth0Process.setProgram("/sbin/ifconfig");
    eth0Process.setArguments(QStringList("eth0"));

    wlan0Process.setProgram("/sbin/ifconfig");
    wlan0Process.setArguments(QStringList("wlan0"));

    wProcess.setProgram("/usr/bin/w");

    connect(&ntpProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readNtpOutput()));
    connect(&eth0Process, SIGNAL(readyReadStandardOutput()), this, SLOT(readEth0Output()));
    connect(&wlan0Process, SIGNAL(readyReadStandardOutput()), this, SLOT(readWlan0Output()));
    connect(&wProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readWOutput()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTimerTimeout()
{
    ntpProcess.kill();
    ntpProcess.start();

    wlan0Process.kill();
    wlan0Process.start();

    eth0Process.kill();
    eth0Process.start();

    wProcess.kill();
    wProcess.start();
}

void MainWindow::readNtpOutput()
{
    QString string = ntpProcess.readAllStandardOutput();
    ui->ntpstat_textedit->setPlainText(string);
}

void MainWindow::readEth0Output()
{
    QString string = eth0Process.readAllStandardOutput();
    ui->eth0_textedit->setPlainText(string);
}

void MainWindow::readWlan0Output()
{
    QString string = wlan0Process.readAllStandardOutput();
    ui->wlan0_textedit->setPlainText(string);
}

void MainWindow::readWOutput()
{
    QString string = wProcess.readAllStandardOutput();
    ui->uptime_textedit->setPlainText(string);
}
