#include "wifiobj.h"

WifiObj::WifiObj(QString ssid, int ecn, int rssi, QString mac, QString ip, QString isInternet)
    : m_ssid(ssid), m_ecn(ecn), m_rssi(rssi), m_mac(mac), m_ip(ip), m_isInternet(isInternet)
{

}

