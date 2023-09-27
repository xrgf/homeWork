#ifndef OPEDB_H
#define OPEDB_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QMap>
class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = nullptr);
    ~OpeDB();
    static OpeDB&getInstance();
    void init();
    bool handleLogin(const char *name, const char *pwd);
    void hanleTogetherData(const char*name,const char*pwd);
    QMap<QString,int> getAllName();//获取数据库所有用户名
    QList<QString> hanleSerachName(const char*name);//找出一样的name
    QList<QString> DataShow();

signals:
public slots:
private:
    QSqlDatabase m_db; //链接数据库



};

#endif // OPEDB_H
