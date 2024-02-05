#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    ui->lcdNumber->display(0);
    if(lcd->startComm(ui->txbSerialPort->text()) < 1)
    {
        msg.setText("The port cannot be initialized");
        msg.setInformativeText("Check the list in the console");
        msg.exec();
        return;
    }

}

void MainWindow::enviarModulo(unsigned char val){
    lcd->writeCharacter(val);
}

void MainWindow::leerModulo(){
}

void MainWindow::closeSerialPort(){
    lcd->stopComm();
}

void MainWindow::on_btnAbrirPuerto_clicked()
{
    if(lcd->portIsOpen()) {
        lcd->stopComm();
        ui->btnAbrirPuerto->setText("Abrir");
    }
    else {
        lcd->startComm(ui->txbSerialPort->text());
        ui->btnAbrirPuerto->setText("Cerrar");
    }
}

void MainWindow::on_btnBorrar_clicked()
{
    lcd->clear();
}

void MainWindow::on_btnEnviar_clicked()
{
    lcd->writeString(ui->txbEnvio->text());
    ui->txbEnvio->text().clear();
}

void MainWindow::on_btnCambiarL_clicked()
{
    lcd->changeLine();
    linea++;
}

void MainWindow::on_btnEnviarNum_clicked()
{
    char val;
    val = static_cast<char>(ui->spinBox->value());
    lcd->writeNumber(val);
}
