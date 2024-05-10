#include "uart_ass.h"
#include "ui_uart_ass.h"

uart_ass::uart_ass(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::uart_ass)
{

    ui->setupUi(this);

    startTimer(1000);
    uart_init();
    start_uart();

}

void uart_ass::uart_init(){ //初始化界面

    ui->baud_rate->addItem("115200");
    ui->baud_rate->addItem("9600");
    ui->baud_rate->addItem("4800");
    ui->baud_rate->addItem("1200"); //添加波特率选项

    ui->data_bit->addItem("8");
//    ui->data_bit->addItem("9");  //添加数据位

    ui->stop_bit->addItem("1");
    ui->stop_bit->addItem("1.5");
    ui->stop_bit->addItem("2"); //添加停止位

    ui->check_bit->addItem("奇校验");
    ui->check_bit->addItem("偶校验");
    ui->check_bit->addItem("无校验"); //添加校验方式

    ui->send_mod->addItem("文本");
    ui->send_mod->addItem("HEX");

    ui->rcv_mod->addItem("文本");
    ui->rcv_mod->addItem("HEX"); //添加接收发送方式


    ui->rcv_area->setReadOnly(true);
    ui->rcv_area->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->rcv_area->viewport()->setCursor(Qt::ArrowCursor);//接收区只读

}
void uart_ass::start_uart(){
    ui->send->setDisabled(true);
    ui->stop->setDisabled(true);
    connect(ui->stop,&QPushButton::clicked,this,[&](){
        ui->send->setDisabled(true);
        ui->stop->setDisabled(true);
        serialport->close();

    }); //断开按键事件

    connect(ui->connect,&QPushButton::clicked,this,[&](){
        if(ui->port_num->currentText()!="") uart_con();
    }); //连接按键事件

    connect(ui->send,&QPushButton::clicked,this,[&](){
        QString data = ui->send_area->toPlainText();
        if(ui->send_mod->currentText() == "HEX"){
            QByteArray arr;
            for(int i=0;i<data.size();i++){
                if(data[i]==' ') continue;
                int num = data.mid(i,2).toUInt(nullptr,16);
                ++i;
                arr.append(num);
            }
            serialport->write(arr);

        }else{
            serialport->write(data.toLocal8Bit().data());
        }

    }); //发送案件事件

    connect(ui->clear_rcv_area,&QPushButton::clicked,this,[&](){
        ui->rcv_area->clear();
    }); //清除接收区

    connect(ui->clear_send_area,&QPushButton::clicked,this,[&](){
        ui->send_area->clear();
    });

}
void uart_ass:: timerEvent(QTimerEvent *e){
    QVector<QString> temp;
    for(const QSerialPortInfo& info : QSerialPortInfo::availablePorts()){
        temp.push_back(info.portName());
    }

    std::sort(temp.begin(),temp.end());
    if(temp!= ports){
        ui->port_num->clear();
        ports = temp;
        for(auto& a :ports) ui->port_num->addItem(a);
    }


}
void uart_ass::uart_con(){
    QSerialPort ::BaudRate Baud;
    QSerialPort ::DataBits databits;
    QSerialPort::StopBits stopbits;
    QSerialPort::Parity check;

    QString port = ui->port_num->currentText();
    QString baud = ui->baud_rate->currentText();
    QString datas = ui->data_bit->currentText();
    QString ch = ui->check_bit->currentText();
    QString stopb = ui->stop_bit->currentText();

    if(baud =="1200") Baud = QSerialPort::Baud1200;
    else if(baud =="4800") Baud = QSerialPort::Baud4800;
    else if(baud =="9600") Baud = QSerialPort::Baud9600;
    else if(baud =="115200") Baud = QSerialPort::Baud115200; //设置波特率

    if(datas == "8") databits = QSerialPort::Data8;  //数据位

    if(stopb == "1") stopbits = QSerialPort::OneStop;
    else if(stopb =="1.5") stopbits = QSerialPort::OneAndHalfStop;
    else if(stopb == "2") stopbits = QSerialPort::TwoStop;  //停止位

    if(ch == "无校验")check = QSerialPort::NoParity;
    else if(ch=="偶校验")check = QSerialPort::EvenParity;
    else if(ch=="奇校验")check = QSerialPort::OddParity; //校验方式

    serialport = new QSerialPort();
    serialport->setPortName(port);
    serialport->setBaudRate(Baud);
    serialport->setDataBits(databits);
    serialport->setParity(check);
    serialport->setStopBits(stopbits);

    if(serialport->open(QSerialPort::ReadWrite)){
        ui->send->setDisabled(false);
        ui->stop->setDisabled(false);
        connect(serialport,&QSerialPort::readyRead,this,[&](){
            auto data = serialport->readAll();
            if(ui->rcv_mod->currentText()=="HEX"){
                QString hex = data.toHex(' ');
                ui->rcv_area->append(hex);
            }else{
                QString str = QString(data);
                ui->rcv_area->append(str);
            }
        });
    }else{
        QMessageBox::critical(this,"串口打开失败","请检查串口连接");
    }



}
uart_ass::~uart_ass()
{
    delete ui;
}

