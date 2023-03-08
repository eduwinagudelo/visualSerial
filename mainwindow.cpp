#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::leerModulo);

    linea = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSerialPort(){
    ui->lcdNumber->display(0);
    serial->setPortName(ui->txbSerialPort->text());
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
    }

}

void MainWindow::enviarModulo(unsigned char comando, unsigned char val){
    char envio[2];
    envio[0] = comando;
    envio[1] = val;
    serial->write(envio);
}

void MainWindow::leerModulo(){
    char entrada = 0xA;
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
    ui->txbRecibido->insertPlainText(str);
}

void MainWindow::closeSerialPort(){
    if(serial->isOpen())
        serial->close();
    ui->txbRecibido->appendPlainText("Puerto " + serial->portName() + " cerrado");
}

void MainWindow::on_btnAbrirPuerto_clicked()
{
    if(serial->isOpen()) {
        this->closeSerialPort();
        ui->btnAbrirPuerto->setText("Cerrar");
    }
    else {
        this->openSerialPort();
        ui->btnAbrirPuerto->setText("Abrir");
    }
}

void MainWindow::on_btnBorrar_clicked()
{
    enviarModulo(0xb, 0xb);
}

void MainWindow::on_btnEnviar_clicked()
{
    for(int i = 0; i < ui->txbEnvio->text().length(); i++){
        QChar t = ui->txbEnvio->text().at(i);
        enviarModulo(0xA, t.toLatin1());
    }
    ui->txbEnvio->text().clear();
}

void MainWindow::on_btnCambiarL_clicked()
{
    enviarModulo(0xC, (linea%2 == 0 ? 0x1 : 0x2));
    linea++;
}

void MainWindow::on_btnEnviarNum_clicked()
{
    char val;
    val = static_cast<char>(ui->spinBox->value());
    enviarModulo(0xD,val);
}
