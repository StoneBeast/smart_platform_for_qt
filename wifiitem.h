#ifndef WIFIITEM_H
#define WIFIITEM_H

#include <QWidget>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include "wifiobj.h"


class WifiItem : public QWidget
{
    Q_OBJECT

public:
    explicit WifiItem(QWidget *parent = nullptr);
    // WifiItem(int index, QString ssid, int ecn, int rssi, QString mac, QWidget *parent = nullptr);
    WifiItem(int index, WifiObj obj, QWidget *parent = nullptr);
    QPoint mousePos;
    QWidget *moreInfo_widget;
    void setFold();
    void setExpansion();
    int getIndex();
    void setButtonText(QString text) { conButton.setText(text); }
    QString getButtonText() { return conButton.text(); }

private:
    int index;      //index
    QString ssid;   // ssid
    int ecn;         // 加密格式
    int rssi;       //  信号强度
    QString mac;        //  mac地址
    QIcon rssiIcon; //  信号强度图标
    QPushButton conButton;  //连接按钮
    QString setWifiIcon(int ecn, int rssi);
    QString getEcn(int ecn);


private slots:
    //void mouseClicked();
    void ctrlClickSlot();


signals:
    void clicked(int index);
    void clickConnect(QString text, int index);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);


signals:

};

#endif // WIFIITEM_H
