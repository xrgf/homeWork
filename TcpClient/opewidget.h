#ifndef OPEWIDGET_H
#define OPEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include "manger.h"
#include"dataoperate.h"
#include <QStackedWidget>
class OpeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpeWidget(QWidget *parent = nullptr);
    static OpeWidget&getInstance();
    DataOperate *getDataOperate();
signals:

private:
    QListWidget*m_pListW;
    Manger *m_pManage;
    DataOperate*m_pData;
    QStackedWidget*m_pSW;


};

#endif // OPEWIDGET_H
