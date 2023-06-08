#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

//心知天气请求接口
#define NOW   "https://api.thinkpage.cn/v3/weather/now.json?key=SFl70ET8z9YLrmSmR&location=%1&language=zh-Hans&unit=c"
#define DAILY "https://api.seniverse.com/v3/weather/daily.json?key=SFl70ET8z9YLrmSmR&location=%1&language=zh-Hans&unit=c&start=0&days=5"

class Weather : public QWidget
{
    Q_OBJECT
public:
    explicit Weather(QWidget *parent = nullptr);

private:
    QWidget *weatabwid; //天气选项页包裹面板
    QLabel *nowwea;     //实时天气显示标签
    QLabel *nowtemp;    //实时气温显示标签
    QLabel *daily[3];   //三日天气显示标签
    QComboBox *citycb;  //选择城市的下拉框
    QList<QString> chnames;//中文城市名列表
    QList<QString> pynames;//拼音城市名列表

    QNetworkReply* getRequest(QString);//GET请求

private slots:
    void indexchangedslot(int);    //下拉框改变槽
    void finishedslot(); //HTTP响应结束槽
    void nowreadslot();  //请求实时天气读取槽
    void dailyreadslot();//请求每日天气读取槽
};

#endif // WEATHER_H
