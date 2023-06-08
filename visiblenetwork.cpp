#include "visiblenetwork.h"

VisibleNetwork::VisibleNetwork(QWidget *parent)
    : QWidget{parent}
{
    this->resize(800, 480);
    this->setMinimumSize(800, 480);

    //  创建串口对象
    serial = new SerialUtil();

    //  设置主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    //  设置网络列表组件
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    networkList = new QWidget(this);
    QVBoxLayout *networkListLayout = new QVBoxLayout(networkList);
    networkListLayout->setAlignment(Qt::AlignTop);
    networkList->setLayout(networkListLayout);
    networkList->setStyleSheet("QWidget {background:rgb(251, 251, 251);}");
    scrollArea->setWidget(networkList);
    mainLayout->addWidget(scrollArea, 5);

    //  设置右侧区域布局
    QVBoxLayout *rightBox = new QVBoxLayout(this);

    //  添加网络信息组件
    netWorkInfo = new QLabel(this);
    netWorkInfo->setStyleSheet("QLabel{background:rgb(255, 255, 255); }");
    QVBoxLayout *infoLayout = new QVBoxLayout(this);
    netWorkInfo->setLayout(infoLayout);
    infoLayout->addWidget(new QLabel("网络名称:"));
    infoLayout->addWidget(new QLabel("IP地址:"));
    infoLayout->addWidget(new QLabel("信号强度:"));
    infoLayout->addWidget(new QLabel("加密格式:"));
    infoLayout->addWidget(new QLabel("是否连接互联网:"));
    rightBox->addWidget(netWorkInfo);

    //  下方控制区域使用垂直布局
    QVBoxLayout *ctrlArea = new QVBoxLayout(this);
    QHBoxLayout *selectArea = new QHBoxLayout(this);
    QHBoxLayout *btnArea = new QHBoxLayout(this);


    //  添加刷新图标以及下拉框
    flushIcon = new QPushButton(this);
    flushIcon->setIcon(QIcon(":/icons/flushIcon.png"));
    flushIcon->setStyleSheet("QPushButton{background: rgba(240, 240, 240, 0); border: 0;}");
    selectArea->addWidget(flushIcon, 1);
    iconAnimation = new QPropertyAnimation(flushIcon, "rotation");
    iconAnimation->setStartValue(0);
    //不起作用
    connect(flushIcon, SIGNAL(clicked()), this, SLOT(flushSerialSlot()));

    serial_cb = new QComboBox();
    initSerial_cb(serial_cb);
    selectArea->addWidget(serial_cb, 8);
    selectArea->setSpacing(5);

    //  添加检查设备连接按钮
    checkButton = new QPushButton("检查设备连接", this);
    checkButton->setMinimumHeight(60);
    btnArea->addWidget(checkButton);

    //  添加刷新网络列表按钮
    flushButton = new QPushButton("刷新列表", this);
    flushButton->setMinimumHeight(60);
    btnArea->addWidget(flushButton);

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

    connect(checkButton, &QPushButton::clicked, this, &VisibleNetwork::checkConnectionSlot);

    this->setLayout(mainLayout);



    WifiItem *item1 = new WifiItem(0, "test", 0, -21, "ac:2d:dd:41:21:as", networkList);
    WifiItem *item2 = new WifiItem(1, "hello", 3, -56, "ac:2d:dd:41:21:ad", networkList);
    WifiItem *item3 = new WifiItem(2, "world", 4, -70, "ac:2d:dd:4c:21:as", networkList);
    WifiItem *item4 = new WifiItem(3, "azur", 3, -10, "ac:2d:d3:41:21:as", networkList);
    WifiItem *item5 = new WifiItem(4, "lane", 0, -45, "ac:22:dd:41:21:as", networkList);
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

    connect(item1, SIGNAL(clicked(int)), this, SLOT(handle(int)));
    connect(item2, SIGNAL(clicked(int)), this, SLOT(handle(int)));
    connect(item3, SIGNAL(clicked(int)), this, SLOT(handle(int)));
    connect(item4, SIGNAL(clicked(int)), this, SLOT(handle(int)));
    connect(item5, SIGNAL(clicked(int)), this, SLOT(handle(int)));
}

void VisibleNetwork::initSerial_cb(QComboBox *cb) {
    foreach (const QSerialPortInfo &info, serial->scanSerial()) {
        cb->addItem(info.portName());
    }
}

void VisibleNetwork::flushSerialSlot() {
    qDebug() << "clicked!";
    rollIcon(360, 300);
}

void VisibleNetwork::checkConnectionSlot() {
    qDebug() << "clicked!";
}

void VisibleNetwork::rollIcon(int angle, int duration) {
    iconAnimation->setEndValue(angle);
    iconAnimation->setDuration(duration);
    iconAnimation->start();
}

void VisibleNetwork::handle(int index) {
    qDebug() << "open!!!!! row:139" << index;
    for(int i = 0; i<5; i++) {
        if (i == index) {
            item[i]->setExpansion();
        } else {
            item[i]->setFold();
        }
    }
}
