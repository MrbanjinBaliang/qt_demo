#include<QObject>
#include<QWidget>
#include<QDebug>
#include<QTimer>
#include<QThread>

#include"DownThread.h"

#ifndef DOWNLOAD_H
#define DOWNLOAD_H

class DownLoad{
    public slots:
    static void startDown(QString username,QString downPath,QList<int> downList);
    static void instantDown();
    static void stopDown(QString username,QList<int> stopList);
    static void updatePercent(int id,int percent);
};
#endif // DOWNLOAD_H
