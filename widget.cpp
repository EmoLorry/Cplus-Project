#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}




void Widget::on_pushButton_clicked()
{
    qInfo()<<"hd";

    //打开文件对话框，让用户选择音乐所在目录
    auto path=QFileDialog::getExistingDirectory(this,"选择音乐所在目录""C:\\Users\\28301\\Desktop\\Music");
    //根据路径，获取其中所有音乐文件
    QDir dir(path);
    auto musicList=dir.entryList(QStringList()<<"*.mp3"<<"*.wav");
    qInfo()<<musicList;

}

