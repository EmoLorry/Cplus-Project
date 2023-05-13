#include <QAudioOutput>
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QListWidget>
#include <QListView>
#include <QtMultimedia/QtMultimedia>
#include <QUrl>
#include <QTimer>
#include <QFrame>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置程序图标与小名称
    setWindowIcon(QIcon("C:\\Users\\28301\\Desktop\\assets\\203839.png"));
    setWindowTitle("音乐播放器2.1");

    //设置ui中textBrowser的表现形式
    ui->textBrowser->setStyleSheet("background:transparent;border-width:0;border->style:outset");

    //给两个时长标签初始化，防止Qtimer激发nextmusic函数读取未初始化的标签导致程序崩溃
    ui->labelc->setText("00:00");
    ui->labels->setText("00:00");

    //设置计时器,并启动
    timer=new QTimer();
    timer->start(1000);
    //connect(timer,SIGNAL(timeout()),this,SLOT(randommusic()));

    //先 new 出 output对象
     audiooutput =new  QAudioOutput(this);
    //再 new 出一个媒体播放对象
     mediaplayer =new  QMediaPlayer(this);
     mediaplayer->setAudioOutput(audiooutput);

     //获取当前播放音乐总时长，通过信号关联来获取（同时增加滑块因素）
     connect(mediaplayer,&QMediaPlayer::durationChanged,this,[=](qint64 duration)
             {ui->labels->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg((duration/1000)%60,2,10,QChar('0')));
              ui->pcslider->setRange(0,duration);
             }
             );
    //获取当前播放时长（同时增加滑块因素）
     connect(mediaplayer,&QMediaPlayer::positionChanged,this,[=](qint64 proposition)
             {ui->labelc->setText(QString("%1:%2").arg(proposition/1000/60,2,10,QChar('0')).arg((proposition/1000)%60,2,10,QChar('0')));
              ui->pcslider->setValue(proposition);
             }
             );
    //拖动滑块改变进度
     connect(ui->pcslider,&QSlider::sliderMoved,mediaplayer,&QMediaPlayer::setPosition);

     //音量反映到竖直滑条
     //connect(audiooutput,&QAudioOutput::volumeChanged,this,[=](float voposition)
     //{
     // ui->verticalSlider->setRange(0,50);
     // ui->verticalSlider->setValue(voposition);
     //}
     //);
     //竖直滑条反映音量
     //connect(ui->verticalSlider,SIGNAL(QSlider::volumeChanged()),audiooutput,SLOT(QAudioOutput::setVolume()));

}

Widget::~Widget()
{
    delete ui;
}


//顺序模式
void Widget::on_radioButton_2_clicked()
{
    if(playmodint==1)
        disconnect(connection1);
    if(playmodint==2)
        disconnect(connection2);
    if(playmodint==3)
        disconnect(connection3);

    connection1=connect(timer,SIGNAL(timeout()),this,SLOT(nextmusic()));
    playmodint =1;
}

//随机模式
void Widget::on_radioButton_3_clicked()
{
    if(playmodint==1)
    disconnect(connection1);
    if(playmodint==2)
    disconnect(connection2);
    if(playmodint==3)
    disconnect(connection3);


    connection2=connect(timer,SIGNAL(timeout()),this,SLOT(randommusic()));
    playmodint =2;
}

//单曲循环
void Widget::on_radioButton_clicked()
{
    if(playmodint==1)
    disconnect(connection1);
    if(playmodint==2)
    disconnect(connection2);
    if(playmodint==3)
    disconnect(connection3);

   connection3=connect(timer,SIGNAL(timeout()),this,SLOT(cyclemusic()));
   playmodint =3;
}

void Widget::nextmusic()
{
    if((ui->labelc->text()==ui->labels->text())&&ui->labels->text()!="00:00")
    {
        cpindex=(cpindex+1)%playlist.size();
        ui->listWidget->setCurrentRow(cpindex);
        mediaplayer->setSource(playlist[cpindex]);
        mediaplayer->play();
    }

}
void Widget::randommusic()
{
    if((ui->labelc->text()==ui->labels->text())&&ui->labels->text()!="00:00")
    {
        srand(time(NULL));
        cpindex=rand()%playlist.size();
        ui->listWidget->setCurrentRow(cpindex);
        mediaplayer->setSource(playlist[cpindex]);
        mediaplayer->play();
    }
}

void Widget::cyclemusic ()
{
    if((ui->labelc->text()==ui->labels->text())&&ui->labels->text()!="00:00")
    {
        mediaplayer->setSource(playlist[cpindex]);
        mediaplayer->play();
    }
}

//导入音乐所在的文件夹
void Widget::on_pushButton_clicked()
{
    qInfo()<<"导入本地音乐文件";
    //打开文件对话框，让用户选择音乐所在目录
    auto path=QFileDialog::getExistingDirectory(this,"选择音乐所在文件夹""C:");
    //根据路径，获取其中所有音乐文件
    QDir dir(path);
    auto musicList=dir.entryList(QStringList()<<"*.mp3"<<"*.wav");
    qInfo()<<musicList;
    ui->listWidget->addItems(musicList);
    //将音乐上传到界面中,仅仅把音乐名字是在listWidget中展示

    //默认选中第一个音乐
    ui->listWidget->setCurrentRow(0);

    //将音乐完整路径保存起来（通过遍历音乐列表中中的所有音乐）
    for(auto file :musicList)
        playlist.append(QUrl::fromLocalFile(path+"/"+file));


}

//播放键
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
        ui->pushButton_4->setStyleSheet("background-image: url(:/new/prefix5/C:/Users/28301/Desktop/assets/005441.png);");
        break;
    }
    case QMediaPlayer:: PlaybackState::PlayingState:
    {
        mediaplayer->pause();
        ui->pushButton_4->setStyleSheet("background-image: url(:/new/prefix5/C:/Users/28301/Desktop/assets/004607.png);");
        break;
    }
    case QMediaPlayer:: PlaybackState::PausedState:
    {
        mediaplayer->play();
        ui->pushButton_4->setStyleSheet("background-image: url(:/new/prefix5/C:/Users/28301/Desktop/assets/005441.png);");
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

//双击切换
void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    cpindex = index.row();
    mediaplayer->setSource(playlist[cpindex]);
    mediaplayer->play();
    ui->pushButton_4->setStyleSheet("background-image: url(:/new/prefix5/C:/Users/28301/Desktop/assets/005441.png);");
}

//声音开关键
void Widget::on_pushButton_8_clicked()
{

    if(audiooutput->volume()==0)
    {audiooutput->setVolume(50);
    ui->pushButton_8->setStyleSheet("background-image: url(:/prefix4/C:/Users/28301/Desktop/assets/224128.png);");
    }
    else
    {audiooutput->setVolume(0);
    ui->pushButton_8->setStyleSheet("background-image: url(:/prefix4/C:/Users/28301/Desktop/assets/225116.png);");
    }
}



