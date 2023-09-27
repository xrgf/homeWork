
#include "mytcpsocket.h"
#include <QDebug>
#include <QStringList>
#include <string.h>
#include <QDir>
#include <qdebug.h>
#include <QFileInfoList>
#include <QMessageBox>
#include "protocol.h"
#include "opedb.h"
MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{

    connect(this,&QTcpSocket::readyRead,this,&MyTcpSocket::recvMsg);
    //让每个客服端自己处理自己



}

void MyTcpSocket::recvMsg()
{

    qDebug()<<this->bytesAvailable();
    //qDebug()<<"**************";
    uint uiPDULen=0;//??为什么能读到大小  答:结构体第一个
    this->read((char*)&uiPDULen,sizeof(uint));//读到uiPDULen.
    uint uiMsgLen=uiPDULen-sizeof(PDU);       //分二次去读取
    PDU*pdu=mkPDU(uiMsgLen);
    this->read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
    {
        PDU*respdu=mkPDU(0);
        respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;
        char caName[32]={'\0'};
        char caPwd[32]={'\0'};
        memcpy(caName,pdu->caData,32);
        memcpy(caPwd,pdu->caData+32,32);
        OpeDB::getInstance().handleLogin(caName,caPwd);
        memcpy(respdu->caData,caName,32);

        write((char*)respdu,respdu->uiPDULen);

        break;

    }
    case ENUM_MSG_TYPE_DATA_TOGETHER_REQUEST:
    {
        int size=pdu->uiMsgLen/64;
        char caName[32]={'\0'};
        char caPwd[32]={'\0'};
        char caNameAndPwd[64]={'\0'};
        for(int i=0;i<size;i++)
        {
            //将客户端数据同步到服务器
            qInfo()<<(char*)pdu->caMsg+64*i;
            strcpy_s(caNameAndPwd,64,(char*)pdu->caMsg+64*i);
            int index=0;
            for(int i=0;i<64;i++)
            {
                if(caNameAndPwd[i]==',')
                {
                    index=i;
                    break;
                }
            }
            //qInfo()<<index;
            memcpy(caName,caNameAndPwd,index);
            caName[index]={'\0'};
            qInfo()<<"caName"<<caName;
            memcpy(caPwd,caNameAndPwd+index+1,32);
            qInfo()<<"caPwd"<<caPwd;
            OpeDB::getInstance().hanleTogetherData(caName,caPwd);
            memset(caName,0,0);
            memset(caPwd,0,0);
            memset(caNameAndPwd,0,0);

        }

        PDU*respdu=nullptr;
        m_listOpedaData=OpeDB::getInstance().DataShow();//获取服务器中所有数据  用来同步客户端
        respdu=mkPDU(m_listOpedaData.size()*64);
        respdu->uiMsgType=ENUM_MSG_TYPE_DATA_TOGETHER_RESPOND;
        for(int i=0;i<m_listOpedaData.size();i++)
        {
            memcpy((char*)respdu->caMsg+64*i,m_listOpedaData[i].toUtf8().toStdString().c_str(),m_listOpedaData[i].size()+1);
            qInfo()<<(char*)respdu->caMsg+64*i;
        }
        write((char*)respdu,respdu->uiPDULen);

    }
    case ENUM_MSG_TYPE_DATA_COMPARE_REQUEST:
    {

        m_listOpedaData=OpeDB::getInstance().DataShow();
        m_listOpedaData.sort();//服务区的数据
        //首先判断二个大小是不是一样
        bool isIdentical=true;
        int serverSize=m_listOpedaData.size();
        int clientSize=pdu->uiMsgLen/64;
        if(serverSize!=clientSize)
        {
            isIdentical=false;
        }
        for(int i=0;i<serverSize;i++)
        {
            //判断二个是不是不一样 如果是不一样把服务器数据发给客户端显示
            if(strcmp(m_listOpedaData[i].toUtf8().toStdString().c_str(),(char*)pdu->caMsg+i*64)!=0)
            {
                isIdentical=false;
                break;
            }
        }
        if(!isIdentical)
        {
            //不相同处理 服务器数据发给客户端显示 并看客户端是否下载服务器数据
            PDU*respdu=mkPDU(m_listOpedaData.size()*64);
            respdu->uiMsgType=ENUM_MSG_TYPE_DATA_COMPARE_RESOPND;
            for(int i=0;i<m_listOpedaData.size();i++)
            {
                memcpy((char*)respdu->caMsg+64*i,m_listOpedaData[i].toUtf8().toStdString().c_str(),m_listOpedaData[i].size()+1);
                qInfo()<<(char*)respdu->caMsg+64*i;
            }
            write((char*)respdu,respdu->uiPDULen);

        }
    }
    case ENUM_MSG_TYPE_DATA_CONFILCT_REQUEST:
    {
        //把冲突的值找出来 name一样的
        QMap<QString,int>caName=OpeDB::getInstance().getAllName();
        QList<QString>confictName;
        //判断值不为1 的

        qInfo()<<"entry map";
        for(auto it=caName.begin();it!=caName.end();it++)
        {
            QString key = it.key();
            int value = it.value();
            qInfo()<<"Key"<<key<<"value"<<value;
            if(value>1)//用重复
            {
              QList<QString>ret= OpeDB::getInstance().hanleSerachName(key.toUtf8().toStdString().c_str());
               for(int i=0;i<ret.size();i++)
              {

                    qInfo()<<"test confict name";
                    qInfo()<<ret[i];
                    confictName.append(ret[i]); //把冲突的名字全部写进confictName
              }
            }
        }

        //通过名字查询字段


    }
    default:
        break;
    }

}
