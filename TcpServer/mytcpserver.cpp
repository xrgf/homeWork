#include "mytcpserver.h"
#include <QDebug>
MyTcpServer::MyTcpServer()
{

}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    //客户端链接上服务器
    qInfo()<<"new Client connected";
    MyTcpSocket*pTcpSocket=new MyTcpSocket;
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);

}
void MyTcpServer::deleteSocket(MyTcpSocket *mySockte)
{
    auto iter=m_tcpSocketList.begin();
    for(;iter!=m_tcpSocketList.end();iter++)
    {
        if(mySockte==*iter)
        {
            (*iter)->deleteLater();//释放list里面的值
            *iter=nullptr;
            m_tcpSocketList.erase(iter);//释放list中的占位
            break;
        }
    }


}

