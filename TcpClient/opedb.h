#ifndef OPEDB_H
#define OPEDB_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QList>
class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = nullptr);
    ~OpeDB();
    static OpeDB&getInstance();
    void init();
    bool handleLogin(const char *name, const char *pwd);
    bool handleDel(const char*name,const char*pwd);//删除数据 用于链接或者未连接操作
    void hanleTogetherData(const char*name,const char*pwd);//同步数据
    void hanleClearData();//清空数据库

    //QList<QString> hanleRead();
    QList<QString> DataShow();//输出数据库所有字段

signals:
public slots:
private:
    QSqlDatabase m_db; //链接数据库


};

#endif // OPEDB_H
