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


class WifiItem : public QWidget
{
    Q_OBJECT
public:
    explicit WifiItem(QWidget *parent = nullptr);
    explicit WifiItem(QString ssid, int ecn, int rssi, QString mac, QWidget *parent = nullptr);
    QPoint mousePos;
    QWidget *moreInfo_widget;

private:
    QString ssid;   // ssid
    int ecn;         // 加密格式
    int rssi;       //  信号强度
    QString mac;        //  mac地址
    QIcon rssiIcon; //  信号强度图标
    QPushButton conButton;  //连接按钮
    QString setWifiIcon(int ecn, int rssi);
    QString getEcn(int ecn);
    QSize foldSize;
    QSize expansionSize;

private slots:
    void mouseClicked();
    void ctrlClickSlot();


signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);


signals:

};

#endif // WIFIITEM_H
