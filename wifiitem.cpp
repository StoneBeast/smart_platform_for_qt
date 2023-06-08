#include "wifiitem.h"

WifiItem::WifiItem(QWidget *parent)
    : QWidget{parent}
{

}

WifiItem::WifiItem(QString ssid, int ecn, int rssi, QString mac, QWidget *parent)
    : QWidget{parent}, ssid(ssid), ecn(ecn), rssi(rssi),
      mac(mac), foldSize(QSize(500, 40)), expansionSize(QSize(500, 110))
{
    this->setFixedSize(expansionSize);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setObjectName("wifi-item");
    this->setStyleSheet("QWidget{background-color:rgb(242, 242, 242);} QWidget#wifi-item{border-radius:8px;}");

    QVBoxLayout *main_layout = new QVBoxLayout(this);     //  main layout
    QHBoxLayout *title_layout = new QHBoxLayout(this);    //  title layout
    moreInfo_widget = new QWidget(this);
    QVBoxLayout *moreInfo_layout = new QVBoxLayout(this); //  will be hidden before cliecked
    QHBoxLayout *status_layout = new QHBoxLayout(this);    //  status layout
    QHBoxLayout *button_layout = new QHBoxLayout(this);   //  button layout


    //  add wifi icon
    QLabel *wifiIconLab = new QLabel(this);
    wifiIconLab->setFixedSize(25, 25);
    QPixmap *wifiIcon = new QPixmap(setWifiIcon(ecn, rssi));
    *wifiIcon = wifiIcon->scaled(wifiIconLab->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    wifiIconLab->setPixmap(*wifiIcon);
    wifiIconLab->setMaximumSize(30, 30);
    title_layout->addWidget(wifiIconLab);

    //  add wifi ssid
    QLabel *wifiSSID = new QLabel(ssid);
    QFont *id_font = new QFont();
    id_font->setPixelSize(18);
    id_font->setBold(true);
    wifiSSID->setFont(*id_font);
    title_layout->addWidget(wifiSSID);

    main_layout->addLayout(title_layout);

    //  add status area
    QLabel *statusLabel = new QLabel("       "+getEcn(ecn) + "\t" + "MAC: "+mac);
    QFont *s_font = new QFont();
    statusLabel->setObjectName("status-label");
    statusLabel->setStyleSheet("QLabel#status-label{color: #616161;}");
    s_font->setPixelSize(12);
    statusLabel->setFont(*s_font);

    status_layout->addWidget(statusLabel);
    moreInfo_layout->addLayout(status_layout);

    //  add button area
    QPushButton *ctrlButton = new QPushButton("连接");
    this->setAttribute(Qt::WA_StyledBackground, true);
    ctrlButton->setObjectName("ctrl-button");
    ctrlButton->setFixedSize(130, 30);
    ctrlButton->setStyleSheet("QPushButton#ctrl-button{background-color:rgb(255, 255, 255); "
                              "border-radius:8px;"
                              "border: 1px solid #e4e4e4;"
                              "}");
    ctrlButton->setFocusPolicy(Qt::NoFocus);
    button_layout->addWidget(ctrlButton);
    button_layout->setAlignment(Qt::AlignRight);
    moreInfo_layout->addLayout(button_layout);
    connect(ctrlButton, SIGNAL(clicked()), ctrlButton, SLOT(ctrlClickSlot()));

    moreInfo_widget->setLayout(moreInfo_layout);
    main_layout->addWidget(moreInfo_widget);

//    moreInfo_widget->setHidden(true);
//    this->setFixedSize(foldSize);

    this->setLayout(main_layout);
    connect(this, SIGNAL(clicked()), this, SLOT(mouseClicked()));
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
void WifiItem::mouseClicked()
{
    //处理代码
    qDebug() << "clicked!";

    if (moreInfo_widget->isHidden()) {
        moreInfo_widget->setHidden(false);
        this->setFixedSize(expansionSize);

    }
}

void WifiItem::mousePressEvent(QMouseEvent *ev)
{
    mousePos = QPoint(ev->position().x(), ev->position().y());
}

void WifiItem::mouseReleaseEvent(QMouseEvent *ev)
{
    if(mousePos == QPoint(ev->position().x(), ev->position().y()))
        emit clicked();

}

void WifiItem::ctrlClickSlot() {
    qDebug() << "connect!";
}
