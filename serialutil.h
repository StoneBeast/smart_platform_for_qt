#ifndef SERIALUTIL_H
#define SERIALUTIL_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class SerialUtil
{
private:
    QSerialPort *sp;    //serial port pointer

public:
    QList<QSerialPortInfo> scanSerial();    //scan serial ports
    void initBaudItem();  //combobox items for baudrate options
    void initByteItem();  //combobox items for bytewidth options
    void initParityItem();//combobox items for parity options
    void initStopItem();  //combobox items for stopbits options

private slots:
    void sendPB_slot();   //send button slot func
    void openPB_slot();   //open button slot func
    void readSB_slot();   //serial receive slot func
    void orderled1on();
    void orderled1off();
    void orderled2on();
    void orderled2off();
    void orderledtoggle();
    void orderdht11();
public:
    SerialUtil();
};

#endif // SERIALUTIL_H
