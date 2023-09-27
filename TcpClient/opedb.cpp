#include "opedb.h"
#include <QMessageBox>
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
    m_db.setDatabaseName("D:\\MyQtCode\\Qtcode\\homeWork\\TcpClient\\cloud.db");
    if(m_db.open())
    {

        QSqlQuery query;
        query.exec("select*from usrInfo");
        while(query.next())
        {
            QString data=QString("%1,%2")
                               .arg(query.value(0).toString())
                               .arg(query.value(1).toString());

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
        qInfo()<<"handleLogin function name or pwd is nullptr ";
        return false;
    }
    //先判断数据库有没有 有就不添加没有就添加
    QString data1=QString("select name,pwd from usrInfo where name=\'%1\' and pwd=\'%2\'").arg(name).arg(pwd);
    qDebug()<<data1;
    QSqlQuery query1;
    query1.exec(data1);
    if(query1.next())
    {
        return false ;
    }

    QString data=QString("insert into usrInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    qDebug()<<data;
    QSqlQuery query;
    return query.exec(data);
}

bool OpeDB::handleDel(const char *name,const char*pwd)
{
    if(nullptr==name)
    {
        qInfo()<<"handleDel function name is nullptr ";
        return false;
    }
    QString data=QString("delete from usrInfo where name=\'%1\'  and  pwd=\'%2\' ").arg(name).arg(pwd);
    qDebug()<<data;
    QSqlQuery query;
    return query.exec(data);
}



QList<QString> OpeDB::DataShow()
{
    QSqlQuery query;
    QList<QString>ret;
    query.exec("select*from usrInfo");
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
void OpeDB::hanleTogetherData(const char *name, const char *pwd)
{
    if(nullptr==name||nullptr==pwd)
    {
        qInfo()<<"hanleTogetherData function name or pwd is nullptr ";
        return ;
    }

    QString data1=QString("select name,pwd from usrInfo where name=\'%1\' and pwd=\'%2\'").arg(name).arg(pwd);
    qDebug()<<data1;
    QSqlQuery query1;
    query1.exec(data1);
    if(query1.next())
    {
        return ;
    }
    QString data=QString("insert into usrInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
}

void OpeDB::hanleClearData()
{
    QString data=QString("delete from usrInfo ");
    qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
}


