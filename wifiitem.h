#ifndef WIFIITEM_H
#define WIFIITEM_H

#include <QWidget>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>


class WifiItem : public QWidget
{
    Q_OBJECT
public:
    explicit WifiItem(QWidget *parent = nullptr);
    explicit WifiItem(QString ssid, int ecn, int rssi, QString mac, QWidget *parent = nullptr);

private:
    QString ssid;   // ssid
    int ecn;         // 加密格式
    int rssi;       //  信号强度
    QString mac;        //  mac地址
    QIcon rssiIcon; //  信号强度图标
    QPushButton conButton;  //连接按钮
    QString setWifiIcon(int ecn, int rssi);
    QString getEcn(int ecn);

signals:

};

#endif // WIFIITEM_H
