#include "inc/udpcommandsender.h"
#include <QDebug>

UdpCmdSender::UdpCmdSender(QObject *parent) : QObject(parent) {
    // Initialize socket
    socket = new QUdpSocket(this);
    
    // Initialize timer for periodic sending (100Hz = 10ms interval)
    sendTimer = new QTimer(this);
    sendTimer->setInterval(10); // 10ms for 100Hz
    
    // Connect timer timeout signal to send message slot
    connect(sendTimer, &QTimer::timeout, this, &UdpCmdSender::prepareAndSendMessage);
    
    // Initialize default values
    destinationAddress = "127.0.0.1"; // Default to localhost
    destinationPort = 5050;           // Default to port 5050
    
    xValue = 0;
    yValue = 0;
    shotValue = false;
    activeValue = false;
}

UdpCmdSender::~UdpCmdSender() {
    stopSending();
    
    if (socket) {
        delete socket;
        socket = nullptr;
    }
    
    if (sendTimer) {
        delete sendTimer;
        sendTimer = nullptr;
    }
}

void UdpCmdSender::setDestination(const QString &address, quint16 port) {
    QMutexLocker locker(&dataMutex);
    destinationAddress = address;
    destinationPort = port;
    
    emit messageSent("Destination set to: " + address + ":" + QString::number(port));
}

void UdpCmdSender::setXValue(int value) {
    QMutexLocker locker(&dataMutex);
    xValue = value;
}

void UdpCmdSender::setYValue(int value) {
    QMutexLocker locker(&dataMutex);
    yValue = value;
}

void UdpCmdSender::setShotValue(bool value) {
    QMutexLocker locker(&dataMutex);
    shotValue = value;
}

void UdpCmdSender::setActiveValue(bool value) {
    QMutexLocker locker(&dataMutex);
    activeValue = value;
}

int UdpCmdSender::getXValue() const {
    QMutexLocker locker(&dataMutex);
    return xValue;
}

int UdpCmdSender::getYValue() const {
    QMutexLocker locker(&dataMutex);
    return yValue;
}

bool UdpCmdSender::getShotValue() const {
    QMutexLocker locker(&dataMutex);
    return shotValue;
}

bool UdpCmdSender::getActiveValue() const {
    QMutexLocker locker(&dataMutex);
    return activeValue;
}

void UdpCmdSender::startSending(const QString &address, quint16 port) {
    QMutexLocker locker(&dataMutex);
    destinationAddress = address;
    destinationPort = port;
    
    // Start the timer to send messages
    if (!sendTimer->isActive()) {
        sendTimer->start();
        emit messageSent("Started sending commands to " + address + ":" + QString::number(port) + " at 100Hz");
    }
}

void UdpCmdSender::stopSending() {
    if (sendTimer->isActive()) {
        sendTimer->stop();
        emit messageSent("Stopped sending commands");
    }
}

void UdpCmdSender::prepareAndSendMessage() {
    QMutexLocker locker(&dataMutex);
    
    // Create the message string in the format "x,y,shot,active"
    int shotInt = shotValue ? 1 : 0;
    int activeInt = activeValue ? 1 : 0;
    
    QString messageStr = QString("%1,%2,%3,%4").arg(xValue).arg(yValue).arg(shotInt).arg(activeInt);
    
    // Pack the message using MsgPack
    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> packer(&buffer);
    std::string stdStr = messageStr.toStdString();
    packer.pack(stdStr);
    
    // Send the packed message
    qint64 bytesSent = socket->writeDatagram(
        buffer.data(), 
        buffer.size(), 
        QHostAddress(destinationAddress), 
        destinationPort
    );
    
    if (bytesSent == -1) {
        emit errorOccurred("Failed to send UDP datagram: " + socket->errorString());
    }
    // Uncomment for debugging (but will flood the log at 100Hz)
    // else {
    //    emit messageSent("Sent: " + messageStr);
    // }
}