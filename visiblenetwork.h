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
#include <QTimer>
#include <QMessageBox>
#include <QInputDialog>

#define DEBUG 1

class VisibleNetwork : public QWidget
{
    Q_OBJECT

public:
    explicit VisibleNetwork(QWidget *parent = nullptr);

    QScrollArea *scrollArea;    //  置于networkList中，如果列表过长可以有滚动的效果
    QWidget *networkList;       //  网络列表控件
    QLabel *netWorkInfo;        //  显示网络信息
    QPushButton *checkButton;   //  检测连接按钮
    QPushButton *flushButton;   //  刷新网络列表按钮
    SerialUtil *serial;         //  串口成员
    QComboBox *serial_cb;       //  选择串口的下拉框
    ClickLabel *flushLab;
    bool deviceConnectionFlag() { return m_deviceConnectionFlag; }  //  获取设备连接状态
    void setDeviceConnectionFlag(bool);                             //  设置设备连接状态
    bool networkConnectionFlag() { return m_networkConnectionFlag; }//  获取网络连接状态
    void setNetworkConnectionFlag(bool);                            //  设置网络连接状态

#if DEBUG == 1
    WifiItem *item[5];
#endif

    QVBoxLayout *networkListLayout;     // networkList部分的布局
    QVBoxLayout *infoLayout;            // 右侧网络信息部分的布局
    QTimer *timer;                      // 用于串口接受数据计时
    QByteArray serialBuf;               // 用于存储串口存放的数据，防止发送较长的数据而丢帧
    QList<WifiObj> wifiObjList;         // 存放所有网络wifi的信息的list
    QList<WifiItem*> tempList_QList;    // 存放networkList中的所有 WifiItem对象的指针

    enum WifiItemArgIndex { SSID, ECN, RSSI, MAC };
    enum NetworkInfoIndex {IS_CONNECTION, IP, INFO_MAC, IS_INTERNTER};

private:
    void initNetworkList(); //  初始化网络连接列表
    void initSerial_cb(QComboBox *cb);  //  初始化串口选择下拉框
    void initNetInfo(); //  初始化网络信息
    void parseResult(QString result);
    void handleCheckDevice(QStringList s);  // check device connection
    void handleGetNetworkList(QStringList s);    //  get network list
    void handleGetNetworkStatus(QStringList s); //  get network status
    void handleDisconnect(QStringList s);       //  disconnect network
    void handleConnectNetwork(QStringList s);   //  连接网络
    void handleChangeNetwork(QStringList s);    //  更改网络连接
    bool m_deviceConnectionFlag;
    bool m_networkConnectionFlag;
    void handleCanntOprnSerial();



private slots:
    void flushSerialSlot();         //  flush icon
    void checkConnectionSlot();     //  check button
    void expansionSlot(int index);  //  expansion
    void connectSlot(QString text, int index);    //  connection button
    void flushNetworkSlot();        //  flush list button
    void readyReadSlot();
    void timeoutSlot();

};

#endif // VISIBLENETWORK_H
