#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //serial = new QSerialPort(this);
    //connect(serial, &QSerialPort::readyRead, this, &MainWindow::leerModulo);
    //linea = 1;

    // This part is to check the number of serial ports
    /*Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
    /    ui->txbRecibido->appendPlainText("Puerto Encontrado:" + port.portName());
        this->portList.push_back(port.portName().toStdString());
    }*/
    //std::function<void (QString&)> appendToConsole = std::bind(QPlainTextEdit::appendPlainText, ui->(&txbRecibido), std::placeholders::_1);
    std::function<void(QString)> appendToConsole = [&](QString val){ ui->txbRecibido->appendPlainText(val); };
    lcd = new LCDComm(appendToConsole);
    lcd->getPorList();
}

MainWindow::~MainWindow()
{
    delete lcd;
    delete ui;
}

void MainWindow::openSerialPort(){
    QMessageBox msg;
    // check if the port is in the list
    /*auto itport = std::find(portList.begin(), portList.end(), ui->txbSerialPort->text().toStdString());
    if(itport == portList.end()){
        msg.setText("Please introduce a valid port name");
        msg.setInformativeText("Check the list in the console");
        msg.exec();
        return;
    }*/
    ui->lcdNumber->display(0);
    /*serial->setPortName(ui->txbSerialPort->text());
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if(serial->open(QIODevice::ReadWrite)){
        ui->txbRecibido->appendPlainText("Puerto " + serial->portName() + " abierto");
    }
    else{
        ui->txbRecibido->appendPlainText("Puerto " + serial->portName() + " en error:" + serial->errorString());
    }*/
    if(lcd->startComm(ui->txbSerialPort->text()) < 1)
    {
        msg.setText("The port cannot be initialized");
        msg.setInformativeText("Check the list in the console");
        msg.exec();
        return;
    }

}

void MainWindow::enviarModulo(unsigned char val){
    /*char envio[2];
    envio[0] = comando;
    envio[1] = val;
    serial->write(envio);*/
    lcd->writeCharacter(val);
}

void MainWindow::leerModulo(){
    /*char entrada = 0xA;
    char borrado = 0xB;
    QByteArray barr = serial->readAll();
    for(int i = 0; i < barr.length(); i++){
        if(barr.at(i) == entrada){
            barr[i] = 0x3E;
        }
        else if(barr.at(i) == borrado)
            barr[i] = 0x3C;
    }
    QString str = QString(barr);
    ui->lcdNumber->display( ui->lcdNumber->intValue() +  str.length());
    ui->txbRecibido->insertPlainText(str);*/
}

void MainWindow::closeSerialPort(){
    /*if(serial->isOpen())
        serial->close();
    ui->txbRecibido->appendPlainText("Puerto " + serial->portName() + " cerrado");*/
    lcd->stopComm();
}

void MainWindow::on_btnAbrirPuerto_clicked()
{
    if(lcd->portIsOpen()) {
        lcd->stopComm();
        ui->btnAbrirPuerto->setText("Cerrar");
    }
    else {
        lcd->startComm(ui->txbSerialPort->text());
        ui->btnAbrirPuerto->setText("Abrir");
    }
}

void MainWindow::on_btnBorrar_clicked()
{
    //enviarModulo(0xb, 0xb);
    lcd->clear();
}

void MainWindow::on_btnEnviar_clicked()
{
    /*for(int i = 0; i < ui->txbEnvio->text().length(); i++){
        QChar t = ui->txbEnvio->text().at(i);
        //enviarModulo(0xA, t.toLatin1());
    }*/
    lcd->writeString(ui->txbEnvio->text());
    ui->txbEnvio->text().clear();
}

void MainWindow::on_btnCambiarL_clicked()
{
    //enviarModulo(0xC, (linea%2 == 0 ? 0x1 : 0x2));
    lcd->changeLine();
    linea++;
}

void MainWindow::on_btnEnviarNum_clicked()
{
    char val;
    val = static_cast<char>(ui->spinBox->value());
    //enviarModulo(0xD,val);
    lcd->writeNumber(val);
}
