#ifndef NETWORKDATAINTERFACE_H
#define NETWORKDATAINTERFACE_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QTime>

enum UDP_IOType {udp_input, udp_output};

class NetworkDataInterface : public QObject
{
    Q_OBJECT
public:
    explicit NetworkDataInterface(QObject *parent = 0,
             QHostAddress host = QHostAddress("10.1.1.81"),
             quint16 local_port = 10001, UDP_IOType iotype = udp_input);

  void processTheDatagram(QByteArray, QTime);

  UDP_IOType iotype;

  QHostAddress host;
  static const quint16 remote_port = 24601;
  const quint16 local_port;

  enum Status {good, bad};
  Status status;

signals:
  void updatedData(char data, QTime time);

public slots:
  void initializeInterface(void);
  void readPendingDatagrams(void);
  void sendDatagram(QByteArray);
  void sendQueryDatagram(void);
  void handleErrors(QAbstractSocket::SocketError);
  void sendTriggerDatagram(int chan, int pulseLength);

private:
  QUdpSocket *udpSocket;
  bool isInitialized;
};

extern NetworkDataInterface *inputNetworkData;
extern NetworkDataInterface *outputNetworkData;

#endif // NETWORKDATAINTERFACE_H
