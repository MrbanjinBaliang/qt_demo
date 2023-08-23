#include "registerpage.h"
#include "ui_registerpage.h"


RegisterPage::RegisterPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterPage)
{
    ui->setupUi(this);

    ui->usernameInput->setPlaceholderText("请输入用户名");
    ui->passwordInput->setPlaceholderText("请输入密码");
    ui->emailInput->setPlaceholderText("请输如邮箱");
}

RegisterPage::~RegisterPage()
{
    delete ui;
}


void RegisterPage::on_backToLogBtn_clicked()
{
    this->destroy();
}


void RegisterPage::on_startRegBtn_clicked()
{
    //将用户数据封装成 Json
    QJsonObject info;
    info.insert("username",ui->usernameInput->text());
    info.insert("password",ui->passwordInput->text());
    info.insert("email",ui->emailInput->text());
    //创建一个QJsonDocument类对象，jsonDoc可以将Json格式数据转换之后，使用post发送
    QJsonDocument jsonDoc = QJsonDocument(info);
    QByteArray post_data = jsonDoc.toJson(QJsonDocument::Compact);
    //设置url
    QString regUrl = Config::url+"/register";
    //请求收到的结果
    QNetworkReply *reply=Request::POST(regUrl,post_data);
    QByteArray responseByte = reply->readAll();
    QString regResult = responseByte;
    //
    if(regResult=="ok"){
        QMessageBox::information(this,
                                 "注册结果",
                                 "您已成功注册 !",
                                  "确定",
                                  "取消");
        ui->usernameInput->setText("");
        ui->passwordInput->setText("");
        ui->emailInput->setText("");
    }else{
        QMessageBox::information(this,
                                 "注册结果",
                                 "注册异常，检查信息 !",
                                  "确定",
                                  "取消");
    }
}

