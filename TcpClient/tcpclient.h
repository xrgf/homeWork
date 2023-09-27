#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient; }
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void loadConfig();
    static TcpClient&getInstance();
    QTcpSocket&getTcpSocket();

private slots:
    void on_login_pb_clicked();
    void recvMsg();

private:
    Ui::TcpClient *ui;
    QString m_strIP;
    quint64 m_usPort;
    //链接服务器，和为服务器数据交互
    QTcpSocket m_tcpSocket;
    QString m_strLoginName;
    QString m_strLoginPwd;

    bool m_serverOnline=false;
};
#endif // TCPCLIENT_H
