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
    WifiItem(int index, WifiObj obj, QWidget *parent = nullptr);
    ~WifiItem();
    QPoint mousePos;
    QWidget *moreInfo_widget;
    void setFold() { this->moreInfo_widget->setHidden(true); }
    void setExpansion() { this->moreInfo_widget->setHidden(false); }
    int getIndex() { return index; }
    void setButtonText(QString text) { ctrlButton->setText(text); }
    QString getButtonText() { return ctrlButton->text(); }

private:
    int index;      //index
    QString ssid;   // ssid
    int ecn;         // 加密格式
    int rssi;       //  信号强度
    QString mac;        //  mac地址
    QIcon rssiIcon; //  信号强度图标
    QString setWifiIcon(int ecn, int rssi);
    QString getEcn(int ecn);
    QPushButton *ctrlButton;  //连接按钮


private slots:
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
