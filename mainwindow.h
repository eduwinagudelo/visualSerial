#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// standar includes
#include <algorithm>
#include <functional>
#include <string>
#include <vector>

// QT includes
#include <QMainWindow>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>

// Custom includes
#include "lcdcomm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnAbrirPuerto_clicked();
    void leerModulo();

    void on_btnBorrar_clicked();

    void on_btnEnviar_clicked();

    void on_btnCambiarL_clicked();

    void on_btnEnviarNum_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    int linea;
    std::vector<std::string> portList;
    LCDComm* lcd;

    void openSerialPort();
    void enviarModulo(unsigned char val);
    void closeSerialPort();
};

#endif // MAINWINDOW_H
