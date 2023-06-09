#ifndef WIFIOBJ_H
#define WIFIOBJ_H

#include <QString>


class WifiObj
{
public:
    WifiObj(QString ssid, int ecn, int rssi, QString mac, QString ip=nullptr, QString isInternet="no");

    // Getter functions
    QString ssid() const { return m_ssid; }
    int ecn() const { return m_ecn; }
    int rssi() const { return m_rssi; }
    QString mac() const { return m_mac; }
    QString ip() const { return m_ip; }
    QString isInternet() { return m_isInternet; }

    // Setter functions
    void setSsid(const QString &ssid) { m_ssid = ssid; }
    void setEcn(int ecn) { m_ecn = ecn; }
    void setRssi(int rssi) { m_rssi = rssi; }
    void setMac(const QString &mac) { m_mac = mac; }
    void setIp(const QString &ip) { m_ip = ip; }
    void setIsInternet(const QString &isInternet) { m_isInternet = isInternet; }

private:
    QString m_ssid;
    int m_ecn;        // 加密格式
    int m_rssi;       // 信号强度
    QString m_mac;    //  mac地址
    QString m_ip;     //  ip地址
    QString m_isInternet;
};

#endif // WIFIOBJ_H
