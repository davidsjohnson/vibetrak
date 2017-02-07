#include "annotationwidget.h"
#include "utils.h"

AnnotationWidget::AnnotationWidget(VideoStream* stream, StreamWidget* main, QWidget* parent) :
    QWidget(parent),
    m_main(main),
    m_stream(stream)

{
    int x = 0;
    int y = 0;
    int width = 512;
    int height = 500;
    setGeometry(x, y, height, width);

    // #### initialize frame label with pixmap ####
    VibeFrame frame;
    m_stream->next(frame);

    // #### Setup UI Layout ####
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // Top Bar
    QHBoxLayout* topBarLayout = new QHBoxLayout;
    QPushButton* nextButton = new QPushButton("Next Frame");
    QLabel* instructLabel = new QLabel("Select Center of all F bars and F#_3 and D#_6");

    topBarLayout->addWidget(instructLabel);
    topBarLayout->addWidget(nextButton);

    connect(nextButton, SIGNAL(released()), this, SLOT(nextFrameCallback()));

    mainLayout->addLayout(topBarLayout);

    // Center Image
    QLabel* frameLabel = new QLabel;

    QImage img = utils::Mat2QImage(frame.colorFrame);
    QPixmap pMap = QPixmap::fromImage(img);
    frameLabel->setPixmap(pMap);
    frameLabel->setObjectName("frameLabel");
    mainLayout->addWidget(frameLabel);

    // Bottom Bar
    QHBoxLayout* bottomBarLayout = new QHBoxLayout;
    QPushButton* closeButton = new QPushButton("Close");
    QPushButton* sendButton = new QPushButton("Send Coordinates to Server");
    QPushButton* viewButton = new QPushButton("View Bar Centers");

    bottomBarLayout->addWidget(closeButton);
    bottomBarLayout->addWidget(sendButton);
    bottomBarLayout->addWidget(viewButton);

    connect(closeButton, SIGNAL(released()), this, SLOT(closeCallback()));
    connect(sendButton, SIGNAL(released()), this, SLOT(sendCallback()));
    connect(viewButton, SIGNAL(released()), this, SLOT(viewCallback()));

    mainLayout->addLayout(bottomBarLayout);

}


// #### Button Callbacks ####
void AnnotationWidget::nextFrameCallback(){

    QLabel* frameLabel = findChild<QLabel*>("frameLabel");

    VibeFrame frame;
    m_stream->next(frame);

    QImage img = utils::Mat2QImage(frame.colorFrame);
    QPixmap pMap = QPixmap::fromImage(img);
    frameLabel->setPixmap(pMap);

}

void AnnotationWidget::closeEvent(QCloseEvent* event){
    closeCallback();
}

void AnnotationWidget::closeCallback(){
    close();
    m_main->start();
}


void AnnotationWidget::sendCallback(){

}


void AnnotationWidget::viewCallback(){

}


