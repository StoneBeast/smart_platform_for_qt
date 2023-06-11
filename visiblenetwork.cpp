#include "visiblenetwork.h"

/*TODO
 *  如果出现不能打开串口相关的问题，需要再调用一次扫描串口函数
 *  下一步需要做出断开连接的操作
 *  连接的操作，注意连接时要判断是不是已经连接了网络，如果是要先执行相应的操作再执行连接操作
 *  输入框，调用虚拟键盘
 *  过场加载动画
 */

VisibleNetwork::VisibleNetwork(QWidget *parent)
    : QWidget{parent}
{
    this->resize(800, 480);
    this->setMinimumSize(800, 480);
    this->setObjectName("main-page");
    this->setStyleSheet("QWidget#main-page{background-color: #f3f3f3;}");

    //  创建定时器对象，主要用于数据接收定时
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));

    //  创建串口对象
    serial = new SerialUtil();

    //  设置主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    //  设置网络列表组件
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    networkList = new QWidget(this);
    networkList->setObjectName("network-list");
    networkListLayout = new QVBoxLayout(networkList);
    networkListLayout->setAlignment(Qt::AlignTop);
    networkList->setLayout(networkListLayout);
    networkList->setStyleSheet("QWidget#network-list {background:rgb(251, 251, 251);}");
    scrollArea->setWidget(networkList);
    mainLayout->addWidget(scrollArea, 5);

    //  设置右侧区域布局
    QVBoxLayout *rightBox = new QVBoxLayout(this);

    //  添加网络信息组件
    netWorkInfo = new QLabel();

    netWorkInfo->setObjectName("network-info");
    netWorkInfo->setStyleSheet("QLabel#network-info{background:rgb(255, 255, 255); "
                               "border: 1px #e7e7f1 solid;"
                               "border-radius: 8px;"
                               "}");
    netWorkInfo->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    setNetworkConnectionFlag(false);
    rightBox->addWidget(netWorkInfo);

    //  下方控制区域使用垂直布局
    QVBoxLayout *ctrlArea = new QVBoxLayout(this);
    QHBoxLayout *selectArea = new QHBoxLayout(this);
    QHBoxLayout *btnArea = new QHBoxLayout(this);


    //  添加刷新图标以及下拉框
    flushLab = new ClickLabel(this);
    flushLab->setFixedSize(25, 25);
    QPixmap *wifiIcon = new QPixmap(":/icons/flushIcon.png");
    *wifiIcon = wifiIcon->scaled(flushLab->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    flushLab->setPixmap(*wifiIcon);
    flushLab->setMaximumSize(25, 25);


    selectArea->addWidget(flushLab, 1);

    connect(flushLab, SIGNAL(clicked()), this, SLOT(flushSerialSlot()));

    serial_cb = new QComboBox();
    initSerial_cb(serial_cb);
    selectArea->addWidget(serial_cb, 8);
    selectArea->setSpacing(5);

    //  添加检查设备连接按钮
    checkButton = new QPushButton("检查设备连接", this);
    checkButton->setMinimumHeight(60);
    btnArea->addWidget(checkButton);
    connect(checkButton, SIGNAL(clicked()), this, SLOT(checkConnectionSlot()));

    //  添加刷新网络列表按钮
    flushButton = new QPushButton("刷新列表", this);
    flushButton->setMinimumHeight(60);
    btnArea->addWidget(flushButton);
    setDeviceConnectionFlag(false);
    connect(flushButton, SIGNAL(clicked()), this, SLOT(flushNetworkSlot()));

    //  将串口选择部分和按钮部分分别加入到ctrlArea中
    ctrlArea->addLayout(selectArea);
    ctrlArea->addLayout(btnArea);
    ctrlArea->setSpacing(30);

    rightBox->addLayout(ctrlArea);

    //  设置信息部分和按钮部分之间的间隔
    rightBox->setSpacing(100);


    mainLayout->addLayout(rightBox, 2);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    this->setLayout(mainLayout);


#if DEBUG == 1
#if 0
    WifiItem *item1 = new WifiItem(0, *(new WifiObj("test", 0, 21, "ac:2d:dd:41:21:as")), networkList);
    WifiItem *item2 = new WifiItem(1, *(new WifiObj("hello", 3, 56, "ac:2d:dd:41:21:ad")), networkList);
    WifiItem *item3 = new WifiItem(2, *(new WifiObj("world", 4, 70, "ac:2d:dd:4c:21:as")), networkList);
    WifiItem *item4 = new WifiItem(3, *(new WifiObj("azur", 3, 10, "ac:2d:d3:41:21:as")), networkList);
    WifiItem *item5 = new WifiItem(4, *(new WifiObj("lane", 0, 45, "ac:22:dd:41:21:as")), networkList);
    item[0] = item1;
    item[1] = item2;
    item[2] = item3;
    item[3] = item4;
    item[4] = item5;

    networkListLayout->addWidget(item1);
    networkListLayout->addWidget(item2);
    networkListLayout->addWidget(item3);
    networkListLayout->addWidget(item4);
    networkListLayout->addWidget(item5);

    connect(item1, SIGNAL(clicked(int)), this, SLOT(expansionSlot(int)));
    connect(item2, SIGNAL(clicked(int)), this, SLOT(expansionSlot(int)));
    connect(item3, SIGNAL(clicked(int)), this, SLOT(expansionSlot(int)));
    connect(item4, SIGNAL(clicked(int)), this, SLOT(expansionSlot(int)));
    connect(item5, SIGNAL(clicked(int)), this, SLOT(expansionSlot(int)));


    connect(item1, SIGNAL(clickConnect(QString, int)), this, SLOT(connectSlot(QString, int)));
    connect(item2, SIGNAL(clickConnect(QString, int)), this, SLOT(connectSlot(QString, int)));
    connect(item3, SIGNAL(clickConnect(QString, int)), this, SLOT(connectSlot(QString, int)));
    connect(item4, SIGNAL(clickConnect(QString, int)), this, SLOT(connectSlot(QString, int)));
    connect(item5, SIGNAL(clickConnect(QString, int)), this, SLOT(connectSlot(QString, int)));
#endif
#endif //DEBUG == 1
}

//  初始化/刷新串口列表
void VisibleNetwork::initSerial_cb(QComboBox *cb) {
    //  如果不是空，则先清除cb中的所有选项
    if (cb->count() != 0) {
        cb->clear();
    }

    //  将所有串口加入到cb中
    foreach (const QSerialPortInfo &info, serial->scanSerial()) {
        cb->addItem(info.portName());
    }

    //  没有可用串口时，使用 ‘空’占位，否则则删除
    if (cb->count() == 0) {
#ifdef Q_OS_WIN
        cb->setPlaceholderText("空");
#else
        cb->setCurrentText("choose");
#endif
    } else {
#ifdef Q_OS_WIN
        cb->setPlaceholderText(nullptr);
#else
        cb->setCurrentText(nullptr);
#endif
    }
}


void VisibleNetwork::expansionSlot(int index) {
#if DEBUG == 1
    qDebug() <<__FILE__ << __LINE__ << "open!!!!! row:139" << index;
#endif //DEBUG == 1
    for(int i = 0; i < networkListLayout->count(); i++) {
        if (i == index) {
#if DEBUG == 1
            qDebug() <<__FILE__ << __LINE__ << "expansionSlot,i==index" << i;
#endif //DEBUG == 1
            // tempList[i]->setExpansion();
            tempList_QList.at(i)->setExpansion();
        } else {
#if DEBUG == 1
            qDebug() <<__FILE__ << __LINE__ << "expansionSlot,i!=index" << i;
#endif //DEBUG == 1
            // tempList[i]->setFold();
            tempList_QList.at(i)->setFold();
        }
    }
}

void VisibleNetwork::flushSerialSlot() {
#if DEBUG == 1
    qDebug() <<__FILE__ << __LINE__ << "flush serial list  row:159";
#endif //DEBUG == 1
    if (serial->isOpen()) {
        serial->close();
    }
//    do flush serial list
    initSerial_cb(serial_cb);
//  尝试添加旋转动画
}

void VisibleNetwork::connectSlot(QString text, int index) {
#if DEBUG == 1
    qDebug() <<__FILE__ << __LINE__  << text << " connect!!!!!" << index;
#endif //DEBUG == 1
    //  do connection...
    /*
     * 基本实现思路：通过串口发送约定好的数据，连接
     * 添加连接成功信号，在对应的槽函数中填写 network into中的相关信息
     * 同时wifiitem上的连接按钮变为断开连接
     *
     * 连接网络
     * command format:
     *      visibleNetwork|3|<ssid>&<password>
     *       eg:   visibleNetwork|3|stone&123456
     * response format:
     *      {visibleNetwork|3|<status>}
     *
     * 断开连接
     *  command format:
     *      {visibleNetwork|4}
     *
     *  response format:
     *      {visibleNetwork|4|OK}
     */
    if (text == "连接") {
        //  执行连接网络的操作，同时也要检擦当前有没有连接其他网络，如果有，则要先执行断开网络的操作
        QString password = "";
        bool confirmFlag = true;
        if (wifiObjList.at(index).ecn()) {
            //  有加密
            QString dlgTitle=QString("连接到%1").arg(wifiObjList.at(index).ssid());//对话框标题
            QString txtLabel=QStringLiteral("输入密码:");//对话框Label显示内容
            QLineEdit::EchoMode echoMode=QLineEdit::Normal;//输入框的文本内容为正常显示
            password=QInputDialog::getText(this,dlgTitle,txtLabel,echoMode);
#if DEBUG == 1
            qDebug() << __FILE__ << __LINE__ << "connecd with: " << password;
#endif
            if (password.isEmpty()) {
                confirmFlag = false;
            }
        }

        if (confirmFlag) {


            connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
#if DEBUG == 1
            timer->start(5000);
#else
            timer->start(500);
#endif
            setNetworkConnectionFlag(false);
            serial->write((QString("visibleNetwork|3|%1&%2").arg(wifiObjList.at(index).ssid(), password)).toUtf8());

        }

    } else {
        //  执行断开网络的操作
        if (serial->isOpen()) {
            connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
#if DEBUG == 1
            timer->start(5000);
#else
            timer->start(500);
#endif //DEBUG == 1

            serial->write("visibleNetwork|4");

        } else {
            //  串口没有被打开
            handleCanntOprnSerial();
        }

    }
}

void VisibleNetwork::checkConnectionSlot() {
#if DEBUG == 1
    qDebug()<<__FILE__ << __LINE__  << "check device connection row: 170!";
#endif //DEBUG == 1
    //  do check connection

    /*
     * 添加定时器，超时函数设置为解除readyread信号
     * 连接readyread信号
     *
     *
     *
     *
     * 基本实现思路：
     *  将检查设备是否正确连接以及设备是否连接网络两个功能合起来，
     *  如果按照一定格式返回，则是连接成功，如果返回的信息中指出设备已经连接
     *  则判定为设备已经连接网络
     *      然后向设备发送命令，获取网络环境信息，更新 network list
     *      然后再获取设备连接的网络的信息，然后修改wifiitem上的链接按钮
     */

    //  close serial if opend before open new one
    if (serial->isOpen()) {
        serial->close();
    }

    //  设置定时器
    if (serial->openSerial(this->serial_cb->currentText())) {
        connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
#if DEBUG == 1
        timer->start(5000);
#else
        timer->start(500);
#endif //DEBUG == 1

        //  询问设备是否被正确连接
        if (serial->isOpen()) {
            serial->write("visibleNetwork|0");
        }   else {
            //  串口没有被打开
            handleCanntOprnSerial();
        }

    } else {
        handleCanntOprnSerial();
    }


}

void VisibleNetwork::flushNetworkSlot() {
#if DEBUG == 1
    qDebug()<<__FILE__ << __LINE__  << "flush list";
#endif //DEBUG == 1
    //  do flush list and instand of the old
    /*
     *  刷新network list，但是这个函数只有在设备已经正确连接之后才可以被调用
     *  如果设备没有正确连接，点击这个按钮则弹出相关提示。
     */

    if (serial->isOpen()) {
        connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
#if DEBUG == 1
        timer->start(5000);
#else
        timer->start(500);
#endif //DEBUG == 1


        serial->write("visibleNetwork|1");

    } else {
        //  串口没有被打开
        handleCanntOprnSerial();
    }
}

void VisibleNetwork::parseResult(QString result) {
    /*
     *  request format:
     *          visibleNetwork|<codeNum>
     *
     *  response format:
     *          {visibleNetwork|<codeNum>|<status>|body}
     *
     * codeNum:
     *          0: is device connect
     *          1: get network list
     *          2: get device network status
     *          4: disconnect network
     * status:
     *          OK
     *          ERROR
     */
    QStringList resultList = result.split('|');
    if (resultList[0] == "visibleNetwork") {
        switch(resultList[1].toInt()) {
        case 0:
            //  check device connect
#if DEBUG == 1
            qDebug()<<__FILE__ << __LINE__  << "handleCheckDevice";
#endif //DEBUG == 1
            handleCheckDevice(resultList);
            break;
        case 1:
            //  get network list
#if DEBUG == 1
            qDebug()<<__FILE__ << __LINE__  << "handleGetNetworkList";
#endif //DEBUG == 1
            handleGetNetworkList(resultList);

            break;
        case 2:
            //  get device network status
#if DEBUG == 1
            qDebug()<<__FILE__ << __LINE__  << "handleGetNetworkStatus";
#endif //DEBUG == 1
            handleGetNetworkStatus(resultList);
            break;
        case 3:
            //  connect to network
#if DEBUG == 1
            qDebug()<<__FILE__ << __LINE__  << "handleGetNetworkStatus";
#endif //DEBUG == 1
            handleConnectNetwork(resultList);
            break;
        case 4:
            //  disconnecet network
            handleDisconnect(resultList);
        default :
            // error
            break;
        }
    }
}

void VisibleNetwork::handleCheckDevice(QStringList s) {
#if DEBUG == 1
    qDebug() << __FILE__ << __LINE__ << "check device";
#endif //DEBUG == 1

    if (s[2] == "OK") {
        //  设备正常连接，开始获取网络列表
        if (serial->isOpen()) {
            connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
            //  扫描wifi时间较长
            timer->start(5000);
            serial->write("visibleNetwork|1");
        }   else {
            handleCanntOprnSerial();
        }

    }
}
#if 1

void VisibleNetwork::handleGetNetworkList(QStringList s) {
    qDebug() << __FILE__ << __LINE__ << "get network list";
    if (s[2] == "OK") {
        /*
         * body format:
         *  ssid % ecn % rssi%     mac      &     ssid%ecn%rssi%mac&...
         * test01%-12%3% ca:ac:24:c5
         */

        QStringList body = s[3].split('&');
        int count = body.count();

#if DEBUG == 1
        qDebug() << __FILE__ << __LINE__ << "count: " << count;
#endif

        //  首先清空布局
        while(networkListLayout->count())
        {
#if DEBUG == 1
            qDebug() << __FILE__ << __LINE__ << "clear all WifiItem";
#endif
            wifiObjList.clear();
            tempList_QList.clear();
            WifiItem *p=static_cast<WifiItem*>(networkListLayout->itemAt(0)->widget());
            disconnect(p, SIGNAL(clicked(int)), this, SLOT(expansionSlot(int)));
            disconnect(p, SIGNAL(clickConnect(QString, int)), this, SLOT(connectSlot(QString, int)));
            p->setParent (nullptr);
            this->networkListLayout->removeWidget(p);
            delete p;
        }

        //  添加布局
#if DEBUG == 1
        qDebug() << __FILE__ << __LINE__ << "begin add widget";
#endif
        for (int i=0; i<count; i++) {
            QStringList item = body[i].split('%');

            wifiObjList.append(*(new WifiObj(item[SSID],
                                              item[ECN].toInt(),
                                              item[RSSI].toInt(),
                                              item[MAC]
                                              )));

            tempList_QList.append(new WifiItem(i, (wifiObjList.at(i)), networkList));

            networkListLayout->addWidget(tempList_QList.at(i));
            connect(tempList_QList.at(i), SIGNAL(clicked(int)), this, SLOT(expansionSlot(int)));
            connect(tempList_QList.at(i), SIGNAL(clickConnect(QString, int)), this, SLOT(connectSlot(QString, int)));

        }

#if DEBUG == 1
        qDebug() << __FILE__ << __LINE__ << "end of add all widget";
#endif
        if (serial->isOpen()) {
            // 获取成功，开始获取网络状态
            connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
            timer->start(3000);
#if DEBUG == 1
            qDebug() << __FILE__ << __LINE__ << "timer start";
#endif

            serial->write("visibleNetwork|2");
        }   else {
            //  串口没有被打开
            handleCanntOprnSerial();
        }
    }
}

#endif
void VisibleNetwork::handleGetNetworkStatus(QStringList s) {

    /*
     * data format:
     *  isConnection%ip%mac%isInternet
     *          true%192.168.1.1%ad:22:42:54:ad%false
     *
     *  {true%192.168.1.1%ac:bb:3a:44%false}
     */

#if DEBUG == 1
    qDebug() << __FILE__ << __LINE__ << "get network status";
#endif //DEBUG == 1
    if (s[2] == "OK") {
        QStringList temp_networkInfo = s[3].split('%');
        if (temp_networkInfo[IS_CONNECTION] == "true") {

            setNetworkConnectionFlag(true);

#if DEBUG == 1
            qDebug() << __FILE__ << __LINE__ << "begin change QLabel";
#endif //DEBUG == 1

            int i = 0;
            foreach (WifiObj obj, wifiObjList) {
                if (obj.mac() == temp_networkInfo[INFO_MAC]) {
                    //  信号强度、加密格式、是否访问互联网
                    QString isInternetLab = (temp_networkInfo[IS_INTERNTER] == "false" ? "无法访问":"可以访问");

                    this->findChild<QLabel *>("network-info")->setText(QString("<table style=\"font-size:16px\" align=\"center\" cellspacing=\"18\">"
                                                                       "<tr > "
                                                                            "<td >网络名称</td>"
                                                                            "<td style=\"color: #646464\">%1</td>"
                                                                       "</tr>"
                                                                       "<tr> "
                                                                            "<td>IP地址</td>"
                                                                            "<td style=\"color: #646464\">%2</td>"
                                                                       "</tr>"
                                                                       "<tr> "
                                                                            "<td>信号强度</td>"
                                                                            "<td style=\"color: #646464\">%3</td>"
                                                                       "</tr>"
                                                                       "<tr> "
                                                                            "<td>加密格式</td>"
                                                                            "<td style=\"color: #646464\">%4</td>"
                                                                       "</tr>"
                                                                       "<tr> "
                                                                            "<td>访问互联网</td>"
                                                                            "<td style=\"color: #646464\">%5</td>"
                                                                       "</tr>"
                                                                               "</table>").arg(obj.ssid(), temp_networkInfo[IP], WifiItem::getRssiStr(obj.rssi()), WifiItem::getEcn(obj.ecn()), isInternetLab));


                    tempList_QList.at(i)->setButtonText("断开连接");
                }
                i++;
            }
#if DEBUG == 1
            qDebug() << __FILE__ << __LINE__ << "end change QLabel";
#endif //DEBUG == 1
        }
        else {
            //  设备没有链接网络
            setNetworkConnectionFlag(false);

        }
        this->setDeviceConnectionFlag(true);
    }
}

void VisibleNetwork::handleConnectNetwork(QStringList s) {
    if (s[2] == "OK") {
        if (serial->isOpen()) {
            connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
            //  扫描wifi时间较长
            timer->start(5000);
            serial->write("visibleNetwork|2");
        }   else {
            handleCanntOprnSerial();
        }
    }
}

void VisibleNetwork::handleDisconnect(QStringList s) {
    if (s[2] == "OK") {
        if (serial->isOpen()) {
            connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
            //  扫描wifi时间较长
            timer->start(5000);
            serial->write("visibleNetwork|2");
        }   else {
            handleCanntOprnSerial();
        }
    }
}

// must add timeout signal
void VisibleNetwork::readyReadSlot() {
#if DEBUG == 1
    qDebug() << __FILE__ << __LINE__ << "ready read";
#endif //DEBUG == 1

    //  接收到信号停止定时器，防止发出超时信号
    timer->stop();

    serialBuf += serial->readAll();

    if (serialBuf.startsWith('{') && serialBuf.endsWith('}')) {
#if DEBUG == 1
        qDebug() << __FILE__ << __LINE__ << "isEnd?" << serial->atEnd();
#endif //DEBUG == 1
        disconnect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));

#if DEBUG == 1
        qDebug() << __FILE__ << __LINE__ << "\n" <<QString(serialBuf) << "\n";
#endif //DEBUG == 1

        serialBuf.remove(0, 1);
        serialBuf.remove(serialBuf.length() - 1, 1);

        parseResult(QString(serialBuf));

        //  清空buf
        serialBuf.clear();

    }
}

void VisibleNetwork::timeoutSlot() {
    //  超时，为收到回复，请检查设备连接
    qDebug() << __FILE__ << __LINE__ << "time out";
    disconnect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    timer->stop();

    QMessageBox::about(NULL, "Error", "返回超时，请检查设备连接");
    setDeviceConnectionFlag(false);
    setNetworkConnectionFlag(false);
    //  首先清空布局
    while(networkListLayout->count())
    {
#if DEBUG == 1
        qDebug() << __FILE__ << __LINE__ << "clear all WifiItem";
#endif
        wifiObjList.clear();
        tempList_QList.clear();
        WifiItem *p=static_cast<WifiItem*>(networkListLayout->itemAt(0)->widget());
        disconnect(p, SIGNAL(clicked(int)), this, SLOT(expansionSlot(int)));
        disconnect(p, SIGNAL(clickConnect(QString, int)), this, SLOT(connectSlot(QString, int)));
        p->setParent (nullptr);
        this->networkListLayout->removeWidget(p);
        delete p;
    }

}

void VisibleNetwork::setDeviceConnectionFlag(bool flag) {
    flushButton->setEnabled(flag);
    m_deviceConnectionFlag = flag;
}

void VisibleNetwork::setNetworkConnectionFlag(bool flag) {
    if(!flag) {
        netWorkInfo->setText("<span style=\"color:#617bac; font-size: 20px; font-weight: bold;\">"
                             "暂无网络信息</span>");
        foreach (WifiItem *item, tempList_QList) {
            if (item->getButtonText() == "断开连接") {
                item->setButtonText("连接");
            }
        }
    }
    m_networkConnectionFlag = flag;
}

void VisibleNetwork::handleCanntOprnSerial() {
    //  解除信号连接
    disconnect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot));

    //  停止定时器，防止超时
    timer->stop();

    //  弹出提示
    QMessageBox::about(NULL, "Error", "无法打开串口");

    //  重新扫描串口
    serial->scanSerial();
}
