#include "exchangehandler.h"
#include <QDebug>
#include <QtEndian>
#include <QDataStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <qcoreapplication.h>

ExchangeHandler::ExchangeHandler(QObject *parent)
    : QObject{parent}, m_socket(nullptr)
{
    connect(&reconnectTimer, &QTimer::timeout, this, &ExchangeHandler::connectInternal);
    connect(qApp, &QCoreApplication::aboutToQuit, this, [this](){
        restartConnection = false;
        if (m_socket) {  // Check before using
            m_socket->abort();
            m_socket->deleteLater();
            m_socket = nullptr;
        }
    });
    connectInternal();
    QTimer::singleShot(1000, [this](){
        if((m_socket->state() != QAbstractSocket::ConnectedState) && (maxSequence < 1)) {
            qDebug() << "Please restart the server";
        }
    });
}

void ExchangeHandler::onConnected()
{
    reconnectTimer.stop();
    restartConnection = true;

    if(receivedSequences.empty()) {
        requestPacket(1, 0);
    } else {
        for(quint8 missnNum = 1; missnNum <= maxSequence; ++missnNum) {
            if(!receivedSequences.contains(missnNum)) {
                requestPacket(2, missnNum);
            }
        }
    }
}
void ExchangeHandler::requestPacket(quint8 reqType, quint8 packetId)
{
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState)
        return;

    QByteArray request;
    request.append(reqType);
    request.append(packetId);

    m_socket->write(request);
    m_socket->flush();

}

void ExchangeHandler::saveToJson() {
    QVector<QJsonObject> sortedTrades;

    qDebug() << Q_FUNC_INFO << receivedSequences.keys().size();

    for (auto [seq, ticker] : receivedSequences.asKeyValueRange()) {
        QJsonObject tradeObj;

        tradeObj["Symbol"  ] = QString::fromLocal8Bit(ticker.symbol, 4).trimmed();
        tradeObj["BuySell" ] = QString(ticker.buySellIndicator);
        tradeObj["Quantity"] = static_cast<int>(ticker.quantity);
        tradeObj["Price"   ] = static_cast<int>(ticker.price);
        tradeObj["Sequence"] = static_cast<int>(ticker.sequence);

        sortedTrades.append(tradeObj);
    }

    std::sort(sortedTrades.begin(), sortedTrades.end(), [](const QJsonObject &a, const QJsonObject &b) {
        return a["Sequence"].toInt() < b["Sequence"].toInt();
    });

    QJsonArray tradeArray;
    for (const auto &trade : sortedTrades) {
        tradeArray.append(trade);
    }

    QJsonDocument jsonDoc(tradeArray);

    QString filePath = QDir::currentPath() + "/tickerUpdates.json";
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Failed to save trade data to result.json:" << file.errorString();
        return;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "Json file saved to:" << filePath;
}


void ExchangeHandler::onReadyRead()
{
    if (!m_socket) return;

    const int PACKET_LEN = sizeof(TickerPacket);

    while (m_socket->bytesAvailable() >= PACKET_LEN) // Ensure we have at least packet size
    {
        QByteArray packet = m_socket->read(PACKET_LEN);
        if (packet.size() < PACKET_LEN)
            continue;

        TickerPacket ticker;
        memcpy(&ticker, packet.constData(), PACKET_LEN); // Copy data into struct

        ticker.quantity = qFromBigEndian(ticker.quantity);
        ticker.price = qFromBigEndian(ticker.price);
        ticker.sequence = qFromBigEndian(ticker.sequence);

        receivedSequences[ticker.sequence] = ticker;
    }

    for(auto key: receivedSequences.keys()) maxSequence = qMax(maxSequence, key);

    if((receivedSequences.size() == maxSequence) && (maxSequence > 0)) {
        qWarning() << Q_FUNC_INFO << (QString("Updated file with %1 ticker data.").arg(maxSequence));
        saveToJson();
    }
}

void ExchangeHandler::connectInternal()
{
    if (m_socket) {
        restartConnection = false;
        closeSocket();
    }
    restartConnection = true;

    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &ExchangeHandler::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &ExchangeHandler::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &ExchangeHandler::onErrorOccurred);

    m_socket->connectToHost("127.0.0.1", 3000, QAbstractSocket::ReadWrite, QAbstractSocket::AnyIPProtocol);
}
void ExchangeHandler::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    // qDebug() << "Socket error:" << m_socket->errorString();
    closeSocket();
}
void ExchangeHandler::closeSocket()
{
    if (m_socket) {
        m_socket->abort();
        m_socket->deleteLater();
        m_socket = nullptr;
    }
    if(restartConnection) reconnectTimer.start(RECONNECT_TIME_MS);
}

