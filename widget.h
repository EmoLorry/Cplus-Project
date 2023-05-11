#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUrl>
class QMediaPlayer;
class QAudioOutput;

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

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_8_clicked();

private:
    Ui::Widget *ui;
     QList<QUrl> playlist;//播放列表
    QAudioOutput*audiooutput;
    QMediaPlayer*mediaplayer;
    int cpindex=0;
};
#endif // WIDGET_H
