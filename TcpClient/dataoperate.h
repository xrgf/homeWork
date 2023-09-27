#ifndef DATAOPERATE_H
#define DATAOPERATE_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>

class DataOperate : public QWidget
{
    Q_OBJECT
public:
    explicit DataOperate(QWidget *parent = nullptr);
    void hanleOpedbDataShow();

signals:
public slots:
    void handleDataOperator();//处理数据同步
    void hanleDataLoad();//对比客户端服务器数据是否一样




private:
    QListWidget *m_pDataListW;
    QPushButton*m_DataBtn;
    QPushButton*m_LoadBtn;
    QList<QString>m_listOpedaData;

};

#endif // DATAOPERATE_H
