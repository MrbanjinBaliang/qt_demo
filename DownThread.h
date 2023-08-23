#include<QObject>
#include<QDebug>
#include<QTimer>
#include<QThread>
#ifndef DOWNTHREAD_H
#define DOWNTHREAD_H


class DownThread:public QThread{
    public:
        void run() override;
        void init(int id);
        void setIsWait(bool isW);
    signals:
        void updatePercent(int id,int percent);
        void setErrorSignal(int id);//出错标志
    private:
        int id;
        int percent=0;
        bool isWait=false;
};

#endif // DOWNTHREAD_H
