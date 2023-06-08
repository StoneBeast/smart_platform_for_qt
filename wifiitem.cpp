#include "wifiitem.h"

WifiItem::WifiItem(QWidget *parent)
    : QWidget{parent}
{

}

WifiItem::WifiItem(int index, QString ssid, int ecn, int rssi, QString mac, QWidget *parent)
    : QWidget{parent}, index(index), ssid(ssid), ecn(ecn), rssi(rssi),
      mac(mac)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setObjectName("wifi-item");
    this->setStyleSheet("QWidget{background-color:rgba(242, 242, 242);"
                        "border-radius:8px; border-left: solid 4px #0067c0;}"
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
    QPushButton *ctrlButton = new QPushButton("连接");
    ctrlButton->setAttribute(Qt::WA_StyledBackground, false);
    ctrlButton->setObjectName("ctrl-button");
    ctrlButton->setFixedSize(130, 30);
    ctrlButton->setStyleSheet("QPushButton#ctrl-button{ "
                                  "border-radius:8px;"
                                  "border: 1px solid #e4e4e4;"
                                  "background-color: #fbfbfb"
                              "}"
                              "QPushButton#ctrl-button:hover{"
                                  "background-color: rgba(242, 242, 242);"
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

    this->setLayout(main_layout);
    //connect(this, SIGNAL(clicked(int)), this, SLOT(mouseClicked(int)));
}

QString WifiItem::setWifiIcon(int ecn, int rssi) {
    QString iconName = ":/icons/";

    if (ecn) {
        iconName += "L";
    }
    iconName += "wifi";

    //  rssi: [-100, 40] -> [-100, 0]
    rssi += 100;    //easy to compute
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
//void WifiItem::mouseClicked(int t)
//{
//    //处理代码
//    qDebug() << "clicked! row:131";

//    if (moreInfo_widget->isHidden()) {
//        setExpansion();
//        //this->setFixedSize(expansionSize);

//    }
//}

void WifiItem::mousePressEvent(QMouseEvent *ev)
{
    //mousePos = QPoint(ev->position().x(), ev->position().y());
    mousePos = QPoint(ev->x(), ev->y());
}

void WifiItem::mouseReleaseEvent(QMouseEvent *ev)
{
    //if(mousePos == QPoint(ev->position().x(), ev->position().y()))
    if(mousePos == QPoint(ev->x(), ev->y())){
        qDebug() << "emit! row: 150";
        emit clicked(getIndex());
    }


}

void WifiItem::ctrlClickSlot() {
    qDebug() << "connect! row:155";
}

void WifiItem::setFold() {
    moreInfo_widget->setHidden(true);

}

void WifiItem::setExpansion() {
    moreInfo_widget->setHidden(false);
}

int WifiItem::getIndex() {
    return index;
}
