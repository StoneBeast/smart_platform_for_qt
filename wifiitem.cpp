#include "wifiitem.h"

WifiItem::WifiItem(QWidget *parent)
    : QWidget{parent}
{

}

WifiItem::WifiItem(QString ssid, int ecn, int rssi, QString mac, QWidget *parent)
    : QWidget{parent}, ssid(ssid), ecn(ecn), rssi(rssi), mac(mac)
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);     //  main layout
    QWidget *main_widget = new QWidget();
    QHBoxLayout *title_layout = new QHBoxLayout(this);    //  title layout
    QVBoxLayout *moreInfo_layout = new QVBoxLayout(this); //  will be hidden before cliecked
    QHBoxLayout *status_layout = new QHBoxLayout(this);    //  status layout
    QHBoxLayout *button_layout = new QHBoxLayout(this);   //  button layout


    //  add wifi icon
    QLabel *wifiIconLab = new QLabel(this);
    wifiIconLab->setFixedSize(20, 20);
    // qDebug() << setWifiIcon(ecn, rssi);
    QPixmap *wifiIcon = new QPixmap(setWifiIcon(ecn, rssi));
    *wifiIcon = wifiIcon->scaled(wifiIconLab->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    wifiIconLab->setPixmap(*wifiIcon);
    wifiIconLab->setMaximumSize(30, 30);
    title_layout->addWidget(wifiIconLab);

    //  add wifi ssid
    QLabel *wifiSSID = new QLabel(ssid);
    title_layout->addWidget(wifiSSID);

    main_layout->addLayout(title_layout);

    //  add status area
    QLabel *statusLabel = new QLabel("       "+getEcn(ecn) + "\t" + "MAC: "+mac);
    QFont *s_font = new QFont();
    s_font->setPixelSize(14);
    statusLabel->setFont(*s_font);

    //QLabel *macLabel = new QLabel("MAC: "+mac);
    status_layout->addWidget(statusLabel);
    //status_layout->addWidget(macLabel);
    //status_layout->setMargin(40);

    moreInfo_layout->addLayout(status_layout);

    //  add button area
    QPushButton *ctrlButton = new QPushButton("连接");
    button_layout->addWidget(ctrlButton);

    moreInfo_layout->addLayout(button_layout);

    main_layout->addLayout(moreInfo_layout);

    //main_layout->setSpacing(0);
    //this->setStyleSheet("QWidget{borde: 0}");
    setStyleSheet("background-color: rgb(125, 155, 155);");
    this->setLayout(main_layout);
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
