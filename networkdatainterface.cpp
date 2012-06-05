#include "networkdatainterface.h"
#include <QtNetwork/QHostAddress>
#include <QStringList>

NetworkDataInterface::NetworkDataInterface(QObject *parent,
  QHostAddress host, quint16 local_port, UDP_IOType iotype) :
    host(host), local_port(local_port), iotype(iotype), QObject(parent)
{
  isInitialized = false;
  initializeInterface();
}

void NetworkDataInterface::initializeInterface(void)
{
  if (isInitialized) {
    udpSocket->disconnectFromHost();
    udpSocket->disconnect(this,0);
    delete udpSocket;
  }

  isInitialized = true;
  udpSocket = new QUdpSocket(this);
  connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleErrors(QAbstractSocket::SocketError)));

  bool bind_result = udpSocket->bind(QHostAddress::LocalHost, local_port);
  if (bind_result) {
    qDebug() << "Successfully bound port." << (int) local_port;
    status = good;
  }
  else {
    qDebug() << "Unsuccessful in binding port. " << udpSocket->error();
    status = bad;
  }
  udpSocket->connectToHost(host,remote_port,QIODevice::ReadWrite);
  connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

  sendQueryDatagram();
}


void NetworkDataInterface::handleErrors(QAbstractSocket::SocketError err) {
  qDebug() << "Network data error signal " << err;
}


void NetworkDataInterface::sendDatagram(QByteArray datagram)
{
  udpSocket->write(datagram.data(), datagram.size());
}

void NetworkDataInterface::sendQueryDatagram() {
  QByteArray newdatagram = QByteArray("P");
  qDebug() << "Sending P";
  sendDatagram(newdatagram);
}

void NetworkDataInterface::sendTriggerDatagram(int chan, int pulseLength) {
    QByteArray newdatagram = QByteArray("S");
    newdatagram.append(1<<chan);
    qDebug() << "Sending S" << chan;
    sendDatagram(newdatagram);
}

void NetworkDataInterface::readPendingDatagrams() {
    QTime t;
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
        t = QTime::currentTime();

        processTheDatagram(datagram, t);
    }
}

void NetworkDataInterface::processTheDatagram(QByteArray datagram,  QTime t) {
  if ((datagram[0] == 'S') & (iotype == udp_output)) {
    // this is the proper return flag after commanding a state change
    return;
  }

  if (datagram[0] != 'D') {
    qDebug() << "Network error: wierd datagram: " << datagram;
    return;
  }

  QList<QByteArray> parsedMessage;
  parsedMessage = datagram.split(' ');
  if (parsedMessage.length() != 2) {
      qDebug() << "Network error: error datagram received "
               << "(" << parsedMessage.length() << ")"
               << datagram;
    return;
  }

  bool ok;
  int MyId = parsedMessage[0].mid(1).toInt(&ok,16);
  int CurrentState = parsedMessage[1].toInt(&ok,16);
  qDebug() << "Current state from [" << MyId << "]:" << CurrentState << "at" << t.toString("h:mm:ss.zzz");

  emit updatedData(CurrentState, t);
}
