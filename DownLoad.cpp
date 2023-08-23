#include"DownLoad.h"
#include"mainwindow.h"

extern MainWindow *w;

void DownLoad::startDown(QString username,QString downPath,QList<int> downList){
    qDebug()<<"用户 : "<<username<<"  下载文件到  "<<downPath<<endl;
    for(int id:downList){
        qDebug()<<"开始下载文件 "<<id<<endl;
        DownThread *downThread = new DownThread;
        downThread->init(id);


        //进度条更新 槽连接
        //QObject::connect(downThread,&DownThread::updatePercent,w->getMainPage(),&MainPage::updatePercent);

        downThread->start();
    }
}

void DownLoad::instantDown()
{

}
void DownLoad::stopDown(QString username,QList<int> stopList){
    qDebug()<<"用户  "<<username<<"  取消下载"<<endl;
    for(int id:stopList){
        qDebug()<<"取消下载文件 "<<id<<endl;
    }
}

void DownLoad::updatePercent(int id,int percent){
    qDebug()<<"更新文件  "<<id<<"  的进度为 : "<<percent<<endl;
}
