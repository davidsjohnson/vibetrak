#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QObject>
#include <QMainWindow>

#include "onistream.h"
#include "colordetector.h"
#include "utils.h"
#include "streamwidget.h"
#include "sendmalletcoords.h"
#include "streamrecorder.h"

/*
 * Function used to obtain correct HSV Values for color tracking
 * TODO: Add this feature to interface for automatically setting values
 */
int setColor(int argc, char* argv[]){

    // Open a stream to use for color detection calibration
    // remove file from instaniate to use live kinect stream
    const char* oniFile= "/Users/fortjay81/Projects/VibeTrak/VibeTrak/vibe_test/all_keys.oni";
    OniStream stream(oniFile);

    ColorDetector cd;
    cd.setColorValues(stream);

    return 0;
}



int startApp(int argc, char *argv[]){

    bool record = false;    // Used to Record the final output of MalletTracker; should usually be off (false)

    // START QT APPLICATION
    QApplication a(argc, argv);
    QApplication::setApplicationName("VibeTrak");

    // Processor for detecting color from kinect stream; used for mallet tracking
    ColorDetector* cd = new ColorDetector;
    cd->setColorValues(130, 179, 95, 255, 80, 255, 100);    // Values determined manually (TODO:

    // Processor for sending the coordinates of each mallet via OSC
    SendMalletCoords* coords = new SendMalletCoords;

    // Processor to Record the final output of MalletTracker; should usually be off (false)
    StreamRecorder* recorder;
    if (record)
        recorder = new StreamRecorder;

    // Instaniate new Live Kinect Stream and add additional processors to the stream
    OniStream* stream = new OniStream;
    stream->addProcessor(cd);
    stream->addProcessor(coords);
    if (record) stream->addProcessor(recorder);


    // If stream does open successfully, then no Kinect is attached.
    // Prompt user to open a Kinect recording
    while (! stream->isOpened()){

        // Message Box
        int ret = QMessageBox::question(nullptr, QObject::tr("VibeTrak"),
                                   QObject::tr("No Kinect Detected.\n"
                                      "Do you want to open a Kinect recording?"),
                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Retry);

        // Message Box Results
        QString filename;
        switch(ret)
        {
        case QMessageBox::Yes:
            filename = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open Oni Stream"),  QDir::home().path(), QObject::tr("OpenNI Files (*.oni)"));
            if (filename!=""){
                // Instantiate new stream using the Kinect Recording file, should be type .ONI, and add processors
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
            // Instaniate new Live Kinect Stream and add additional processors to the stream
            stream = new OniStream;
            stream->addProcessor(cd);
            stream->addProcessor(coords);
            if (record) stream->addProcessor(recorder);
            break;
        default:
            return 1;
            break;
        }
    }

    // Start the stream display
    StreamWidget* streamWidget = new StreamWidget(stream);
    streamWidget->show();
    streamWidget->start();

    // Add callback for closing when clicking X icon
    QObject::connect(&a, SIGNAL(aboutToQuit()), streamWidget, SLOT(closing()));

    return a.exec();

}

int main(int argc, char *argv[])
{

    return startApp(argc, argv);  // Main Application
//    return setColor(argc, argv);  // Used to manually obtain correct HSV Values for color tracking

}
