#include "lcdcomm.h"

LCDComm::LCDComm()
{
    this->currentLine = 0;
    //this->runStatus = false;
    this->appendCallback = nullptr;
    this->port = new QSerialPort(this);
    QObject::connect(port, &QSerialPort::readyRead, this, &LCDComm::readPort);
}

LCDComm::LCDComm(QString portname, std::function<void(QString)> f)
{
    this->currentLine = 0;
    //this->runStatus = false;
    this->appendCallback = f;
    this->portName = portname;
    this->port = new QSerialPort(this);
    QObject::connect(port, &QSerialPort::readyRead, this, &LCDComm::readPort);
}

LCDComm::LCDComm( std::function<void(QString)> f)
{
    this->currentLine = 0;
    //this->runStatus = false;
    this->appendCallback = f;
    this->port = new QSerialPort(this);
    QObject::connect(port, &QSerialPort::readyRead, this, &LCDComm::readPort);
}

LCDComm::~LCDComm()
{
    if(port->isOpen())
    {
        port->close();
    }
}

int LCDComm::startComm(QString port)
{
    this->portName = port;
    return this->configPort();
}

int LCDComm::stopComm()
{
    this->appendCallback("Closing port " + this->portName);
    this->port->close();
    return 0;
}

std::vector<std::string> LCDComm::getPorList()
{
    // This part is to check the number of serial ports
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        this->appendCallback("Puerto Encontrado:" + port.portName());
        this->portList.push_back(port.portName().toStdString());
    }
    return this->portList;
}

void LCDComm::writeCharacter(char c)
{
    this->appendCallback(QString::fromStdString("Writing char -> ") +  QString::fromStdString(std::to_string(c)));
    this->writePort(SENDTYPE::CHAR, &c);
}

void LCDComm::writeString(QString string)
{
    this->appendCallback("Writing String -> " +  string);
    std::string strTmp = string.toStdString();
    for(size_t i = 0; i < strTmp.size(); i++)
    {
        this->writeCharacter(strTmp.c_str()[i]);
    }
}

void LCDComm::writeNumber(int i)
{
    this->appendCallback(QString::fromStdString("Writing number -> ") +  QString::fromStdString(std::to_string(i)));
    this->writePort(SENDTYPE::INTEGER, &i);
}

void LCDComm::writeNumber(float f)
{
    this->appendCallback(QString::fromStdString("Writing number[Float] -> ") +  QString::fromStdString(std::to_string(f)));
    QString value = QString::number(f);
    this->writeString(value);
}

void LCDComm::clear()
{
    this->appendCallback("Clearing the LCD");
    this->writePort(SENDTYPE::CLEAR,nullptr);
}

void LCDComm::changeLine()
{
    this->appendCallback("Changing line in the LCD");
    if(this->currentLine == 0){
        this->currentLine = 1;
        this->writePort(SENDTYPE::NEXTLINE,&(this->currentLine));
    }
}

void LCDComm::setPortName(QString name)
{
    this->portName = name;
}

void LCDComm::setAppendCallback(std::function<void (QString)> f)
{
    this->appendCallback = f;
}

bool LCDComm::portIsOpen()
{
    return port->isOpen();
}

int LCDComm::configPort()
{
    auto itport = std::find(portList.begin(), portList.end(), this->portName.toStdString());
    if(itport == portList.end()){
        this->appendCallback("Port " + this->portName + " is not in the list of ports available");
        return -1;
    }

    port->setPortName(this->portName);
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setStopBits(QSerialPort::OneStop);
    port->setParity(QSerialPort::NoParity);
    port->setFlowControl(QSerialPort::NoFlowControl);
    if(port->open(QIODevice::ReadWrite)){
        this->appendCallback("Puerto " + port->portName() + " abierto");
        return 1;
    }
    else{
        this->appendCallback("Puerto " + port->portName() + " en error:" + port->errorString());
        return 0;
    }
}

int LCDComm::writePort(SENDTYPE type, void *value)
{
    if(!this->portIsOpen())
    {
        this->appendCallback("The port is not open, please open first");
        return -1;
    }
    int len = 0;
    char data[5] = {0, 0, 0, 0, 0};
    switch(type){
    case SENDTYPE::CHAR : // character
        data[0] = S_CHAR;
        data[1] = (char)*(static_cast<char*>(value));
        len = 2;
        break;
    case SENDTYPE::INTEGER : // integer
        data[0] = S_NUM;
        len = 1;
        for(unsigned int j = 0 ; (j < sizeof(int) && j < 2); j++){
            data[j+1] = static_cast<char>(((*(static_cast<int*>(value))) >> (j*8)) & 0xff);
            len++;
        }
        break;
    case SENDTYPE::FLOAT : // float
        data[0] = S_NUM;
        data[1] = (char)*(static_cast<char*>(value));
        len = 2;
        break;
    case SENDTYPE::CLEAR :
        data[0] = S_CLEAR;
        data[1] = 0xB;
        len = 2;
        break;
    case SENDTYPE::NEXTLINE :
        data[0] = S_CHANGELINE;
        data[1] = 0xC;
        len = 2;
        break;
    default: break;
    }

    // Process
    if(data[0] == 0){
        return len;
    }

    port->write(data, len);
    return len;
}

void LCDComm::readPort()
{
    QByteArray barr = port->readAll();
    for(int i = 0; i < barr.length(); i++){
        if(barr.at(i) == S_CHAR){
            barr[i] = 0x3E;
        }
        else if(barr.at(i) == S_CLEAR)
            barr[i] = 0x3C;
    }
    QString str = QString(barr);
    this->appendCallback(str);
}
