#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QFile>
#include <QMessageBox>
#include "protocol.h"
#include "opedb.h"
#include "opewidget.h"
#include <QMessageBox>
TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    resize(300,250);
    loadConfig();
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);
    connect(&m_tcpSocket,&QTcpSocket::readyRead,this,&TcpClient::recvMsg);
}

TcpClient::~TcpClient()
{
    delete ui;
}



void TcpClient::loadConfig()
{

    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray baData=file.readAll();
        QString strData=baData.toStdString().c_str();

        file.close();
        strData.replace("\r\n"," ");

        QStringList strList=strData.split(" ");

        m_strIP=strList.at(0);
        m_usPort=strList.at(1).toUShort();
        qInfo()<<"Ip"<<m_strIP<<"port"<<m_usPort;
    }
    else
    {
        QMessageBox::critical(this,"open config ","open config failed");
    }
}

TcpClient &TcpClient::getInstance()
{
    static TcpClient instance;
    return instance;
}

QTcpSocket &TcpClient::getTcpSocket()
{
    return m_tcpSocket;
}
void TcpClient::recvMsg()
{

    qDebug()<<m_tcpSocket.bytesAvailable();
    qDebug()<<"**************";
    uint uiPDULen=0;//??为什么能读到大小  答:结构体第一个
    m_tcpSocket.read((char*)&uiPDULen,sizeof(uint));//读到uiPDULen
    uint uiMsgLen=uiPDULen-sizeof(PDU);       //分二次去读取
    PDU*pdu=mkPDU(uiMsgLen);
    m_tcpSocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));

    switch(pdu->uiMsgType)
    {

    case ENUM_MSG_TYPE_LOGIN_RESPOND:
    {

        //收到回复说明服务器在线
        //服务器在线就把刚才传给客户端这边的删除
        OpeDB::getInstance().handleDel(pdu->caData,pdu->caData+32);
        //OpeWidget::getInstance().getDataOperate()->show();

        break;

    }
    case ENUM_MSG_TYPE_DATA_TOGETHER_RESPOND:
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
        //因为同步了数据库 类表刷新一下
        OpeWidget::getInstance().getDataOperate()->hanleOpedbDataShow();//用来在列表显示数据

        break;
    }
    case ENUM_MSG_TYPE_DATA_COMPARE_RESOPND:
    {

        QListWidget *list=new QListWidget;
        int size=pdu->uiMsgLen/64;
        for(int i=0;i<size;i++)
        {
            list->addItem((char*)pdu->caMsg+i*64);
        }
        list->show();
        auto IsYes=QMessageBox::information(this,"覆盖客户端"," cover client",QMessageBox::StandardButton::Yes,QMessageBox::StandardButton::No);

        //qInfo()<<(QMessageBox::StandardButton)IsYes;
        if(IsYes==QMessageBox::StandardButton::Yes)
        {
            //删除本地数据库
            OpeDB::getInstance().hanleClearData();

            //服务器数据库添加到本地
            char caName[32]={'\0'};
            char caPwd[32]={'\0'};
            char caNameAndPwd[64]={'\0'};
            for(int i=0;i<size;i++)
            {

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
            // 类表刷新一下
            OpeWidget::getInstance().getDataOperate()->hanleOpedbDataShow();//用来在列表显示数据

            list->hide();
        }
        //qInfo()<<"compare end";
    }
    default:
        break;
    }
    free(pdu);
    pdu=nullptr;

}

void TcpClient::on_login_pb_clicked()
{
    QString strName=ui->name_le->text();
    m_strLoginName=strName;
    QString strPwd=ui->pwd_le->text();
    m_strLoginPwd=strPwd;
    //直接先加入客户端数据库 如果服务器在线把客户端这边删除
    OpeDB::getInstance().handleLogin(strName.toUtf8().toStdString().c_str()
                                     ,strPwd.toUtf8().toStdString().c_str());

    OpeWidget::getInstance().show();
    this->hide();
    qInfo()<<"test";
    OpeWidget::getInstance().getDataOperate()->hanleOpedbDataShow();//用来在列表显示数据
    qInfo()<<"end";
    if(!strName.isEmpty()&&!strPwd.isEmpty())
    {
        PDU*pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_REQUEST;
        strcpy_s(pdu->caData,32,strName.toUtf8().toStdString().c_str());
        strcpy_s(pdu->caData+32,32,strPwd.toUtf8().toStdString().c_str());
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=nullptr;

    }
    else
    {
        QMessageBox::critical(this,"登录","登录失败 用户名或者密码为空:");
    }
}

