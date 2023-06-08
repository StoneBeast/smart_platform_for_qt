#include "serialutil.h"


SerialUtil::SerialUtil()
{

}

QList<QSerialPortInfo> SerialUtil::scanSerial() {
//    扫描串口
    return QSerialPortInfo::availablePorts();
//    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
//        cb[0]->addItem(info.portName());
//    }
}

void SerialUtil::initBaudItem() {
//  初始化波特率
//    QList<QString> list;
//    list<<"1200"<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<
//        "115200"<<"230400"<<"460800"<<"921600";
//    for(int i=0; i<11; i++) {
//        cb[1]->addItem(list[i]);
//    }
//    cb[1]->setCurrentIndex(7);
}

void SerialUtil::initByteItem() {
//  设置数据位
//    QList<QString> list;
//    list<<"5"<<"6"<<"7"<<"8";
//    for(int i=0; i<4; i++) {
//        cb[2]->addItem(list[i]);
//    }
//    cb[2]->setCurrentIndex(3);
}

void SerialUtil::initParityItem() {
//  设置校验位
//    QList<QString> list;
//    list<<"None"<<"Even"<<"Odd"<<"Space"<<"Mark";
//    for(int i=0; i<5; i++) {
//        cb[3]->addItem(list[i]);
//    }
//    cb[3]->setCurrentIndex(0);
}

void SerialUtil::initStopItem() {
//  设置停止位
//    QList<QString> list;
//    list<<"1"<<"2";
//    for(int i=0; i<2; i++) {
//        cb[4]->addItem(list[i]);
//    }
//    cb[4]->setCurrentIndex(0);
}

void SerialUtil::sendPB_slot() {
//
//    QByteArray data = te->toPlainText().toUtf8();
//    sp->write(data);
}

void SerialUtil::openPB_slot() {
//  开启串口槽函数
//    if(pb[1]->text() == "Open") {
//        //get port & baudrate
//        sp->setPortName(cb[0]->currentText());
//        sp->setBaudRate(cb[1]->currentText().toInt());
//        //get byte width option
//        switch(cb[2]->currentText().toInt()) {
//        case 5:
//            sp->setDataBits(QSerialPort::Data5);
//            break;
//        case 6:
//            sp->setDataBits(QSerialPort::Data6);
//            break;
//        case 7:
//            sp->setDataBits(QSerialPort::Data7);
//            break;
//        case 8:
//            sp->setDataBits(QSerialPort::Data8);
//            break;
//        default:
//            break;
//        }
//        //get parity option
//        switch(cb[3]->currentIndex()) {
//        case 0:
//            sp->setParity(QSerialPort::NoParity);
//            break;
//        case 1:
//            sp->setParity(QSerialPort::EvenParity);
//            break;
//        case 2:
//            sp->setParity(QSerialPort::OddParity);
//            break;
//        case 3:
//            sp->setParity(QSerialPort::SpaceParity);
//            break;
//        case 4:
//            sp->setParity(QSerialPort::MarkParity);
//            break;
//        default:
//            break;
//        }
//        //get stop bits option
//        switch(cb[2]->currentText().toInt()) {
//        case 1:
//            sp->setStopBits(QSerialPort::OneStop);
//            break;
//        case 2:
//            sp->setStopBits(QSerialPort::TwoStop);
//            break;
//        default:
//            break;
//        }
//        //no flow control always
//        sp->setFlowControl(QSerialPort::NoFlowControl);
//        if(!sp->open(QIODevice::ReadWrite)) {
//            QMessageBox::about(NULL, "Error", "Can't Open SerialPort");
//        } else {
//            for(int i=0; i<5; i++) {
//                cb[i]->setEnabled(false);
//            }
//            for(int i=0; i<6; i++) {
//                ord[i]->setEnabled(true);
//            }
//            pb[1]->setText("Close");
//            pb[0]->setEnabled(true);
//        }
//    } else {
//        sp->close();
//        for(int i=0; i<5; i++) {
//            cb[i]->setEnabled(true);
//        }
//        for(int i=0; i<6; i++) {
//            ord[i]->setEnabled(false);
//        }
//        pb[1]->setText("Open");
//        pb[0]->setEnabled(false);
//    }
}

void SerialUtil::readSB_slot() {
//  接受数据槽函数
//    QByteArray buf = sp->readAll();
//    tb->insertPlainText(QString(buf));
}

//void SerialUtil::orderled1on() {
//    sp->write("led1on\r\n");
//}
//void SerialUtil::orderled1off() {
//    sp->write("led1off\r\n");
//}
//void SerialUtil::orderled2on() {
//    sp->write("led2on\r\n");
//}
//void SerialUtil::orderled2off() {
//    sp->write("led2off\r\n");
//}
//void SerialUtil::orderledtoggle() {
//    sp->write("ledtoggle\r\n");
//}
//void SerialUtil::orderdht11() {
//    sp->write("dht11\r\n");
//}
