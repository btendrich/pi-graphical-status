#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkInterface>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateTimerTimeout();
    void readNtpOutput();
    void readEth0Output();
    void readWlan0Output();
    void readWOutput();

private:
    Ui::MainWindow *ui;
    QTimer *updateTimer;
    QProcess ntpProcess;
    QProcess eth0Process;
    QProcess wlan0Process;
    QProcess wProcess;
};

#endif // MAINWINDOW_H
