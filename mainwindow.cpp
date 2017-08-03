#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QNetworkRequest>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    item_types.append(10);
    item_types.append(15);
    item_types.append(20);


    Bgroup.addButton(ui->common_button);
    Bgroup.addButton(ui->uncommon_button);
    Bgroup.addButton(ui->rare_button);
    Bgroup.setId(ui->common_button, 0);
    Bgroup.setId(ui->uncommon_button, 1);
    Bgroup.setId(ui->rare_button, 2);

    connect(ui->startButton,SIGNAL(clicked(bool)),this,SLOT(Start()));
    connect(this,SIGNAL(StopSignal()),this,SLOT(Stop()));
    connect(this,SIGNAL(DisplayErrorSignal(QString)),this,SLOT(DisplayError(QString)));
    connect(this,SIGNAL(SendRequestSignal(QList<QString>,bool,QString)),this,SLOT(SendRequest(QList<QString>,bool,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::DisplayError(QString error)
{
    QMessageBox messageBox;
    messageBox.critical(0,"Error ",error);
    messageBox.setFixedSize(500,200);

}


void MainWindow::Start()
{

    qDebug() << Bgroup.checkedId();
    if(Bgroup.checkedId()<0 || Bgroup.checkedId()>2)
    {
        emit DisplayErrorSignal("не выбран тип вещи!");
        return;
    }
    QString UserID = ui->UserID_input->toPlainText();
    if(UserID.size()<1)
    {
        emit DisplayErrorSignal("не установлен UserID!");
        return;
    }
    else if(UserID.size()<10)
    {
        emit DisplayErrorSignal("UserID коротковат!");
        return;
    }

    items = ui->ItemsID_input->toPlainText().trimmed().split(";");
    if(items.size()<1)
    {
        emit DisplayErrorSignal("не введены ItemsID!");
        return;
    }

    int slice_by  = item_types.at(Bgroup.checkedId());

    if(items.size()<slice_by)
    {
        emit DisplayErrorSignal("маловато вещей!");
        return;
    }


    int position = 0;

    bool IsHumanCraftingRequest = ui->IsHumanCraftingRequest->isChecked();
    requests_count = (items.size()/slice_by);
    for(int i=0;i<(items.size()/slice_by);i++)
    {
        QList<QString> items_sliced;
        qDebug() << "iteration №:" << i;
        for(int ii=0;ii<slice_by;ii++)
        {
             QString item = items.at(position).trimmed();
             items_sliced.append(item);
             qDebug() << item << position;
             position++;
        }
        emit SendRequest(items_sliced,IsHumanCraftingRequest,UserID);

    }

//    ui->ItemsID_input->clear();
//    ui->UserID_input->clear();
//    ui->logarea->clear();
    ui->groupBox->setEnabled(false);
    ui->groupBox_2->setEnabled(false);
    ui->groupBox_3->setEnabled(false);
    ui->startButton->setEnabled(false);

}

void MainWindow::Stop()
{
    ui->groupBox->setEnabled(true);
    ui->groupBox_2->setEnabled(true);
    ui->groupBox_3->setEnabled(true);
    ui->startButton->setEnabled(true);

}



void MainWindow::SendRequest(QList<QString> items, bool IsHumanCraftingRequest, QString UserID)
{
    QString human = IsHumanCraftingRequest ? "true" : "false";
    qDebug() << human;
    QByteArray jsonString = QString("{\"UserID\":\"%1\",\"IsHumanCraftingRequest\":\"%2\",\"InputItems\":\"%3\",\"ModifierId\":\"\"}").arg(UserID,human,items.join(";")).toUtf8() ;
    qDebug() << jsonString;


    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    QNetworkRequest request(QUrl("http://economybackend.primalcarnage.com/CraftItem"));
    //QNetworkRequest request(QUrl("http://httpbin.org/post"));
    request.setHeader(QNetworkRequest::ContentLengthHeader,
        "application/json");
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "291");
    request.setHeader(QNetworkRequest::UserAgentHeader,
        "UE3-PrimalCarnage,UE3Ver(10897)");
    manager->post(request,jsonString);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    requests_count--;
    ui->logarea->append(reply->readAll());

    if(requests_count==0)
    {
        emit StopSignal();
    }

}
