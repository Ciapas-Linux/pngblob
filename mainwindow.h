#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QSoundEffect>
#include <QPointF>
#include <QVector>
#include "mouse.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void zip(QString filename , QString zip_filename);
    void unZip(QString zip_filename , QString filename);
    int  randomBetween(int low, int high);
    std::string random_string( size_t length );

private slots:
    void on_Button_Close_clicked();

    void on_ButtonCode_clicked();

    void on_ButtonDecode_clicked();

    void on_ButtonKey_clicked();

    void Tmrupdate();

    void on_progressBar_valueChanged(int value);


private:
    Ui::MainWindow *ui;
    QImage image;
    QVector <QPointF> points;
    QVector <QGraphicsTextItem*> elips;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QSoundEffect sound_effect1;
    QSoundEffect sound_effect_click;
    QSoundEffect sound_effect_eror;
    QGraphicsTextItem *text1;
    QGraphicsTextItem *text2;
    QGraphicsTextItem *text3;
    QGraphicsTextItem *text4;
    QGraphicsTextItem *text5;
    QGraphicsTextItem *text6;
    QGraphicsTextItem *text7;
    QGraphicsTextItem *Title_text;
    QGraphicsPixmapItem *img_item;
    QTimer *timer;
    bool scale;
    QString danger = "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}";
    QString safe= "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}";
    QByteArray file_blob;
    QString App_path;
    Mouse *mouse;
};

#endif // MAINWINDOW_H
