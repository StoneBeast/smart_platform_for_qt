#include "wifiitem.h"

WifiItem::WifiItem(QWidget *parent)
    : QWidget{parent}
{

}

//WifiItem::WifiItem(int index, QString ssid, int ecn, int rssi, QString mac, QWidget *parent)
//    : QWidget{parent}, index(index), ssid(ssid), ecn(ecn), rssi(rssi),
//      mac(mac)
WifiItem::WifiItem(int index, WifiObj obj, QWidget *parent)
    : QWidget{parent}, index(index), ssid(obj.ssid()),
      ecn(obj.ecn()), rssi(obj.rssi()), mac(obj.mac())
{   
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setObjectName("wifi-item");
    this->setStyleSheet("QWidget#wifi-item{border :2px solid #d3d3e6;}"
                        "QWidget{background-color:rgb(242, 242, 242);"
                        "border-radius:8px;}"
                        "QWidget:hover{"
                        "background-color: #f5f5f5;"
                        "}"
                        );

    QVBoxLayout *main_layout = new QVBoxLayout(this);     //  main layout
    QHBoxLayout *title_layout = new QHBoxLayout(this);    //  title layout
    moreInfo_widget = new QWidget(this);
    QVBoxLayout *moreInfo_layout = new QVBoxLayout(this); //  will be hidden before cliecked
    QHBoxLayout *status_layout = new QHBoxLayout(this);    //  status layout
    QHBoxLayout *button_layout = new QHBoxLayout(this);   //  button layout


    //  add wifi icon
    QLabel *wifiIconLab = new QLabel(this);
    wifiIconLab->setFixedSize(30, 30);
    QPixmap *wifiIcon = new QPixmap(setWifiIcon(ecn, rssi));
    *wifiIcon = wifiIcon->scaled(wifiIconLab->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    wifiIconLab->setPixmap(*wifiIcon);
    wifiIconLab->setObjectName("wifi-icon-lab");
    wifiIconLab->setStyleSheet("QLabel#wifi-icon-lab{background-color: rgba(255,255,255,0)}");
    wifiIconLab->setMaximumSize(30, 30);
    title_layout->addWidget(wifiIconLab);

    //  add wifi ssid
    QLabel *wifiSSID = new QLabel(ssid);
    QFont *id_font = new QFont();
    id_font->setPixelSize(18);
    id_font->setBold(true);
    wifiSSID->setFont(*id_font);
    wifiSSID->setObjectName("wifi-ssid");
    wifiSSID->setStyleSheet("QLabel#wifi-ssid{background-color: rgba(255,255,255,0)}");
    title_layout->addWidget(wifiSSID);

    main_layout->addLayout(title_layout);

    //  add status area
    QLabel *statusLabel = new QLabel(getEcn(ecn) + "\t" + "MAC: "+mac);
    QFont *s_font = new QFont();
    statusLabel->setObjectName("status-label");
    statusLabel->setStyleSheet("QLabel#status-label{color: #616161; margin-left: 30px; background-color:rgba(255,255,255,0);}");
    s_font->setPixelSize(12);
    statusLabel->setFont(*s_font);

    status_layout->addWidget(statusLabel);
    moreInfo_layout->addLayout(status_layout);

    //  add button area
    ctrlButton = new QPushButton("连接");
    ctrlButton->setAttribute(Qt::WA_StyledBackground, false);
    ctrlButton->setObjectName("ctrl-button");
    ctrlButton->setFixedSize(130, 30);
    ctrlButton->setStyleSheet("QPushButton#ctrl-button{ "
                                  "border-radius:8px;"
                                  "border: 1px solid #e4e4e4;"
                                  "background-color: #fbfbfb"
                              "}"
                              "QPushButton#ctrl-button:hover{"
                                  "background-color: rgb(242, 242, 242);"
                              "}"
                              "QPushButton#ctrl-button:pressed{"
                                  "background-color: #b2b0b3;"
                              "}"
                              );
    ctrlButton->setFocusPolicy(Qt::NoFocus);
    button_layout->addWidget(ctrlButton);
    button_layout->setAlignment(Qt::AlignRight);
    moreInfo_layout->addLayout(button_layout);
    connect(ctrlButton, SIGNAL(clicked()), this, SLOT(ctrlClickSlot()));

    moreInfo_widget->setLayout(moreInfo_layout);
    moreInfo_widget->setObjectName("more-info");
    moreInfo_widget->setStyleSheet("QWidget#more-info{background-color: rgba(255, 255, 255, 0)}");
    main_layout->addWidget(moreInfo_widget);

    moreInfo_widget->setHidden(true);
    //setFold();

    this->setLayout(main_layout);
}

WifiItem::~WifiItem() {
    delete ctrlButton;
    delete moreInfo_widget;
    qDeleteAll(children());
}

//  根据加密情况以及信号强度，获取相应icon的文件名
QString WifiItem::setWifiIcon(int ecn, int rssi) {
    QString iconName = ":/icons/";

    if (ecn) {
        iconName += "L";
    }
    iconName += "wifi";

    //  rssi: [-100, 40] -> [-100, 0] -> [0, 100]
    //  rssi的范围是 -100到40，但是在实际测试中没有遇到过大于 -20和低于-80的
    //  情况，所以这里把范围人为调整为 -100到0
    //  rssi += 100;    //easy to compute
    iconName += QString::number(rssi/20+1);
    iconName +=".png";

    return iconName;
}

QString WifiItem::getEcn(int ecn) {
    QString ecnArr[] = {
        "开放式", "WEP",
        "WPA_PSK", "WPA2_PSK",
        "WPA_WPA2_PSK", "WPA2_ENTERPRISE",
        "WPA3_PSK", "WPA2_WPA3_PSK",
        "WAPI_PSK "
    };

    return ecnArr[ecn];
}


//  添加自定义clicked信号
void WifiItem::mousePressEvent(QMouseEvent *ev)
{
#ifdef Q_OS_WIN
    mousePos = QPoint(ev->position().x(), ev->position().y());
#else
    mousePos = QPoint(ev->x(), ev->y());
#endif
}

void WifiItem::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)

#ifdef Q_OS_WIN
    if(mousePos == QPoint(ev->position().x(), ev->position().y())){
#else  //Q_OS_WIN
    if(mousePos == QPoint(ev->x(), ev->y())){
#endif //Q_OS_WIN
        qDebug() <<__FILE__ << __LINE__ << "emit! ";
        emit clicked(getIndex());
    }
}

void WifiItem::ctrlClickSlot() {
    emit clickConnect(this->getButtonText(), getIndex());
}

QString WifiItem::getRssiStr(int rssi) {
    // rssi: 很弱、弱、一般、较强、很强
    QString rssiArr[] = { "很弱", "弱", "一般", "较强", "很强" };

    return rssiArr[rssi/20];
}
