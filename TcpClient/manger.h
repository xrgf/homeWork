#ifndef MANGER_H
#define MANGER_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
class Manger : public QWidget
{
    Q_OBJECT
public:
    explicit Manger(QWidget *parent = nullptr);

signals:
public slots:
    void handleLoginManage();
private:
    QPushButton*m_pLoginBtn;
    QLineEdit*m_pPwdEt;


};

#endif // MANGER_H
