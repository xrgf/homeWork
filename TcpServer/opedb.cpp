#include "opedb.h"
#include <QMessageBox>
#include <QMap>
OpeDB::OpeDB(QObject *parent)
    : QObject{parent}
{
    m_db=QSqlDatabase::addDatabase("QSQLITE");
    init();
}

OpeDB::~OpeDB()
{
    m_db.close();
}

OpeDB &OpeDB::getInstance()
{
    static OpeDB instance;
    return instance;
}

void OpeDB::init()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("D:\\MyQtCode\\Qtcode\\homeWork\\TcpServer\\cloud.db");
    if(m_db.open())
    {

        QSqlQuery query;
        query.exec("select*from usrall");
        while(query.next())
        {
            QString data=QString("%1,%2")
                               .arg(query.value(0).toString())
                               .arg(query.value(1).toString())
                               ;
            qDebug()<<data;
        }
    }
    else
    {
        QMessageBox::critical(nullptr,"打开数据库失败","open db faild");
    }

}
bool OpeDB::handleLogin(const char *name, const char *pwd)
{

    if(nullptr==name||nullptr==pwd)
    {
        qInfo()<<"handleRegist function name or pwd is nullptr ";
        return false;
    }
    QString data1=QString("select name,pwd from usrall where name=\'%1\' and pwd=\'%2\'").arg(name).arg(pwd);
    qDebug()<<data1;
    QSqlQuery query1;
    query1.exec(data1);
    if(query1.next())
    {
        return false;
    }

    QString data=QString("insert into usrall(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    qDebug()<<data;
    QSqlQuery query;
    return query.exec(data);
}

void OpeDB::hanleTogetherData(const char *name, const char *pwd)
{
    if(nullptr==name||nullptr==pwd)
    {
        qInfo()<<"hanleTogetherData function name or pwd is nullptr ";
        return ;
    }
    //有就不添加 没有添加
    QString data1=QString("select name,pwd from usrall where name=\'%1\' and pwd=\'%2\'").arg(name).arg(pwd);
    qDebug()<<data1;
    QSqlQuery query1;
    query1.exec(data1);
    if(query1.next())
    {
        return ;
    }
    QString data=QString("insert into usrall(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
}

QMap<QString,int> OpeDB::getAllName()
{
    QSqlQuery query;
    QMap<QString,int>ret;
    query.exec("select name usrall");
    while(query.next())
    {
        QString data=QString("%1")
                           .arg(query.value(0).toString());


        qDebug()<<data;
        ret[data]=0;//先初始化所以键为0

    }

    query.exec("select name usrall");
    while(query.next())
    {
        QString data=QString("%1")
                           .arg(query.value(0).toString());


        qDebug()<<data;
        ret[data]++;//先初始化所以键为0

    }
    for(auto it=ret.begin();it!=ret.end();it++)
    {
        QString key = it.key();
        int value = it.value();
        qInfo()<<"Key"<<key<<"value"<<value;
    }
    return ret;//获取所以名字
}

QList<QString> OpeDB::hanleSerachName(const char *name)
{
    QSqlQuery query;
    QList<QString>ret;
    QString data=QString("select*from usrall where name=\'%1\'").arg(name);
    query.exec(data);
    while(query.next())
    {
        QString data=QString("%1,%2")
                           .arg(query.value(0).toString())
                           .arg(query.value(1).toString());

        qDebug()<<data;
        ret.push_back(data);
    }
    return ret;

}
QList<QString> OpeDB::DataShow()
{

    //用来返回数据库所有字段的
    QSqlQuery query;
    QList<QString>ret;
    query.exec("select*from usrall");
    while(query.next())
    {
        QString data=QString("%1,%2")
                           .arg(query.value(0).toString())
                           .arg(query.value(1).toString());

        qDebug()<<data;
        ret.push_back(data);
    }
    return ret;
}
