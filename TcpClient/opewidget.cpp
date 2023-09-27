#include "opewidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
OpeWidget::OpeWidget(QWidget *parent)
    : QWidget{parent}
    ,m_pListW(new QListWidget)
{

    m_pListW->addItem("管理员");
    m_pListW->addItem("数据同步");

    m_pManage=new Manger;
    m_pData=new DataOperate;
    m_pSW=new QStackedWidget;
    m_pSW->addWidget(m_pManage);
    m_pSW->addWidget(m_pData);
    QHBoxLayout *pMain=new QHBoxLayout;
    pMain->addWidget(m_pListW);
    pMain->addWidget(m_pSW);

    setLayout(pMain);
    connect(m_pListW,&QListWidget::currentRowChanged,m_pSW,&QStackedWidget::setCurrentIndex);
}
OpeWidget &OpeWidget::getInstance()
{

    static OpeWidget instance;
    return instance;
}

DataOperate* OpeWidget::getDataOperate()
{

    return m_pData;
}
