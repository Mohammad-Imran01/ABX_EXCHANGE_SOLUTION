#ifndef EXCHANGEHANDLER_H
#define EXCHANGEHANDLER_H

#define RECONNECT_TIME_MS 1000

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QHash>
#include <QAbstractSocket>
#include <QTimer>

#pragma pack(1)
struct TickerPacket {
    char symbol[4];
    char buySellIndicator;
    quint32 quantity;
    quint32 price;
    quint32 sequence;
};
#pragma pack()

class ExchangeHandler : public QObject
{
    Q_OBJECT
public:
    explicit ExchangeHandler(QObject *parent = nullptr);

private slots:
    void connectInternal();
    void onConnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void closeSocket();

private:
    void requestPacket(quint8 reqType, quint8 packetId);
    void saveToJson();


    QHash<quint32, TickerPacket> receivedSequences;
    quint32 maxSequence = 0;
    QTcpSocket *m_socket = nullptr;
    QTimer reconnectTimer;

    int safetyCheck = 10;
    bool restartConnection = false;
};

#endif // EXCHANGEHANDLER_H
