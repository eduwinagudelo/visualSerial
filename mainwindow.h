#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

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

    void openSerialPort();
    void enviarModulo(unsigned char comando, unsigned char val);
    void closeSerialPort();
};

#endif // MAINWINDOW_H
