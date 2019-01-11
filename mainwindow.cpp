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


// get the hardware address
    QNetworkInterface interface = QNetworkInterface::interfaceFromName("eth0");
    ui->hwaddr_label->setText(interface.hardwareAddress());


    // list all ipv4 addresses
    QStringList addresses;
    foreach(QNetworkAddressEntry address, interface.addressEntries())
    {
        if( address.ip().protocol() == QAbstractSocket::IPv4Protocol)
            addresses << address.ip().toString();
    }

    ui->ip_label->setText( addresses.join(", "));

    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        // Return only the first non-loopback MAC Address
        qDebug() << "Got interface " << netInterface.name() << " with hwaddr " << netInterface.hardwareAddress() << "...";
    }


    // get the local hostname
    ui->status_label->setText( QString("Status '%1':").arg(QHostInfo::localHostName()) );


    // setup the internet access test
    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished,
        this, [=](QNetworkReply *reply) {
            if (reply->error()) {
                ui->internet_label->setText( reply->errorString() );
                return;
            }

            QString answer = reply->readAll();

            if( answer.contains("<HTML><HEAD><TITLE>Success</TITLE></HEAD><BODY>Success</BODY></HTML>") ) {
                ui->internet_label->setStyleSheet("border-style: solid; border-color #ffffff; color: #00ff00;");
                ui->internet_label->setText("Internet Access OK");
            } else {
                ui->internet_label->setStyleSheet("border-style: solid; border-color #ffffff; color: #ff0000;");
                ui->internet_label->setText("No Internet Access");
            }
        }
    );
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

    request.setUrl(QUrl("http://captive.apple.com/hotspot-detect.html"));
    manager->get(request);
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
