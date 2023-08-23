#include"DownThread.h"
#include"mainwindow.h"

extern MainWindow *w;

void DownThread::run()
{
    qDebug() << "新的 下载 线程 running...    下载 id:"<<this->id<<endl;
    while (!isInterruptionRequested()||!this->isWait) {
        qDebug() << "*******下载进度:"<<this->percent<<endl;

        w->getMainPage()->updatePercent(this->id,this->percent);

        this->percent++;
        //模拟下载出错 ------------------
        if(this->percent==5){
            qDebug() << "*******下载出错 :"<<this->percent<<endl;
            w->getMainPage()->setError(this->id);
            this->setIsWait(true);
            wait();
            break;
        }else if(this->percent>=100){
            //模拟下载完成 ------------------
            qDebug() << "*******下载完成 :"<<this->percent<<endl;
            this->setIsWait(false);
            requestInterruption();
            break;
        }
        QThread::currentThread()->sleep(1);
    }
    // 执行需要在新线程中执行的操作
}

void DownThread::init(int id){
    this->id=id;
}

void DownThread::setIsWait(bool isW)
{
    this->isWait=isW;
}
