#include "mainpage.h"
#include "ui_mainpage.h"
#include"mainwindow.h"

extern MainWindow *w;

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    ui->downPathInput->setObjectName("downPathInput");
    ui->downPathInput->setPlaceholderText("点击按钮选择下载路径");
    ui->selectPathBtn->setObjectName("selectPathBtn");
    ui->startDownBtn->setObjectName("startDownBtn");
    ui->backToLogBtn->setObjectName("backToLogBtn");
    //加载 qss 文件
    QFile file(":/qss/qss/mainPage.qss");
    /* 判断文件是否存在 */
    if (file.exists() ) {
        qDebug()<<"mainPage.qss 加载成功 ______"<<endl;
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }else {
        qDebug()<<"mainPage.qss 加载失败 ______"<<endl;
    }
    //获取服务端文件
    QString getFileUrl=Config::url+"/getFileList";
    QNetworkReply *reply=Request::GET(getFileUrl);
    QByteArray responseByte = reply->readAll();
    QString valid= responseByte;
    //转为 Json
    QJsonObject json_object2 = QJsonDocument::fromJson(responseByte).object();
    QJsonValue fileStr = json_object2.value("data");
    QJsonArray fileArray = fileStr.toArray();
    for ( QJsonValueRef ele:fileArray ) {
         QJsonValue value = ele;
         QStringList namePaths=value.toString().split("/");

         //针对每个文件获取它的断点信息
         //BreakPoint::breakPoints(":/config/break.txt")

         // 创建一个QWidget用于放置列表项的上下两部分
         QWidget *itemWidget = new QWidget();
         // 创建一个QHBoxLayout布局，将上下两部分放在一起
         QHBoxLayout *vBoxlayout = new QHBoxLayout();
         // 创建一个QLabel和QPushButton用于放在上部分
         QLabel *namelabel = new QLabel(namePaths[namePaths.size()-1]);
         namelabel->setObjectName("namelabel");
         // 创建一个QProgressBar用于放在下部分
         QProgressBar *progressBar = new QProgressBar();
         progressBar->setObjectName("progressBar");
         progressBar->setMaximumHeight(5);
         progressBar->setValue(0);
         progressBar->setTextVisible(false);

         vBoxlayout->addWidget(namelabel);

         // 创建一个占位符和一个伸缩因子，用来将按钮往右侧推
         QSpacerItem *spacer = new QSpacerItem(200, 0,
             QSizePolicy::Expanding, QSizePolicy::Minimum);
         vBoxlayout->addItem(spacer);

         QLabel *percentLabel = new QLabel(QString(QString::number(progressBar->value())+" %"));
         percentLabel->setObjectName("percentLabel");
         vBoxlayout->addWidget(percentLabel);

         QVBoxLayout *itemLayout = new QVBoxLayout(itemWidget);
         itemLayout->setObjectName("itemLayout");
         itemLayout->addLayout(vBoxlayout);
         itemLayout->addWidget(progressBar);
         //初始时不显示
         progressBar->hide();
         percentLabel->hide();

         // 将创建好的QWidget放置到列表中
         QListWidgetItem *item = new QListWidgetItem();
         item->setSizeHint(QSize(1000,60));


         ui->fileListWidget->addItem(item);
         ui->fileListWidget->setItemWidget(item, itemWidget);
    }
    ui->fileListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::startDown()
{
    //收集待下载文件的 id itemLayout
    QList<QListWidgetItem *> items = ui->fileListWidget->selectedItems();
    foreach (QListWidgetItem *item, items) {
        int row = ui->fileListWidget->row(item);
        this->selectedIndexList.append(row);

        QWidget *widget = ui->fileListWidget->itemWidget(item);
        QProgressBar *progressBar = widget->findChild<QProgressBar*>("progressBar");
        QLabel *percentLabel = widget->findChild<QLabel*>("percentLabel");
        //显示进度
        percentLabel->show();
        progressBar->show();
        widget->update();
    }
    DownLoad::startDown(this->username,ui->downPathInput->text(),this->selectedIndexList);
    this->selectedIndexList.clear();
}

void MainPage::stopDown()
{
    //收集待下载文件的 id itemLayout
    QList<QListWidgetItem *> items = ui->fileListWidget->selectedItems();
    foreach (QListWidgetItem *item, items) {
        int row = ui->fileListWidget->row(item);
        this->selectedIndexList.append(row);

        QWidget *widget = ui->fileListWidget->itemWidget(item);
        QProgressBar *progressBar = widget->findChild<QProgressBar*>("progressBar");
        QLabel *percentLabel = widget->findChild<QLabel*>("percentLabel");
        progressBar->setValue(0);
        percentLabel->setText(QString::number(progressBar->value())+" %");
        //隐藏进度
        percentLabel->hide();
        progressBar->hide();
        widget->update();
    }
    DownLoad::stopDown(this->username,this->selectedIndexList);
    this->selectedIndexList.clear();
}

void MainPage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor color(90, 185, 193);
    painter.setBrush(color);
    painter.drawRect(this->rect());
}

void MainPage::setUserName(QString username)
{
    this->username=username;
}

QString MainPage::getUserName()
{
    return this->username;
}

void MainPage::updatePercent(int id, int percent)
{
    emit this->updatePercentSignal(id,percent);
}

void MainPage::updatePercentSlot(int id, int percent)
{
    QListWidgetItem* item = ui->fileListWidget->item(id);

    QWidget *widget = ui->fileListWidget->itemWidget(item);
    QProgressBar *progressBar = widget->findChild<QProgressBar*>("progressBar");
    QLabel *percentLabel = widget->findChild<QLabel*>("percentLabel");
    //更新进度
    percentLabel->setText(QString::number(percent));
    progressBar->setValue(percent);
    widget->update();
}

void MainPage::setError(int id)
{
    emit this->setErrorSignal(id);
}

void MainPage::setErrorSlot(int id)
{
    qDebug()<<"文件 "<<id<<"  出错 !"<<endl;
    //修改进度颜色
    QListWidgetItem* item = ui->fileListWidget->item(id);
    QWidget *widget = ui->fileListWidget->itemWidget(item);
    QProgressBar *progressBar = widget->findChild<QProgressBar*>("progressBar");
    QLabel *percentLabel = widget->findChild<QLabel*>("percentLabel");
    percentLabel->setStyleSheet("color: rgb(255, 0, 0)");

    progressBar->setStyleSheet(
        "QProgressBar::chunk {"
        "    background-color: red;"
        "}"
    );

    widget->update();
}

void MainPage::on_selectPathBtn_clicked()
{
    //获取选择的目录路径
    QString selectedDir=QFileDialog::getExistingDirectory(this,"选择一个目录","./",QFileDialog::ShowDirsOnly);
    //若目录路径不为空
    if (!selectedDir.isEmpty())
    {
        //将路径中的斜杠替换为反斜杠
        selectedDir = selectedDir.replace(QRegExp("\\"), "/");
        //显示选择的目录路径
        ui->downPathInput->setText(selectedDir);
    }
}


void MainPage::on_startDownBtn_clicked()
{    
    //将用户数据封装成 Json
    QJsonObject info;
    info.insert("username",this->username);
    //创建一个QJsonDocument类对象，jsonDoc可以将Json格式数据转换之后，使用post发送
    QJsonDocument jsonDoc = QJsonDocument(info);
    QByteArray post_data = jsonDoc.toJson(QJsonDocument::Compact);
    //设置url
    QString regUrl = Config::url+"/down";
    //请求收到的结果
    QNetworkReply *reply=Request::POST(regUrl,post_data);
    QByteArray responseByte = reply->readAll();
    QString regResult = responseByte;
    qDebug()<<" 请求结果 ： "<<regResult<<endl;
    if(regResult=="ok"){
        //收集待下载文件的 id itemLayout
        QList<QListWidgetItem *> items = ui->fileListWidget->selectedItems();
        foreach (QListWidgetItem *item, items) {
            int row = ui->fileListWidget->row(item);
            this->selectedIndexList.append(row);

            QWidget *widget = ui->fileListWidget->itemWidget(item);
            QProgressBar *progressBar = widget->findChild<QProgressBar*>("progressBar");
            QLabel *percentLabel = widget->findChild<QLabel*>("percentLabel");
            //修改颜色为
            percentLabel->setStyleSheet("color: #0078d7");
            progressBar->setStyleSheet(
                "QProgressBar::chunk {"
                "    background-color: #0078d7;"
                "}"
            );
            widget->update();

            //显示进度
            percentLabel->show();
            progressBar->show();
            widget->update();
        }
        DownLoad::startDown(this->username,ui->downPathInput->text(),this->selectedIndexList);
        this->selectedIndexList.clear();
    }else{
        QMessageBox::information(this,
                                 "下载",
                                 "下载异常 !",
                                  "确定",
                                  "取消");
    }
}

void MainPage::on_fileListWidget_customContextMenuRequested(const QPoint &pos)
{
    //收集待下载文件的 id
    QList<QListWidgetItem *> items = ui->fileListWidget->selectedItems();
    foreach (QListWidgetItem *item, items) {
        int row = ui->fileListWidget->row(item);
        this->selectedIndexList.append(row);
        //修改颜色为
        QWidget *widget = ui->fileListWidget->itemWidget(item);
        QProgressBar *progressBar = widget->findChild<QProgressBar*>("progressBar");
        QLabel *percentLabel = widget->findChild<QLabel*>("percentLabel");
        percentLabel->setStyleSheet("color: #0078d7");

        progressBar->setStyleSheet(
            "QProgressBar::chunk {"
            "    background-color: #0078d7;"
            "}"
        );
        widget->update();
    }

    menu = new QMenu;
    QAction *downAction = new QAction(tr("开始下载"), this);
    // 将 QAction 的 triggered 信号连接到槽函数上
    QObject::connect(downAction, &QAction::triggered,this,&MainPage::startDown);
    menu->addAction(downAction);

    QAction *instantDown = new QAction(tr("继续下载"), this);
    // 将 QAction 的 triggered 信号连接到槽函数上
    QObject::connect(instantDown, &QAction::triggered,this,&MainPage::startDown);
    menu->addAction(instantDown);

    QAction *cancel = new QAction(tr("取消下载"), this);
    // 将 QAction 的 triggered 信号连接到槽函数上
    QObject::connect(cancel, &QAction::triggered,this,&MainPage::stopDown);
    menu->addAction(cancel);

    menu->popup(QCursor::pos());
}


void MainPage::on_backToLogBtn_clicked()
{
    this->destroy();
    w->show();
}

