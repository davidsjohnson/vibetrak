#include "streamwidget.h"
#include "utils.h"
#include "annotationwidget.h"

#include <iostream>

#include <QWidget>
#include <QVBoxLayout>
#include <QPixmap>
#include <QImage>
#include <QTimer>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QMenuBar>
#include <QMessageBox>

using namespace std;

StreamWidget::StreamWidget(VideoStream* stream, QWidget *parent) :
  QWidget(parent),
  m_lFrame(new QLabel()),
  m_stream(stream),
  m_timer(new QTimer(this))
{

    int x = 0;
    int y = 0;
    int width = 512;
    int height = 424;
    setGeometry(x,y, width, height);
    setStyleSheet("background-color: #cccece");

    // #### initialize frame label with pixmap ####
    VibeFrame frame;
    m_stream->next(frame);


    QVBoxLayout* layout = new QVBoxLayout;
    cv::Mat temp(height, width, CV_8UC3, double(0));
    QImage img = utils::Mat2QImage(temp);
    QPixmap pMap = QPixmap::fromImage(img);
    m_lFrame->setPixmap(pMap);
    layout->addWidget(m_lFrame);


    QHBoxLayout* controls_layout = new QHBoxLayout;
    controls_layout->setObjectName(tr("Controls"));

    if (m_stream->isLive()){

        m_btnRecord = new QPushButton("Record", this);
        connect(m_btnRecord, SIGNAL (released()), this, SLOT (handleRecord()));
        controls_layout->addWidget(m_btnRecord);
    }

    layout->addLayout(controls_layout);

    setLayout(layout);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(run()));

   // #### Add Menu Item for Opening a Recording ####
    auto openAct = new QAction(tr("&Open Kinect Recording"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a previously created Kinect recording (ONI File)"));
    connect(openAct, SIGNAL (triggered()), this, SLOT (handleOpen()));

    auto openAnnotator = new QAction(tr("&Annotate Bars"), this);
    openAct->setShortcuts(QKeySequence::SelectAll);
    openAct->setStatusTip("Annotate the locations of each Vibraphone Bar");
    connect(openAnnotator, SIGNAL(triggered()), this, SLOT (handleOpenAnnotator()));

//    auto liveAct = new QAction(tr("&Connect to Kinect"), this);
//    liveAct->setShortcuts(QKeySequence::New);
//    liveAct->setStatusTip(tr("Open a live Kinect stream"));
//    connect(liveAct, SIGNAL (triggered()), this, SLOT (handleLive()));

    auto menuBar = new QMenuBar;
    auto fileMenu = menuBar->addMenu("File");
    fileMenu->addAction(openAct);
    fileMenu->addAction(openAnnotator);
//    fileMenu->addAction(liveAct);
}

//StreamWidget::~StreamWidget()
//{
//    delete m_stream;
//}


void StreamWidget::run()
{
    VibeFrame frame;
    m_stream->next(frame);

    QImage img = utils::Mat2QImage(frame.colorFrame);
    QPixmap pMap = QPixmap::fromImage(img);
    m_lFrame->setPixmap(pMap);
}


void StreamWidget::handleOpen()
{
    openOniFile();
    start();
}


void StreamWidget::handleOpenAnnotator()
{
    m_timer->stop();
    QWidget* annotator = new AnnotationWidget(m_stream, this);
    annotator->show();
}


void StreamWidget::handleLive()
{

    VideoStream* stream = new OniStream;

    while (!stream->isOpened()){

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
                auto temp = stream->getProcessors();
                delete stream;
                stream = new OniStream(filename.toUtf8());  // Open a new stream with the same processors
                stream->setProcessors(temp);
            }
            break;
        case QMessageBox::Retry:
            delete stream;
            stream = new OniStream;
            break;
        case QMessageBox::No:
        default:
            delete stream;
            stream = m_stream;
            break;
        }
    }

    m_stream = stream;
}


void StreamWidget::handleRecord()
{
    if (m_stream->isLive() && !m_stream->isRecording()){
        auto filename = QFileDialog::getSaveFileName(this, tr("Open Oni Stream"),  QDir::home().path(), tr("OpenNI Files (*.oni)"));
        if (filename != ""){
            m_stream->record(filename.toStdString());
            m_btnRecord->setText(tr("Stop &Recording"));
            setStyleSheet("QWidget{background-color: #7cd691;} QPushButton{background-color: #cccece;}");
        }
    }
    else if (m_stream->isLive() && m_stream->isRecording()){
        m_stream->stopRecording();
        m_btnRecord->setText(tr("Record"));
        setStyleSheet("background-color: #cccece");
    }
}


void StreamWidget::start()
{
    m_timer->start(1);
}

void StreamWidget::stop()
{
    m_timer->stop();
}


void StreamWidget::openOniFile()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Open Oni Stream"),  QDir::home().path(), tr("OpenNI Files (*.oni)"));
    if (filename!=""){
        if (m_stream){
            auto temp = m_stream;
            m_stream = new OniStream(filename.toUtf8());
            m_stream->setProcessors(temp->getProcessors());
            delete temp;
        }
        else{
           m_stream = new OniStream(filename.toUtf8());
        }

        if (m_btnRecord){
            QHBoxLayout* layout = findChild<QHBoxLayout*>("Controls");
            layout->removeWidget(m_btnRecord);
        }

    }
}

void StreamWidget::closing()
{
    delete m_stream;
}
