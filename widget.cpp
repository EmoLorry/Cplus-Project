#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QListWidget>
#include <QListView>
#include <QtMultimedia/QtMultimedia>
#include <QAudioOutput>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //如何播放音乐 先加载multimedia
    //先 new 出 output对象
    auto audioOutput =new QAudioOutput(this);
    //再一个媒体播放对象
    auto mediaPlayer =new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);

    //给播放器设置音乐
    mediaPlayer->setSource(QUrl::fromLocalFile("C:\\Users\\28301\\Desktop\\Music\\Alex Goot,Madilyn Bailey - Something Just Like This.mp3"));
    //播放
    mediaPlayer->play();
}

Widget::~Widget()
{
    delete ui;
}




void Widget::on_pushButton_clicked()
{
    qInfo()<<"hd";

    //打开文件对话框，让用户选择音乐所在目录
    auto path=QFileDialog::getExistingDirectory(this,"选择音乐所在目录""C://Users//28301//Desktop//Music");
    //根据路径，获取其中所有音乐文件
    QDir dir(path);
    auto musicList=dir.entryList(QStringList()<<"*.mp3"<<"*.wav");
    qInfo()<<musicList;
    ui->listWidget->addItems(musicList);
    //将音乐上传到界面中


}

