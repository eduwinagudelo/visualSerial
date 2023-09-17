#ifndef LCDCOMM_H
#define LCDCOMM_H

// standar includes
#include <algorithm>
#include <functional>
#include <string>
#include <thread>
#include <vector>

// QT includes
#include <QByteArray>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

// Custom includes
#include "visualconstanst.h"

// Definitions
static const int MAX_LINES = 2;

class LCDComm : public QObject
{
    Q_OBJECT
public:
    LCDComm();
    LCDComm( std::function<void(QString)> f);
    LCDComm(QString portname, std::function<void(QString)> f);
    virtual ~LCDComm();

    // Process methods
    int startComm(QString port);
    int stopComm();
    std::vector<std::string> getPorList();

    // Write methods
    void writeCharacter(char c);
    void writeString(QString string);
    void writeNumber(int i);
    void writeNumber(float f);
    void clear();
    void changeLine();

    // special setter
    void setPortName(QString name);
    void setAppendCallback(std::function<void(QString)> f);

    // getters
    bool portIsOpen();

private:
    QSerialPort* port;
    QString portName;
    //std::thread commProcess;
    std::vector<std::string> portList;
    int currentLine;
    //bool runStatus;
    std::function<void(QString)> appendCallback;

    // Private specific methods
    int configPort();
    int writePort(SENDTYPE type, void *value);

private slots:
    void readPort();
    //void monitorPort(); // Thread method

};

#endif // LCDCOMM_H
