#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QButtonGroup>
#include <QMainWindow>
#include <QNetworkReply>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QList<int> item_types;
    QList<QString> items;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QButtonGroup Bgroup;
    int requests_count;
private slots:
    void Start();
    void Stop();
    void DisplayError(QString QString);
    void SendRequest(QList<QString> items,bool IsHumanCraftingRequest,QString UserID);
    void replyFinished(QNetworkReply *reply);
signals:
    void StopSignal();
    void DisplayErrorSignal(QString);
    void SendRequestSignal(QList<QString>,bool,QString);
};

#endif // MAINWINDOW_H
