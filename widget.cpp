#include <QAudioOutput>
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QListWidget>
#include <QListView>
#include <QtMultimedia/QtMultimedia>
#include <QUrl>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //如何播放音乐 先加载multimedia
    //先 new 出 output对象
     audiooutput =new QAudioOutput(this);
    //再一个媒体播放对象
     mediaplayer =new QMediaPlayer(this);
     mediaplayer->setAudioOutput(audiooutput);

    //历史代码
    //给播放器设置音乐
    //mediaPlayer->setSource(QUrl::fromLocalFile("C:\\Users\\28301\\Desktop\\Music\\Alex Goot,Madilyn Bailey - Something Just Like This.mp3"));
    //播放(测试）
    //mediaPlayer->play();


}

Widget::~Widget()
{
    delete  ui;
}




void Widget::on_pushButton_clicked()
{
    qInfo()<<"导入本地音乐文件";

    //打开文件对话框，让用户选择音乐所在目录
    auto path=QFileDialog::getExistingDirectory(this,"选择音乐所在目录""C://Users//28301//Desktop//Music");
    //根据路径，获取其中所有音乐文件
    QDir dir(path);
    auto musicList=dir.entryList(QStringList()<<"*.mp3"<<"*.wav");//仅仅把音乐名字是在listWidget中展示
    qInfo()<<musicList;
    ui->listWidget->addItems(musicList);
    //将音乐上传到界面中

    //默认选中第一个音乐
    ui->listWidget->setCurrentRow(0);

    //将音乐完整路径保存起来（通过遍历音乐列表中中的所有音乐）
    for(auto file :musicList)
        playlist.append(QUrl::fromLocalFile(path+"/"+file));



}


void Widget::on_pushButton_4_clicked()
{
    switch(mediaplayer->playbackState())
    {
    case QMediaPlayer:: PlaybackState::StoppedState:
    { //播放当前选中的音乐 首先获取选中的行号
        cpindex=ui->listWidget->currentRow();
        //播放对应音乐
        mediaplayer->setSource(playlist[cpindex]);
        mediaplayer->play();
        break;
    }
    case QMediaPlayer:: PlaybackState::PlayingState:
    {
        mediaplayer->pause();
        break;
    }
    case QMediaPlayer:: PlaybackState::PausedState:
    {
        mediaplayer->play();
        break;
    }

    }

    //我们需要设置：如果正在播放点击就暂停，反之就播放，通过函数获取播放状态
}

//下一首
void Widget::on_pushButton_7_clicked()
{
    cpindex=(cpindex+1)%playlist.size();
    ui->listWidget->setCurrentRow(cpindex);
    mediaplayer->setSource(playlist[cpindex]);
    mediaplayer->play();
}

//上一首
void Widget::on_pushButton_3_clicked()
{
    if(cpindex==0)
    {
    cpindex=playlist.size()-1 ;
    ui->listWidget->setCurrentRow(cpindex);
    mediaplayer->setSource(playlist[cpindex]);
    mediaplayer->play();
    }
    else
    {
    cpindex=(cpindex-1);
    ui->listWidget->setCurrentRow(cpindex);
    mediaplayer->setSource(playlist[cpindex]);
    mediaplayer->play();
    }
}


