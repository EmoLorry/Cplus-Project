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
     audiooutput =new  QAudioOutput(this);
    //再一个媒体播放对象
     mediaplayer =new  QMediaPlayer(this);
     mediaplayer->setAudioOutput(audiooutput);

     //获取当前播放音乐总时长，通过信号关联来获取（同时增加滑块因素）
     connect(mediaplayer,&QMediaPlayer::durationChanged,this,[=](qint64 duration)
             {ui->labels->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg((duration/1000)%60,2,10,QChar('0')));
              ui->pcslider->setRange(0,duration);
             }
             );
    //获取当前播放时长（同时增加滑块因素）
     connect(mediaplayer,&QMediaPlayer::positionChanged,this,[=](qint64 position)
             {ui->labelc->setText(QString("%1:%2").arg(position/1000/60,2,10,QChar('0')).arg((position/1000)%60,2,10,QChar('0')));
              ui->pcslider->setValue(position);
             }
             );
    //拖动滑块改变进度
     connect(ui->pcslider,&QSlider::sliderMoved,mediaplayer,&QMediaPlayer::setPosition);
    //更改音量大小
     //audiooutput->setVolume(0.5);//[0,1]
     //connect(ui->voslider,&QSlider::sliderMoved,audiooutput,&QAudioOutput::setVolume);

}

Widget::~Widget()
{
    delete ui;
}




void Widget::on_pushButton_clicked()
{
    qInfo()<<"导入本地音乐文件";

    //打开文件对话框，让用户选择音乐所在目录
    auto path=QFileDialog::getExistingDirectory(this,"选择音乐所在目录""C:");
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
    if(playlist.empty())
        return;
    switch(mediaplayer->playbackState())
    {
    case QMediaPlayer:: PlaybackState::StoppedState:
    { //播放当前选中的音乐 首先获取选中的行号
        cpindex=ui->listWidget->currentRow();
        //播放对应音乐
        mediaplayer->setSource(playlist[cpindex]);
        mediaplayer->play();
        ui->pushButton_4->setStyleSheet("background-image: url(:/C:/Users/28301/Desktop/assets/123907.png);");
        break;
    }
    case QMediaPlayer:: PlaybackState::PlayingState:
    {
        mediaplayer->pause();
        ui->pushButton_4->setStyleSheet("background-image: url(:/C:/Users/28301/Desktop/assets/163649.png);");
        break;
    }
    case QMediaPlayer:: PlaybackState::PausedState:
    {
        mediaplayer->play();
        ui->pushButton_4->setStyleSheet("background-image: url(:/C:/Users/28301/Desktop/assets/123907.png);");
        break;
    }

    }

    //我们需要设置：如果正在播放点击就暂停，反之就播放，通过函数获取播放状态
}

//下一首
void Widget::on_pushButton_7_clicked()
{
    if(playlist.empty())
         return;
    cpindex=(cpindex+1)%playlist.size();
    ui->listWidget->setCurrentRow(cpindex);
    mediaplayer->setSource(playlist[cpindex]);
    mediaplayer->play();
}

//上一首
void Widget::on_pushButton_3_clicked()
{
    if(playlist.empty())
         return;
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

void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    cpindex = index.row();
    mediaplayer->setSource(playlist[cpindex]);
    mediaplayer->play();
    ui->pushButton_4->setStyleSheet("background-image: url(:/C:/Users/28301/Desktop/assets/123907.png);");
}


void Widget::on_pushButton_8_clicked()
{
    if(playlist.empty())
    return;

    if(audiooutput->volume()==0)
    {audiooutput->setVolume(50);
    ui->pushButton_8->setStyleSheet("background-image: url(:/C:/Users/28301/Desktop/assets/163804.png);");
    }
    else
    {audiooutput->setVolume(0);
    ui->pushButton_8->setStyleSheet("background-image: url(:/C:/Users/28301/Desktop/assets/175020.png);");
    }
}




