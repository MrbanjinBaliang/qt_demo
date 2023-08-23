#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include<QDebug>
#include<QMessageBox>


#include<QtCore>
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkReply>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include"config.h"
#include"Request.h"

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage();


private slots:
    void on_backToLogBtn_clicked();
    void on_startRegBtn_clicked();


private:
    Ui::RegisterPage *ui;
};

#endif // REGISTERPAGE_H
