#include "manger.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include "protocol.h"
#include <tcpclient.h>
Manger::Manger(QWidget *parent)
    : QWidget{parent}
{

    m_pLoginBtn=new QPushButton("登录");
    m_pPwdEt=new QLineEdit;
    QHBoxLayout*blayout=new QHBoxLayout;
     blayout->addWidget(m_pPwdEt);
    blayout->addWidget(m_pLoginBtn);

    setLayout(blayout);
    connect(m_pLoginBtn,&QPushButton::clicked,this,&Manger::handleLoginManage);

}

void Manger::handleLoginManage()
{
    qInfo()<<"test";
    QString pwd=m_pPwdEt->text();
    if(pwd.isEmpty())
    {
        QMessageBox::warning(this,"进去管理员","请输入管理员密码");
    }
    if(strcmp(pwd.toUtf8().toStdString().c_str(),"111111")==0)
    {
        //进入管理模式 //要去获取服务器数据
        PDU*pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_DATA_CONFILCT_REQUEST;
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=nullptr;
        //目的是找出name一样pwd不一样 去选择一个name把其他name删除
        //进入服务器找出name一样pwd不一样的
        //需要再服务器把其他删除


    }
    else
    {
        QMessageBox::warning(this,"进入管理员","请输入正确的管理员密码");
    }

}
