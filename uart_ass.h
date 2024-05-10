#ifndef UART_ASS_H
#define UART_ASS_H

#include <QMainWindow>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include <stdlib.h>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class uart_ass; }
QT_END_NAMESPACE

class uart_ass : public QMainWindow
{
    Q_OBJECT

public:



    uart_ass(QWidget *parent = nullptr);

    void uart_init(void);//界面初始化

    void start_uart(void);//按键事件绑定
    void uart_con(void); // 串口连接
    void timerEvent(QTimerEvent *e);//定时器事件
    ~uart_ass();


private:
    Ui::uart_ass *ui;
    QSerialPort *serialport;
    QVector<QString> ports;
};
#endif // UART_ASS_H
