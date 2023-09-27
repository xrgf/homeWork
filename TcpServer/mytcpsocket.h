#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>

#include <QFile>
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);




public slots:
    void recvMsg();

private:
    QString m_strName;
      QList<QString>m_listOpedaData;


};

#endif // MYTCPSOCKET_H
