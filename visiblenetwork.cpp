#include "visiblenetwork.h"

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
    netWorkInfo = new QLabel(this);
    infoLayout = new QVBoxLayout(this);
    netWorkInfo->setLayout(infoLayout);
    infoLayout->addWidget(new QLabel("网络名称:"));
    infoLayout->addWidget(new QLabel("IP地址:"));
    infoLayout->addWidget(new QLabel("信号强度:"));
    infoLayout->addWidget(new QLabel("加密格式:"));
    infoLayout->addWidget(new QLabel("是否连接互联网:"));
    netWorkInfo->setObjectName("network-info");
    netWorkInfo->setStyleSheet("QLabel#network-info{background:rgb(255, 255, 255); "
                               "border: 1px #e7e7f1 solid;"
                               "border-radius: 8px;"
                               "}");
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


//  debug
    WifiItem *item1 = new WifiItem(0, *(new WifiObj("test", 0, -21, "ac:2d:dd:41:21:as")), networkList);
    WifiItem *item2 = new WifiItem(1, *(new WifiObj("hello", 3, -56, "ac:2d:dd:41:21:ad")), networkList);
    WifiItem *item3 = new WifiItem(2, *(new WifiObj("world", 4, -70, "ac:2d:dd:4c:21:as")), networkList);
    WifiItem *item4 = new WifiItem(3, *(new WifiObj("azur", 3, -10, "ac:2d:d3:41:21:as")), networkList);
    WifiItem *item5 = new WifiItem(4, *(new WifiObj("lane", 0, -45, "ac:22:dd:41:21:as")), networkList);
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


    connect(item1, SIGNAL(clickConnect(int)), this, SLOT(connectSlot(int)));
    connect(item2, SIGNAL(clickConnect(int)), this, SLOT(connectSlot(int)));
    connect(item3, SIGNAL(clickConnect(int)), this, SLOT(connectSlot(int)));
    connect(item4, SIGNAL(clickConnect(int)), this, SLOT(connectSlot(int)));
    connect(item5, SIGNAL(clickConnect(int)), this, SLOT(connectSlot(int)));
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
    qDebug() << "open!!!!! row:139" << index;
    for(int i = 0; i<5; i++) {
        if (i == index) {
            item[i]->setExpansion();
        } else {
            item[i]->setFold();
        }
    }
}

void VisibleNetwork::flushSerialSlot() {
    qDebug() << "flush serial list  row:159";

//    do flush serial list
    initSerial_cb(serial_cb);
//  尝试添加旋转动画
}

void VisibleNetwork::connectSlot(int index) {
    qDebug() << "connect!!!!! row:168" << index;

    //  do connection...
    /*
     * 基本实现思路：通过串口发送约定好的数据，连接
     * 添加连接成功信号，在对应的槽函数中填写 network into中的相关信息
     * 同时wifiitem上的连接按钮变为断开连接
     */
}

void VisibleNetwork::checkConnectionSlot() {
    qDebug() << "check device connection row: 170!";

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

    //  设置定时器
    connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    timer->start(500);

    //  询问设备是否被正确连接
    serial->write("visibleNetwork|0");

}

void VisibleNetwork::flushNetworkSlot() {
    qDebug() << "flush list row: 176";

    //  do flush list and instand of the old
    /*
     *  刷新network list，但是这个函数只有在设备已经正确连接之后才可以被调用
     *  如果设备没有正确连接，点击这个按钮则弹出相关提示。
     */
}

void VisibleNetwork::parseResult(QString result) {
    /*
     *  request format:
     *          visibleNetwork|<codeNum>
     *
     *  response format:
     *          visibleNetwork|<codeNum>|<status>|body
     *
     * codeNum:
     *          0: is device connect
     *          1: get network list
     *          2: get device network status
     * status:
     *          OK
     *          ERROR
     */
    QStringList resultList = result.split('|');
    if (resultList[0] == "visibleNetwork") {
        switch(resultList[1].toInt()) {
        case 0:
            //  check device connect
            handleCheckDevice(resultList);
            break;
        case 1:
            handleGetNetworkList(resultList);
            //  get network list
            break;
        case 2:
            handleGetNetworkStatus(resultList);
            //  get device network status
            break;
        default :
            // error
            break;
        }
    }
}

// must add timeout signal
void VisibleNetwork::readyReadSlot() {
    //  接收到信号九停止定时器，防止发出超时信号
    timer->stop();
    QByteArray buf = serial->readAll();
    parseResult(QString(buf));
}

void VisibleNetwork::handleCheckDevice(QStringList s) {
    if (s[2] == "OK") {
        //  设备正常连接，开始获取网络列表

        connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
        //  扫描wifi时间较长
        timer->start(5000);
        serial->write("visibleNetwork|1");
    }
}
#if 1

void VisibleNetwork::handleGetNetworkList(QStringList s) {
    if (s[2] == "OK") {
        /*
         * body format:
         * "{
         *    "networkList":[
         *                      {
         *                          "ssid": "asdasd",
         *                          "ecn" : "-12",
         *                          "rssi": "3",
         *                          "mac" : "ca:ac:24:c5";
         *                      },
         *                      {},....
         *                  ]
         * }"
         *
         */
        QJsonParseError err_rpt;
        QJsonDocument  root_Doc = QJsonDocument::fromJson(s[3].toLatin1(), &err_rpt); // 字符串格式化为JSON

        if(err_rpt.error != QJsonParseError::NoError) {
            qDebug() << "JSON格式错误";
        }

        else {
            //qDebug() << "JSON格式正确：\n" << root_Doc;

            QJsonObject root_Obj = root_Doc.object();
            QJsonValue network_list = root_Obj.value("networkList");
            if(network_list.isArray()) // 可省略
            {
                this->wifiArray = network_list.toArray();
                int count = wifiArray.count();

                //  首先清空布局
                while(networkListLayout->count())
                {
                    wifiItemList.clear();
                    QWidget *p=this->networkListLayout->itemAt(0)->widget();
                    p->setParent (NULL);
                    this->networkListLayout->removeWidget(p);
                    delete p;
                }

                //  添加布局
                for (int i=0; i<count; i++) {
                    QJsonObject networkObj = wifiArray.at(i).toObject();

                    wifiItemList.append(*(new WifiObj(networkObj.value("ssid").toString(),
                                                      networkObj.value("ecn").toInt(),
                                                      networkObj.value("ssid").toInt(),
                                                      networkObj.value("mac").toString()
                                                      )));

                    WifiItem *temp = new WifiItem(i, (wifiItemList.at(i)));

                    networkListLayout->addWidget(temp);
                    connect(temp, SIGNAL(clicked(int)), this, SLOT(expansionSlot(int)));
                    connect(temp, SIGNAL(clickConnect(int)), this, SLOT(connectSlot(int)));
                }
            }
        }

        // 获取成功，开始获取网络状态
        connect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
        timer->start(3000);
        serial->write("visibleNetwork|2");
    }
}

#endif
void VisibleNetwork::handleGetNetworkStatus(QStringList s) {

    /*
     *
     * body format:
     *  "{
     *      "data" : {
     *          "isConnection": "true",
     *          "ip"   : "192.168.1.1",
     *          "mac"  :  "ad:22:42:54:ad"
     *          "isInternet": "false"
     *      }
     *  }"
     */
    if (s[2] == "OK") {
        QJsonParseError err_rpt;
        QJsonDocument  root_Doc = QJsonDocument::fromJson(s[3].toLatin1(), &err_rpt); // 字符串格式化为JSON
        QJsonObject root_Obj = root_Doc.object();
        QJsonValue data = root_Obj.value("data");
        QJsonObject dataObj = data.toObject();
        if (dataObj.value("isConnection").toString() == "true") {

            int i = 0;
            foreach (WifiObj obj, wifiItemList) {
                if (obj.mac() == dataObj.value("mac").toString()) {
                    ((QLabel)infoLayout->itemAt(0)->widget()).setText(QString("网络名称:\t%1").arg(obj.ssid()));
                    ((QLabel)infoLayout->itemAt(1)->widget()).setText(QString("IP地址:\t%1").arg(obj.ip()));
                    ((QLabel)infoLayout->itemAt(2)->widget()).setText(QString("信号强度:\t%1").arg(obj.ecn()));
                    ((QLabel)infoLayout->itemAt(3)->widget()).setText(QString("加密格式:\t%1").arg(obj.rssi()));
                    ((QLabel)infoLayout->itemAt(4)->widget()).setText(QString("是否连接互联网:\t%1").arg(dataObj.value("isInternet").toString()));

                    //  修改网络列表，将已连接的网络的按钮改为断开连接
                    ((WifiItem)(networkListLayout->itemAt(i)->widget())).setButtonText("断开连接");
                }
                i++;
            }
        }

    }
}

void VisibleNetwork::timeoutSlot() {
    //  超时，为收到回复，请检查设备连接
    disconnect(serial, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
}
