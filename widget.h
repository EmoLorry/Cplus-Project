#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUrl>
class QMediaPlayer;
class QAudioOutput;
class QTimer;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:


    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &);

    void on_pushButton_8_clicked();

    void nextmusic();

    void randommusic();

    void cyclemusic ();

   void on_radioButton_2_clicked();

   void on_radioButton_3_clicked();

   void on_radioButton_clicked();



   private:
    Ui::Widget *ui;
    QList<QUrl> playlist;//播放列表
    QAudioOutput *audiooutput;
    QMediaPlayer *mediaplayer;
    QTimer *timer;
    int cpindex=0;
    int playmodint =0;
    QMetaObject::Connection connection1;
    QMetaObject::Connection connection2;
    QMetaObject::Connection connection3;
};
#endif // WIDGET_H
