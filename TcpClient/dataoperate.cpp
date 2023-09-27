#include "dataoperate.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "opedb.h"
#include "tcpclient.h"
#include "protocol.h"
#include "opewidget.h"
#include <QtAlgorithms>
DataOperate::DataOperate(QWidget *parent)
    : QWidget{parent}
{

    m_pDataListW=new QListWidget ;
    m_DataBtn=new QPushButton("同步客户端与服务器数据");
    m_LoadBtn=new QPushButton("对比服务器数据");
    QVBoxLayout *vlayout=new QVBoxLayout;
    vlayout->addWidget(m_DataBtn);
    vlayout->addWidget(m_LoadBtn);
     QHBoxLayout*pMain=new QHBoxLayout;
     pMain->addWidget(m_pDataListW);
     pMain->addLayout(vlayout);
     setLayout(pMain);
     connect(m_DataBtn,&QPushButton::clicked,this,&DataOperate::handleDataOperator);
     connect(m_LoadBtn,&QPushButton::clicked,this,&DataOperate::hanleDataLoad);
}

void DataOperate::hanleOpedbDataShow()
{
     m_pDataListW->clear();//清空之前的
     m_listOpedaData=OpeDB::getInstance().DataShow();

     for(int i=0;i<m_listOpedaData.size();i++)
     {
         m_pDataListW->addItem(m_listOpedaData[i]);
     }

}

void DataOperate::handleDataOperator()
{

     //用来同步服务器与客户端数据库
   m_listOpedaData=OpeDB::getInstance().DataShow();
   PDU*pdu=mkPDU(m_listOpedaData.size()*64);
   pdu->uiMsgType=ENUM_MSG_TYPE_DATA_TOGETHER_REQUEST;
   for(int i=0;i<m_listOpedaData.size();i++)
   {
       memcpy((char*)pdu->caMsg+64*i,m_listOpedaData[i].toUtf8().toStdString().c_str(),m_listOpedaData[i].size()+1);
       qInfo()<<(char*)pdu->caMsg+64*i;
   }
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;

     //1 本地传到服务器
     //2 服务器拷贝到本地


}

void DataOperate::hanleDataLoad()
{

    //客户端与服务器数据对比 先发过去然后服务器哪里不一样就发过来看服务器的
    m_listOpedaData=OpeDB::getInstance().DataShow();
    m_listOpedaData.sort();//排好序的发过去
    PDU*pdu=mkPDU(m_listOpedaData.size()*64);
    pdu->uiMsgType=ENUM_MSG_TYPE_DATA_COMPARE_REQUEST;
    for(int i=0;i<m_listOpedaData.size();i++)
    {
       memcpy((char*)pdu->caMsg+64*i,m_listOpedaData[i].toUtf8().toStdString().c_str(),m_listOpedaData[i].size()+1);
       qInfo()<<(char*)pdu->caMsg+64*i;
    }
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}
