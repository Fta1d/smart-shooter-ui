#ifndef UDPCOMMANDSENDER_H
#define UDPCOMMANDSENDER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QMutex>
#include <msgpack.hpp>

class UdpCmdSender : public QObject {
    Q_OBJECT
    
private:
    QUdpSocket *socket;
    QTimer *sendTimer;
    mutable QMutex dataMutex;
    
    QString destinationAddress;
    quint16 destinationPort;
    
    int xValue;
    int yValue;
    bool shotValue;
    bool activeValue;
    int mode;
    bool shutdownValue;
    
    void prepareAndSendMessage();
    
public:
    UdpCmdSender(QObject *parent = nullptr);
    ~UdpCmdSender();
    
    void setDestination(const QString &address, quint16 port);
    
    // Setters for command values
    void setXValue(int value);
    void setYValue(int value);
    void setShotValue(bool value);
    void setActiveValue(bool value);
    void setMode(int value);
    void setShutdownValue(bool value);
    
    // Getters for command values
    int getXValue() const;
    int getYValue() const;
    bool getShotValue() const;
    bool getActiveValue() const;
    
public slots:
    // Modified to accept parameters for address and port
    void startSending(const QString &address = QString(), quint16 port = 0);
    void stopSending();
    
signals:
    void messageSent(const QString &message);
    void errorOccurred(const QString &errorMessage);
};

#endif // UDPCOMMANDSENDER_H