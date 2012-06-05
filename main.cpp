#include <QtGui>
#include "mainwindow.h"
#include "behaviorcontrol.h"
#include "networkdatainterface.h"

// Global def
BehaviorControl *bControl;

NetworkDataInterface *inputNetworkData;
NetworkDataInterface *outputNetworkData;
QThread NetworkingThread;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bControl = new BehaviorControl();
    inputNetworkData = new NetworkDataInterface(0,QHostAddress("192.168.1.201"),10001,udp_input);
    outputNetworkData = new NetworkDataInterface(0,QHostAddress("192.168.1.202"),10002,udp_output);
    bControl->connect(inputNetworkData,SIGNAL(updatedData(char,QTime)),SLOT(newDataReceived(char,QTime)));
    //dataModule->moveToThread(&NetworkingThread);
    //dataModule->connect(&NetworkingThread, SIGNAL(started()), SLOT(initializeInterface()));

    MainWindow w;

    w.show();

    //NetworkingThread.connect(&a,SIGNAL(lastWindowClosed()),SLOT(quit()));
    //NetworkingThread.start(QThread::HighestPriority);

    return a.exec();
}
