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

class VisibleNetwork : public QWidget
{
    Q_OBJECT

public:
    explicit VisibleNetwork(QWidget *parent = nullptr);

    QWidget *networkList;   //  网络列表控件
    QLabel *netWorkInfo;        //  显示网络信息
    QPushButton *checkButton;   //  检测连接按钮
    QPushButton *flushButton;   //  刷新网络列表按钮
    SerialUtil *serial;         //  串口成员
    QComboBox *serial_cb;       //  选择串口的下拉框
    QPushButton *flushIcon;     //  刷新图标
    QPropertyAnimation *iconAnimation;   //  刷新图标动画


private:
    void initNetworkList(); //  初始化网络连接列表
    void initSerial_cb(QComboBox *cb);  //  初始化串口选择下拉框
    void rollIcon(int angle, int duration); //  旋转图标
    void initNetInfo(); //  初始化网络信息

private slots:
    void flushSerialSlot();
    void checkConnectionSlot();
//    void flushListSlot();
};

#endif // VISIBLENETWORK_H
