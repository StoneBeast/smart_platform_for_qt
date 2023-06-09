#ifndef SERIALUTIL_H
#define SERIALUTIL_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class SerialUtil : public QSerialPort
{
private:
    void initSerial(QString portName);
    //QSerialPort *sp;    //serial port pointer

public: 
    void initBaudItem();  //combobox items for baudrate options
    void initByteItem();  //combobox items for bytewidth options
    void initParityItem();//combobox items for parity options
    void initStopItem();  //combobox items for stopbits options
    void sendPB_slot();   //send button slot func
    void openPB_slot();   //open button slot func
    void readSB_slot();   //serial receive slot func
    void orderled1on();
    void orderled1off();
    void orderled2on();
    void orderled2off();
    void orderledtoggle();
    void orderdht11();

    QList<QSerialPortInfo> scanSerial();    //scan serial ports
    bool openSerial(QString portName);

public:
    SerialUtil(QObject *parent = nullptr);
};

#endif // SERIALUTIL_H
