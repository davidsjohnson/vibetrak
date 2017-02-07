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
#include "vibeosc.h"
#include "sendmalletcoords.h"
#include "streamrecorder.h"


int setColor(int argc, char* argv[]){

    const char* oniFile= "/Users/fortjay81/Projects/VibeTrak/vibe_test/all_keys.oni";
    OniStream stream(oniFile);

    ColorDetector cd;

    cd.setColorValues(stream);

    return 0;
}



int startApp(int argc, char *argv[]){

    QApplication a(argc, argv);
    QApplication::setApplicationName("VibeTrak");

    bool record = false;

    ColorDetector* cd = new ColorDetector;
    cd->setColorValues(145, 179, 100, 255, 100, 255, 100);

    SendMalletCoords* coords = new SendMalletCoords;

     StreamRecorder* recorder;
    if (record)
        recorder = new StreamRecorder;

    OniStream* stream = new OniStream;
    stream->addProcessor(cd);
    stream->addProcessor(coords);
    if (record) stream->addProcessor(recorder);

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
                stream->addProcessor(cd);
                stream->addProcessor(coords);
                if (record) stream->addProcessor(recorder);
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

int test(){

    VibeOsc* sender = new VibeOsc("127.0.0.1", 50505);
    sender->sendOsc("/address", "message", 1);

    return 0;
}

int main(int argc, char *argv[])
{

    return startApp(argc, argv);

//    return setColor(argc, argv);

//    return test();


}









//#include <opencv/cv.h>
//#include <opencv2/highgui.hpp>

//#include <iostream>
//#include <string>

//int main(int argc, char* argv[])
//{
//    // Load input video
//    cv::VideoCapture input_cap(0);
//    if (!input_cap.isOpened())
//    {
//        std::cout << "!!! Input video could not be opened" << std::endl;
//        return -1;
//    }

//    // Setup output video
//    cv::VideoWriter output_cap("/Users/fortjay81/Projects/VibeTrak/output.avi",
//                               input_cap.get(CV_CAP_PROP_FOURCC),
//                               input_cap.get(CV_CAP_PROP_FPS),
//                               cv::Size(input_cap.get(CV_CAP_PROP_FRAME_WIDTH), input_cap.get(CV_CAP_PROP_FRAME_HEIGHT)));

//    std::cout << input_cap.get(CV_CAP_PROP_FOURCC) << std::endl;
//    std::cout << input_cap.get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
//    std::cout << input_cap.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl << std::endl;

//    std::cout << input_cap.get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
//    std::cout << input_cap.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;



//    if (!output_cap.isOpened())
//    {
//        std::cout << "!!! Output video could not be opened" << std::endl;
//        return -1;
//    }

//    // Loop to read frames from the input capture and write it to the output capture
//    cv::Mat frame;
//    int i = 0;
//    while (true)
//    {

//        input_cap >> frame;
//        if (i==300)
//            break;

//        output_cap.write(frame);
//        i++;
//    }

//    // Release capture interfaces
//    input_cap.release();
////    output_cap.release();

//    return 0;
//}

