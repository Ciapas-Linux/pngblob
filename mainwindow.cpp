#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QtGlobal>
#include <QTimer>
#include <QThread>
#include <QTextStream>
#include <QColor>
#include <QByteArray>
#include <QSoundEffect>
#include <QtWidgets>
#include <QPointF>
#include <QVector>
#include <string>

#include <algorithm>
#include <iterator>
#include <numeric>

#include <stdlib.h>
#include <stdio.h>
//#include <omp .h>

#include <math.h>

#include "mouse.h"

//static const int MouseCount = 2;
#define safety_buff 512

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QTextStream out(stdout);
    //out << "Starting......" << endl;

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0,0,640,480);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "paralyzed II"));
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);


//    for (int i = 0; i < MouseCount; ++i)
//    {
//            mouse = new Mouse;
//            mouse->setPos(::sin((i * 6.28) / MouseCount) * 200,
//                          ::cos((i * 6.28) / MouseCount) * 200);
//            scene->addItem(mouse);
//    }

//    QTimer timer1;
//    QObject::connect(&timer1, SIGNAL(timeout()), scene, SLOT(advance()));
//    timer1.start(1000 / 33);

    QFont serifFont("Times", 13, QFont::Bold);

    QFont Fontsm("Times", 8, QFont::Bold);

    QPen pen(Qt::yellow, 2, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);

    // Fill in points with n number of points
        for(int i = 0; i < 144; i++)
           points.append( QPointF(randomBetween(1, 190), randomBetween(1, 250)) );

        for(int i = 0; i< points.size(); i++)
        {
            elips.append(scene->addText(random_string( 1 ).c_str(),Fontsm));
            elips[i]->setX(points[i].x());
            elips[i]->setY(points[i].y());
            elips[i]->setDefaultTextColor(QColor(150,255,0));
        }
        // <a,b>: @x = a + rand() * (b-a)@

        //elips[0]->moveBy(2,2);


#define ww 80
#define hh 60



    //Title_text = scene->addText("pngblob v1.0 Mi$rocoft corp. 2019  all rights reversed",serifFont);
    //Title_text->setDefaultTextColor(QColor(255,255,0));
    //Title_text->setX(ww);
    //Title_text->setY(hh);

    text1 = scene->addText("-->png:   any file convert to png",serifFont);
    text1->setDefaultTextColor(QColor(0,255,0));
    text1->setX(ww);
    text1->setY(hh + 40);

    text2 = scene->addText("png-->:   convert png to any file",serifFont);
    text2->setDefaultTextColor(QColor(0,255,0));
    text2->setX(ww);
    text2->setY(hh + 60);

    text3 = scene->addText("key:        generate key",serifFont);
    text3->setDefaultTextColor(QColor(50,255,0));
    text3->setX(ww);
    text3->setY(hh + 80);

    text4 = scene->addText("exit:       simply exit",serifFont);
    text4->setDefaultTextColor(QColor(50,255,0));
    text4->setX(ww);
    text4->setY(hh + 100);

    text5 = scene->addText("......",serifFont);
    text5->setDefaultTextColor(QColor(0,255,0));
    text5->setX(ww);
    text5->setY(hh + 120);

    text6 = scene->addText("....",serifFont);
    text6->setDefaultTextColor(QColor(0,255,0));
    text6->setX(ww);
    text6->setY(hh + 140);

    text7 = scene->addText("..",serifFont);
    text7->setDefaultTextColor(QColor(0,255,0));
    text7->setX(ww);
    text7->setY(hh + 160);

    ui->graphicsView->show();
    ui->progressBar->setValue(0);

    //ui->graphicsView->

    scale = true;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Tmrupdate()));
    timer->setSingleShot(true);
    timer->start(400);

    App_path = QCoreApplication::applicationFilePath();
    App_path = App_path.remove(App_path.lastIndexOf("/") +1 ,
                                         App_path.length() -
                                         App_path.lastIndexOf("/"));

    sound_effect1.setSource(QUrl::fromLocalFile(":beer.wav"));
    sound_effect1.setVolume( static_cast<qreal>(1.0f) );
    sound_effect1.play();

    sound_effect_click.setSource(QUrl::fromLocalFile(":buttonc.wav"));
    sound_effect_click.setVolume( static_cast<qreal>(1.0f) );

    sound_effect_eror.setSource(QUrl::fromLocalFile(":eror.wav"));
    sound_effect_eror.setVolume( static_cast<qreal>(1.0f) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::randomBetween(int low, int high)
{
    return (qrand() % ((high + 1) - low) + low);
}

std::string MainWindow::random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "!@#$%^&*():<>?,.~`_+-"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ static_cast<unsigned int>( rand() ) % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

void MainWindow::zip(QString filename , QString zip_filename)
{
   QFile infile(filename);
   QFile outfile(zip_filename);
   infile.open(QIODevice::ReadOnly);
   outfile.open(QIODevice::WriteOnly);
   QByteArray uncompressed_data = infile.readAll();

   QByteArray compressed_data = qCompress(uncompressed_data, 9);
   outfile.write(compressed_data);
   infile.close();
   outfile.close();
}

void MainWindow::unZip(QString zip_filename , QString filename)
{
   QFile infile(zip_filename);
   QFile outfile(filename);
   infile.open(QIODevice::ReadOnly);
   outfile.open(QIODevice::WriteOnly);
   QByteArray uncompressed_data = infile.readAll();
   QByteArray compressed_data = qUncompress(uncompressed_data);
   outfile.write(compressed_data);
   infile.close();
   outfile.close();
}


void MainWindow::on_Button_Close_clicked()
{
    sound_effect_click.play();

    text1->setPlainText("Do zobaczenia");
    text1->update();
    text2->setPlainText("uruchom mnie jeszcze kiedyś");
    text2->update();
    text3->setPlainText("zakodujemy kilka");
    text3->update();
    text4->setPlainText("pliczków");
    text4->update();
    text5->setPlainText("zmienimy je w obrazki");
    text5->update();
    text6->setPlainText("...");
    text6->update();
    text7->setPlainText(".");
    text7->update();

    QThread::msleep(700);

    this->close();

}

void MainWindow::on_ButtonCode_clicked()
{

    text1->setPlainText("");
    text1->update();
    text2->setPlainText("");
    text2->update();
    text3->setPlainText("");
    text3->update();
    text4->setPlainText("");
    text4->update();
    text5->setPlainText("");
    text5->update();
    text6->setPlainText("");
    text6->update();
    text7->setPlainText("");
    text7->update();


    sound_effect_click.play();

    QImage key_img;
    if(! key_img.load(App_path + "key.png"))
    {
        text1->setPlainText("Nie moge otworzyć pliku key.png !");
        text1->update();
        text2->setPlainText("Możesz wygenerować klucz");
        text2->update();
        text3->setPlainText("klikając na przycisk z kluczykiem!");
        text3->update();
        text4->setPlainText("ok ?");
        text4->update();
        text5->setPlainText("...");
        text5->update();
        text6->setPlainText("..");
        text6->update();
        text7->setPlainText(".");
        text7->update();

        QMessageBox Msgbox;
        Msgbox.setText("Brak klucza ---> key.png");
        Msgbox.exec();

        sound_effect_eror.play();

        return;
    }


    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Otworzyc jakiś plik --> "), "", tr("All Files (*.*)"));

    if(fileName.length() == 0)
    {
        text1->setPlainText("Wybierz plik !");
        text1->update();
        text2->setPlainText("człowieku miałes wybrać plik");
        text2->update();
        text3->setPlainText("do obrobienia, co ty robisz ? ");
        text3->update();
        text4->setPlainText("jeszcze raz tak zrobisz i");
        text4->update();
        text5->setPlainText("skasuje losowo twoje pliki");
        text5->update();
        text6->setPlainText("albo sformatuje ci dysk");
        text6->update();
        text7->setPlainText("chyba wiesz o co chodzi ?");
        text7->update();

        QMessageBox Msgbox;
        Msgbox.setText("Nie wybrałeś pliku!");
        Msgbox.exec();

        sound_effect_eror.play();

     return;
    }

    QFile file(fileName + ".png");
    QString open_filepath = file.fileName();
    QString open_filename = file.fileName();
    open_filename = open_filename.remove(0,open_filename.lastIndexOf("/") + 1);
    open_filename = open_filename.remove(open_filename.lastIndexOf("."),4);

    QThread::msleep(500);
    timer->stop();
    text1->setPlainText("file: " + open_filename);
    text1->update();
    text2->setPlainText("rolowanie ...");
    text2->update();

    //ZIP ----->
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);


     zip(fileName , fileName + ".png");

     ui->progressBar->setValue(100);
    text2->setPlainText("zrolowano ... OK");
    text2->update();

    QThread::msleep(200);

    if (!file.open(QIODevice::ReadOnly))
    {
        text1->setPlainText("file: Cant open! file error");
        text1->update();

        QMessageBox Msgbox;
        Msgbox.setText("Nie moge otworzyć pliku!");
        Msgbox.exec();

        sound_effect_eror.play();

     return;
    }else
    {

    }


    if(file.size() <= 1024*4)
    {
        QMessageBox Msgbox;
        Msgbox.setText("Ten plik jest zbyt mały!");
        Msgbox.exec();
        sound_effect_eror.play();
        return;
    }


    char *data = new char[( file.size() ) + safety_buff];
    memset(data,0, static_cast<size_t>( file.size() )   + safety_buff );
    char *datas = data;

    QDataStream infile(&file);
    qint64 buff_size = infile.readRawData(data, static_cast<int>(file.size()) );
    if( buff_size !=  file.size() )
    {
        text1->setPlainText("file: reading error!");
        text1->update();

        QMessageBox Msgbox;
        Msgbox.setText("Nie mogę odczytać pliku!");
        Msgbox.exec();

        sound_effect_eror.play();
        delete[] datas;
        return;
    }else
    {
        text2->setPlainText("size: " + QString::number(buff_size));
        text2->update();
    }

    this->setCursor(QCursor(Qt::WaitCursor));

    ui->progressBar->setMaximum(file.size() / 3);
    ui->progressBar->setValue(0);

    int x = 0;
    int y = 1;
    const int width = 900;
    //#pragma omp parallel for default(shared)
    for(qint64 cnt = 0;cnt <= file.size(); cnt+=3)
    {
        if(x == width)
        {
          y++;
          x = 0;
          ui->progressBar->setValue(y);
        }
        x++;
    }

     if( x > (width - 5) )
     {
        y++;
     }

    text4->setPlainText(QString("pass 1:  ") + "x: " + QString::number(x) + "  y: " +  QString::number(y));
    text4->update();


    QImage img(900, y + 5, QImage::Format_RGB32);
    img.fill(QColor(0, 0, 0, 0));
    int prg_max = y;

    ui->progressBar->setMaximum(prg_max);
    ui->progressBar->setValue(0);

    x = 0;
    y = 0;
    int key_x = 0;
    int key_y = 0;
    QColor col = qRgb(0, 0, 0);
    qint64 counter_end = 0;
    //#pragma omp parallel for
    //#pragma omp parallel for num_threads(2)
    for(qint64 cnt = 0;cnt <= file.size(); cnt+=3)
    {
        if(x == width)
        {
          y++;
          x = 0;
          ui->progressBar->setValue(y);
        }

        if(key_x == key_img.width())
        {
          key_x = 0;
          key_y++;
        }
        if(key_y == key_img.height())
        {
          key_y = 0;
        }

        col = col.fromRgb( key_img.pixel(key_x,key_y) );
        key_x++;

        *data ^= static_cast<uint_fast8_t>(col.red());
        *(data + 1) ^= static_cast<uint_fast8_t>(col.green());
        *(data + 2) ^= static_cast<uint_fast8_t>(col.blue());

        img.setPixel( x , y , qRgb(*data,*(data + 1), *(data + 2)) );
        x++;
        data += 3;
     counter_end++;
    }



    text5->setPlainText(QString("pass 2:  ") + "x: " + QString::number(x) + "  y: " +  QString::number(y));
    text5->update();

    if( (x > 894) && (x <= 900))
    {
        switch( x )
        {
        case 895:
            text7->setPlainText("end: c1");
            text7->update();
            img.setPixel(x , y , qRgb(112 ,12 ,2));
            x++;
            img.setPixel(x , y , qRgb(3 ,13 ,113));
            x++;
            img.setPixel(x , y , qRgb(114 ,14 ,4));
            x++;
            img.setPixel(x , y , qRgb(5 ,15 ,115));
            x++;
            img.setPixel(x , y , qRgb(116 ,16 ,6));
            break;
        case 896:
            text7->setPlainText("end: c2");
            text7->update();
            img.setPixel(x , y , qRgb(112 ,12 ,2));
            x++;
            img.setPixel(x , y , qRgb(3 ,13 ,113));
            x++;
            img.setPixel(x , y , qRgb(114 ,14 ,4));
            x++;
            img.setPixel(x , y , qRgb(5 ,15 ,115));
            y++;
            x = 0;
            img.setPixel(x , y , qRgb(116 ,16 ,6));
            break;
        case 897:
            text7->setPlainText("end: c3");
            text7->update();
            img.setPixel(x , y , qRgb(112 ,12 ,2));
            x++;
            img.setPixel(x , y , qRgb(3 ,13 ,113));
            x++;
            img.setPixel(x , y , qRgb(114 ,14 ,4));
            y++;
            x = 0;
            img.setPixel(x , y , qRgb(5 ,15 ,115));
            x++;
            img.setPixel(x , y , qRgb(116 ,16 ,6));
            break;
        case 898:
            text7->setPlainText("end: c4");
            text7->update();
            img.setPixel(x , y , qRgb(112 ,12 ,2));
            x++;
            img.setPixel(x , y , qRgb(3 ,13 ,113));
            y++;
            x = 0;
            img.setPixel(x , y , qRgb(114 ,14 ,4));
            x++;
            img.setPixel(x , y , qRgb(5 ,15 ,115));
            x++;
            img.setPixel(x , y , qRgb(116 ,16 ,6));
            break;
        case 899:
            text7->setPlainText("end: c5");
            text7->update();
            img.setPixel(x , y , qRgb(112 ,12 ,2));
            x = 0;
            y++;
            img.setPixel(x , y , qRgb(3 ,13 ,113));
            x++;
            img.setPixel(x , y , qRgb(114 ,14 ,4));
            x++;
            img.setPixel(x , y , qRgb(5 ,15 ,115));
            x++;
            img.setPixel(x , y , qRgb(116 ,16 ,6));
            break;
        case 900:
            text7->setPlainText("end: c6");
            text7->update();
            x = 0;
            y++;
            img.setPixel(x , y , qRgb(112 ,12 ,2));
            x++;
            img.setPixel(x , y , qRgb(3 ,13 ,113));
            x++;
            img.setPixel(x , y , qRgb(114 ,14 ,4));
            x++;
            img.setPixel(x , y , qRgb(5 ,15 ,115));
            x++;
            img.setPixel(x , y , qRgb(116 ,16 ,6));
            break;
          }
       }else
       {
        text7->setPlainText("end: c-all");
        text7->update();
        img.setPixel(x , y , qRgb(112 ,12 ,2));
        x++;
        img.setPixel(x , y , qRgb(3 ,13 ,113));
        x++;
        img.setPixel(x , y , qRgb(114 ,14 ,4));
        x++;
        img.setPixel(x , y , qRgb(5 ,15 ,115));
        x++;
        img.setPixel(x , y , qRgb(116 ,16 ,6));
       }

    text3->setPlainText("processed: " + QString::number(counter_end * 3) + " bytes");
    text3->update();
    text6->setPlainText(QString("pass 3:  ") + "x: " + QString::number(x) + "  y: " +  QString::number(y));
    text6->update();

    //----------------------------------------
    //QImage image;
    //QByteArray ba(QByteArray::fromRawData(data, sizeof(data)));
    //QBuffer buffer(&ba);
    //buffer.open(QIODevice::WriteOnly);
    //image.save("gówno", "PNG"); // writes image into ba in PNG format
    //----------------------------------------
    //buffer.close();

    img.save(open_filepath,"PNG" , -1);

    ui->progressBar->setValue(prg_max);
    this->setCursor(QCursor(Qt::ArrowCursor));
    delete[] datas;
    timer->start();
}

void MainWindow::on_ButtonDecode_clicked()
{
    sound_effect_click.play();

    text1->setPlainText("");
    text1->update();
    text2->setPlainText("");
    text2->update();
    text3->setPlainText("");
    text3->update();
    text4->setPlainText("");
    text4->update();
    text5->setPlainText("");
    text5->update();
    text6->setPlainText("");
    text6->update();
    text7->setPlainText("");
    text7->update();

    QImage key_img;
    if(! key_img.load(App_path + "key.png"))
    {
        text1->setPlainText("Nie moge otworzyć pliku key.png !");
        text1->update();
        text2->setPlainText("Możesz wygenerować klucz");
        text2->update();
        text3->setPlainText("klikając na przycisk z kluczykiem!");
        text3->update();
        text4->setPlainText("ok ?");
        text4->update();
        text5->setPlainText("...");
        text5->update();
        text6->setPlainText("..");
        text6->update();
        text7->setPlainText(".");
        text7->update();

        QMessageBox Msgbox;
        Msgbox.setText("Brak klucza ---> key.png");
        Msgbox.exec();

        sound_effect_eror.play();

        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Otworzyc jakiś plik --> "), "", tr("All Files (*.*)"));

    if(fileName.length() == 0)
    {
        text1->setPlainText("Wybierz plik !");
        text1->update();
        text2->setPlainText("człowieku miałes wybrać plik");
        text2->update();
        text3->setPlainText("do zdekodowania, co ty robisz ? ");
        text3->update();
        text4->setPlainText("jeszcze raz ty tak zrobisz i");
        text4->update();
        text5->setPlainText("z twojego dysku zrobie kisiel");
        text5->update();
        text6->setPlainText("zobaczysz...");
        text6->update();
        text7->setPlainText("rozumisz!");
        text7->update();

        QMessageBox Msgbox;
        Msgbox.setText("Nie wybrałes pliku!");
        Msgbox.exec();

        sound_effect_eror.play();

     return;
    }

    QThread::msleep(400);
    timer->stop();

    QString open_filepath = fileName;
    QString open_filename = fileName;
    open_filepath = open_filename.remove(open_filename.lastIndexOf("."),4);
    open_filename = open_filename.remove(0,open_filename.lastIndexOf("/") + 1);
    open_filename = open_filename.remove(open_filename.lastIndexOf("."),4);

    text1->setPlainText("Go-->" + open_filename);
    text1->update();

    QImage img;
    if(! img.load(fileName))
    {
        text1->setPlainText("Nie mogę otworzyć pliku !!");
        text1->update();
        text2->setPlainText("To nie jest plik PNG!");
        text2->update();
        text3->setPlainText("Czy wiesz co robisz ?");
        text3->update();
        QMessageBox Msgbox;
        Msgbox.setText("Nie moge otworzyć pliku!");
        Msgbox.exec();
        sound_effect_eror.play();
        return;
    }

    this->setCursor(QCursor(Qt::WaitCursor));

    char *data = new char[ (( (img.height() * img.width()) * 3)) + 512];
    memset(data,0,static_cast<size_t>((( img.height() * img.width() ) * 3) + safety_buff ));
    char *datas = data;

    int prg_max = img.width();
    ui->progressBar->setMaximum(prg_max);

    qint64 x = 0;
    qint64 y = 0;
    qint64 cntr = 0;
    QColor col;
    int key_x = 0;
    int key_y = 0;
    QColor key_col;
    for( qint64 cnt = 0; cnt < img.height() * img.width();cnt++)
    {
       if(x == 900)
        {
          y++;
         x = 0;
         ui->progressBar->setValue( static_cast<int>(y) );
        }

       col = col.fromRgb( img.pixel(x, y) );

       if(col.red() == 112 && col.green() == 12 && col.blue() == 2)
       {
           col = col.fromRgb(img.pixel(x + 1, y));
           text4->setPlainText("end pass: " + QString::number(1));
           ui->graphicsView->update();
           if(col.red() == 3 && col.green() == 13 && col.blue() == 113)
           {
               col = col.fromRgb(img.pixel(x + 2, y));
               text4->setPlainText("end pass: " + QString::number(2));
               ui->graphicsView->update();
               if(col.red() == 114 && col.green() == 14 && col.blue() == 4)
               {
                   col = col.fromRgb(img.pixel(x + 3, y));
                   text4->setPlainText("end pass: " + QString::number(3));
                   ui->graphicsView->update();
                   if(col.red() == 5 && col.green() == 15 && col.blue() == 115)
                   {
                       col = col.fromRgb(img.pixel(x + 4, y));
                       text4->setPlainText("end pass: " + QString::number(4));
                       ui->graphicsView->update();
                       if(col.red() == 116 && col.green() == 16 && col.blue() == 6)
                       {
                           text4->setPlainText("end pass: " + QString::number(5));
                           ui->graphicsView->update();
                        break;
                       }
                    }
                 }

           }

       }


       if(key_x == key_img.width())
       {
         key_x = 0;
         key_y++;
       }
       if(key_y == key_img.height())
       {
         key_y = 0;
       }
       key_col = key_col.fromRgb( key_img.pixel(key_x,key_y) );
       key_x++;

       col = col.fromRgb( img.pixel(x, y) );

      // *data ^= static_cast<unsigned char>(key_col.red());
      // *(data + 1) ^= static_cast<unsigned char>(key_col.green());
      // *(data + 2) ^= static_cast<unsigned char>(key_col.blue());

       data[cntr] = static_cast<char>(col.red() );
       data[cntr] ^= static_cast<char>(key_col.red() );
       cntr++;
       data[cntr] = static_cast<char>(col.green() );
       data[cntr] ^= static_cast<char>(key_col.green() );
       cntr++;
       data[cntr] = static_cast<char>(col.blue() );
       data[cntr] ^= static_cast<char>(key_col.blue() );
       cntr++;
       x++;
     }

    text2->setPlainText("processed: " + QString::number(img.height() * img.width()) + " pixels");
    text2->update();

    QFile fileOut(open_filepath + ".tmp");

    if (!fileOut.open(QIODevice::WriteOnly))
    {
        text2->setPlainText("Can't open output file!");
        text2->update();
        QMessageBox Msgbox;
        Msgbox.setText("Nie mogę otworzyć pliku wyjściowego!");
        Msgbox.exec();
        sound_effect_eror.play();
        delete[] datas;
     return;
    }else
    {

    }

    cntr--;

    QDataStream Outfile(&fileOut);
    qint64 buff_size = Outfile.writeRawData(data, static_cast<int>(cntr) );
    if( buff_size ==  -1 )
    {
        text2->setPlainText("Write file error!");
        text2->update();
        QMessageBox Msgbox;
        Msgbox.setText("Błąd zapisu do pliku!");
        Msgbox.exec();
        sound_effect_eror.play();
        delete[] datas;
        return;
    }else
    {
        text3->setPlainText(QString("Save") + ": " + QString::number(buff_size) + " b");
        text3->update();
    }

    fileOut.close();
    unZip(open_filepath + ".tmp",open_filepath);
    QFile file(open_filepath + ".tmp");
    file.remove();
    ui->progressBar->setValue(prg_max);
    this->setCursor(QCursor(Qt::ArrowCursor));
    delete[] datas;
    timer->start();
}

void MainWindow::on_ButtonKey_clicked()
{
    sound_effect_click.play();

    text1->setPlainText("");
    text1->update();
    text2->setPlainText("");
    text2->update();
    text3->setPlainText("");
    text3->update();
    text4->setPlainText("");
    text4->update();
    text5->setPlainText("");
    text5->update();
    text6->setPlainText("");
    text6->update();
    text7->setPlainText("");
    text7->update();

    QMessageBox msgBox;
    msgBox.setWindowTitle("UWAGA ACHTUNG !!!");
    msgBox.setText("Czy na pewno utworzyć nowy klucz ???");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
      // do something
    }else
    {
        text1->setPlainText("A więc nie utworzono");
        text1->update();
        text2->setPlainText("nowego klucza");
        text2->update();
        text3->setPlainText("........");
        text3->update();
        text4->setPlainText(".......");
        text4->update();
        text5->setPlainText("......");
        text5->update();
        text6->setPlainText("...");
        text6->update();
        text7->setPlainText(".");
        text7->update();

        sound_effect_eror.play();
        return;
    }

    QThread::msleep(400);

    const int width = 512;
    const int height = 512;

    QImage img(width, height, QImage::Format_RGB32);

    this->setCursor(QCursor(Qt::WaitCursor));

    text1->setPlainText("-->");
    text1->update();
    text2->setPlainText("Start --->");
    text2->update();
    text3->setPlainText("Generate key file...");
    text3->update();

    int prg_max = height;
    ui->progressBar->setMaximum(prg_max);


    int x = 0;
    int y = 0;
    QRgb value = qRgb(0, 0, 0);
    qint64 cnt = 0;
    for(cnt = 0;cnt < width * height; cnt+=1)
    {
        if(x == width)
        {
           y++;
          x = 0;
          ui->progressBar->setValue(y);
        }
        value = qRgb(qrand() % 255,qrand() % 255,qrand() % 255);
        img.setPixel(x , y , value);
        x++;
    }

    QString open_filepath = QCoreApplication::applicationFilePath();
    open_filepath = open_filepath.remove(open_filepath.lastIndexOf("/") +1 ,
                                         open_filepath.length() -
                                         open_filepath.lastIndexOf("/"));

    img.save(open_filepath + "key.png","PNG" , -1);

   ui->progressBar->setValue(prg_max);

   text4->setPlainText("Utworzono klucz");
   text4->update();
   text5->setPlainText(QString::number(width) + " px  " + QString::number(height) + " px");
   text5->update();
   text6->setPlainText("processed: " + QString::number(cnt) + " pix");
   text6->update();
   text7->setPlainText("OK!");
   text7->update();
   this->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::Tmrupdate()
{
    for(int i = 0; i < elips.size(); i++)
    {
        elips[i]->setX(randomBetween(370,560));
        elips[i]->setY(randomBetween(170,260));
    }

   if(scale == true)
   {
    //ui->graphicsView->scale(1.01,1.01);
    scale = false;
   }else
   {
    //ui->graphicsView->scale(1.0,1.0);
    scale = true;
   }
   timer->start();
}

void MainWindow::on_progressBar_valueChanged(int value)
{

}








//        *data = *data << (char)123;
//        *(data + 1) = *(data + 1) << (char)12;
//        *(data + 1) = *(data + 2) << (char)2;
//        *data = *data >> (char)123;
//        *(data + 1) = *(data + 1) >> (char)12;
//        *(data + 1) = *(data + 2) >> (char)2;
        //*data = *data ^ (char)123;

        //*data ^= 123;
        //*(data + 1) ^= 123;
        //*(data + 2) ^= 123;

//if(y > 1)
//{
// if( x > (width - 5) ) //895
// {
  //  y++;
// }
//}


//image.load(":null-byte.jpg");
//img_item = new QGraphicsPixmapItem(QPixmap::fromImage(image));

//QPixmap pix;
//scene->addItem(img_item);

//scene->setsetForegroundBrush(QColor(255, 255, 255, 127));

// image.save("obrazek.png","PNG" , -1);
// QFile file("aaaaa");
// file.open(QIODevice::WriteOnly);
// file.write(":null-byte.jpg", qstrlen(":null-byte.jpg"));        // write to stderr
// file.close();

//QTimer timer;
//    QObject::connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
//    timer.start(1000 / 33);

//timer = new QTimer(this);
//connect(timer, SIGNAL(timeout()), this, SLOT(Tmrupdate()));
//timer->start(5000);

//#ButtonCode:hover{
//border-image: url(:from_png_off.png);
//}

//void Foo::paintEvent(QPaintEvent* event)
//{
//  QPainter painter(this);
//  std::vector<QPoint> points;
//  // Fill points with the points
//  painter.drawPolyLine(points.data(), static_cast<int>(points.size()));
//}


//    int i = 1;
//    int p = 0;
//        for(char c : dataString){
//           std::copy_if(p,dataString.end(),clearedDataString.begin(),[i,dataString](char c = ';')
//           {
//               if(dataString.at(i) != c)
//                   return true;});
//                    i++;
//        }








