#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QObject>
#include <QMainWindow>

#include "webcamstream.h"
#include "onistream.h"
#include "colordetector.h"
#include "utils.h"
#include "streamwidget.h"



int startApp(int argc, char *argv[]){

    QApplication a(argc, argv);
    QApplication::setApplicationName("VibeTrak");

    OniStream* stream = new OniStream;

    while (! stream->isOpened()){

        int ret = QMessageBox::question(nullptr, QObject::tr("VibeTrak"),
                                   QObject::tr("No Kinect Detected.\n"
                                      "Do you want to open a Kinect recording?"),
                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Retry);

        QString filename;
        switch(ret)
        {
        case QMessageBox::Yes:
            filename = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open Oni Stream"),  QDir::home().path(), QObject::tr("OpenNI Files (*.oni)"));
            if (filename!=""){
                stream = new OniStream(filename.toUtf8());
            }
            break;
        case QMessageBox::No:
            return 1;
            break;
        case QMessageBox::Retry:
            stream = new OniStream;
            break;
        default:
            return 1;
            break;
        }
    }

    StreamWidget* streamWidget = new StreamWidget(stream);

    streamWidget->show();
    streamWidget->start();

    QObject::connect(&a, SIGNAL(aboutToQuit()), streamWidget, SLOT(closing()));

    return a.exec();

}

int main(int argc, char *argv[])
{

    return startApp(argc, argv);

}
