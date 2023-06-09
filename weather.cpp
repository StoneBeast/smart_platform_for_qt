#include "weather.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QVBoxLayout>

Weather::Weather(QWidget *parent)
    : QWidget{parent}
{
    this->resize(800, 480);
    this->setMinimumSize(800, 480);

    //天气页采用垂直布局;
    QVBoxLayout *vbl = new QVBoxLayout();//天气相关控件垂直布局
    vbl->setAlignment(Qt::AlignHCenter); //控件水平居中
    //城市列表，以下拉菜单选择
    chnames<<"天津"<<"北京"<<"上海"<<"重庆"<<"沧州";
    pynames<<"tianjin"<<"beijing"<<"shanghai"<<"chongqing"<<"cangzhou";
    citycb = new QComboBox();
    for(int i=0; i<5; i++) {
        citycb->addItem(chnames[i]);
    }
    vbl->addWidget(citycb, 1);
    //城市选择变化会触发indexchangedslot()以进行天气更新
//    connect(citycb, &QComboBox::currentIndexChanged, this, &Weather::indexchangedslot);
    connect(citycb, SIGNAL(currentIndexChanged(int)), this, SLOT(indexchangedslot(int)));
    //实时温度标签，大号字体显示
    QFont font;
    font.setPixelSize(40);
    nowtemp = new QLabel("20");
    nowtemp->setFont(font);
    nowtemp->setAlignment(Qt::AlignHCenter);
    vbl->addWidget(nowtemp, 2);
    //实时天气标签
    nowwea = new QLabel();
    nowwea->setAlignment(Qt::AlignHCenter);
    vbl->addWidget(nowwea, 1);
    //三日天气信息标签
    for(int i=0; i<3; i++) {
        daily[i] = new QLabel();
        vbl->addWidget(daily[i], 1);
    }
    //面板设置布局
    this->setLayout(vbl);
    //启动时请求城市0之天气
    indexchangedslot(0);
}

//下拉菜单选择改变的槽函数
void Weather::indexchangedslot(int index) {
    //获取实时天气信息
    QString now = NOW;
    QString nowurl = now.arg(pynames[index]);    //格式化url字串加入城市
    qDebug() <<__FILE__ << __LINE__  << "now url: " << nowurl;
    QNetworkReply *reply = getRequest(nowurl);
    connect(reply, &QNetworkReply::readyRead, this, &Weather::nowreadslot);
    //获取每日天气信息
    QString daily = DAILY;
    QString dailyurl = daily.arg(pynames[index]);//格式化url字串加入城市
    qDebug() <<__FILE__ << __LINE__ << "daily url: " << dailyurl;
    QNetworkReply *rep = getRequest(dailyurl);
    connect(rep, &QNetworkReply::readyRead, this, &Weather::dailyreadslot);
}
//get请求函数，返回QNetworkReply* ——实例用于处理响应数据包
QNetworkReply* Weather::getRequest(QString url) {
    QNetworkRequest request;
    QNetworkAccessManager *manager;

    manager = new QNetworkAccessManager(this);
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager->get(request);
    //仅绑定finished信号，因为多个请求处理方式是统一的。
    connect(reply, &QNetworkReply::finished, this, &Weather::finishedslot);
    return reply;
}
//reply finished信号的槽函数——删除实例防止内存溢出
void Weather::finishedslot() {
    QNetworkReply *reply = (QNetworkReply *)sender();
    reply->deleteLater();
}
//响应数据包读取处理，now（即实时天气）请求
void Weather::nowreadslot() {
    QNetworkReply *reply = (QNetworkReply *)sender();
    QByteArray bytes = reply->readAll();
    QString data = QString::fromUtf8(bytes);

    QJsonParseError jperr;
    QJsonDocument jdoc = QJsonDocument::fromJson(data.toUtf8(), &jperr);
    if(jperr.error != QJsonParseError::NoError) {
        qDebug() <<__FILE__ << __LINE__  <<jperr.errorString();
        return;
    }
    QJsonObject jobject = jdoc.object();

    QJsonArray jaresults = jobject["results"].toArray();
    QJsonObject joresult = jaresults[0].toObject();
    QJsonObject jonow = joresult["now"].toObject();
    QString text = jonow["text"].toString();
    QString temp = jonow["temperature"].toString();
    QString last = joresult["last_update"].toString();
    //刷新控件
    nowtemp->setText(temp + "℃");
    nowwea->setText(text);
}
//响应数据包读取处理，now（即每日天气）请求
void Weather::dailyreadslot() {
    QNetworkReply *rep = (QNetworkReply *)sender();
    QByteArray bytes = rep->readAll();
    QString data = QString::fromUtf8(bytes);

    QJsonParseError jperr;
    QJsonDocument jdoc = QJsonDocument::fromJson(data.toUtf8(), &jperr);
    if(jperr.error != QJsonParseError::NoError) {
        qDebug() <<__FILE__ << __LINE__  << jperr.errorString();
        return;
    }
    QJsonObject jobject = jdoc.object();

    QJsonArray jaresults = jobject["results"].toArray();
    QJsonObject joresult = jaresults[0].toObject();
    QJsonArray jadaily = joresult["daily"].toArray();

    for(int i=0; i<jadaily.count(); i++) {
        QJsonObject jodaily = jadaily[i].toObject();
        QString date = jodaily["date"].toString();
        QString text_day = jodaily["text_day"].toString();
        QString text_night = jodaily["text_night"].toString();
        QString high = jodaily["high"].toString();
        QString low = jodaily["low"].toString();
        //刷新控件
        QString text = date + " | 日间：" + text_day + " | 夜间：" +
                text_night + " | 气温：" + low + "-" + high;
        daily[i]->setText(text);
    }
}
