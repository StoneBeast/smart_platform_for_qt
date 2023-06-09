#ifndef VISIBLENETWORK_H
#define VISIBLENETWORK_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "serialutil.h"
#include <QComboBox>
#include <QIcon>
#include <QPropertyAnimation>
#include "wifiitem.h"
#include <QDebug>
#include <QScrollArea>
#include "clicklabel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

class VisibleNetwork : public QWidget
{
    Q_OBJECT

public:
    explicit VisibleNetwork(QWidget *parent = nullptr);

    QScrollArea *scrollArea;
    QWidget *networkList;   //  网络列表控件
    QLabel *netWorkInfo;        //  显示网络信息
    QPushButton *checkButton;   //  检测连接按钮
    QPushButton *flushButton;   //  刷新网络列表按钮
    SerialUtil *serial;         //  串口成员
    QComboBox *serial_cb;       //  选择串口的下拉框
    ClickLabel *flushLab;

    WifiItem *item[5];
    QVBoxLayout *networkListLayout;
    QVBoxLayout *infoLayout;

    QString ssid;
    QString ip;
    QString rssi;
    QString ecn;
    QString isInternet;
    QString mac;
    QTimer *timer;

    QJsonArray wifiArray;
    QList<WifiObj> wifiItemList;


private:   
    void initNetworkList(); //  初始化网络连接列表
    void initSerial_cb(QComboBox *cb);  //  初始化串口选择下拉框
    void initNetInfo(); //  初始化网络信息
    void parseResult(QString result);
    void handleCheckDevice(QStringList s);  // check device connection
    void handleGetNetworkList(QStringList s);    //  get network list
    void handleGetNetworkStatus(QStringList s); //  get network status

private slots:
    void flushSerialSlot();         //  flush icon
    void checkConnectionSlot();     //  check button
    void expansionSlot(int index);  //  expansion
    void connectSlot(int index);    //  connection button
    void flushNetworkSlot();        //  flush list button
    void readyReadSlot();
    void timeoutSlot();

};

#endif // VISIBLENETWORK_H
