#include <iostream>

#include "annotationwidget.h"
#include "utils.h"

/**
  Constructor for AnnotationWidget
  @param stream the VideoStream being annotated
  @param main the main Window of the application
  @param parent the parent widget; 0 by default
*/
AnnotationWidget::AnnotationWidget(VideoStream* stream, StreamWidget* main, QWidget* parent) :
    QWidget(parent),
    m_stream(stream),
    m_main(main),
    redCircles(RED_CIRCLES),
    blueCircles(BLU_CIRCLES),
    m_vibeosc("127.0.0.1", 10103)
{
    int x = 0;
    int y = 0;
    int width = 600;
    int height = 500;
    setGeometry(x, y, width, height);

    // #### initialize frame label with pixmap ####
    m_stream->next(m_frame);

    // #### Setup UI Layout ####
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // Top Bar
    QHBoxLayout* topBarLayout = new QHBoxLayout;
    QPushButton* nextButton = new QPushButton("Next Frame");
    QLabel* instructLabel = new QLabel("Select Center of all F bars with Red Circles\nand F#_3 and D#_6 with Blue Circles");

    topBarLayout->addWidget(instructLabel);
    topBarLayout->addWidget(nextButton);

    connect(nextButton, SIGNAL(released()), this, SLOT(nextFrameCallback()));

    mainLayout->addLayout(topBarLayout);

    // Center Image
    frameGfxScene = new QGraphicsScene;
    frameGfxScene->setSceneRect(0,0,512,424);
    QGraphicsView* frameView = new QGraphicsView(frameGfxScene);
    m_pMapItem = new QGraphicsPixmapItem;
    frameGfxScene->addItem(m_pMapItem);

    mainLayout->addWidget(frameView);
    nextFrameCallback();
    addAnnotationCircles();

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

void AnnotationWidget::addAnnotationCircles()
{
    for (int i=0; i<RED_CIRCLES; ++i)
    {
        redCircles[i] = new LocationCircle;
        redCircles[i]->setParentItem(m_pMapItem);
        redCircles[i]->setPos(QPointF(i*15, 40));
    }

    for (int i=0; i<BLU_CIRCLES; ++i)
    {
        blueCircles[i] = new LocationCircle(10,10,QColor(0,0,255));
        blueCircles[i]->setParentItem(m_pMapItem);
        blueCircles[i]->setPos(QPointF(i*15, 80));
    }
}


// #### Button Callbacks ####
void AnnotationWidget::nextFrameCallback(){

//    QLabel* frameLabel = findChild<QLabel*>("frameLabel");

    m_stream->next(m_frame);

    QImage img = utils::Mat2QImage(m_frame.colorFrame);
    QPixmap pMap = QPixmap::fromImage(img);
    m_pMapItem->setPixmap(pMap);
//    frameLabel->setPixmap(pMap);

}


void AnnotationWidget::closeEvent(QCloseEvent* event){
    closeCallback();
}


void AnnotationWidget::closeCallback(){
    close();
    m_main->start();
}


void AnnotationWidget::sendCallback(){

    // Send Bar locations via OSC
    std::cout << "\tSending Bar Locations: " << m_barLocations.size() << std::endl;
    for(int i = 0; i < m_barLocations.size(); ++i){

        Point3d rw = utils::kinect2realworld(m_barLocations[i]);
        qreal z = m_frame.depthFrame.at<uint16_t>(rw.y, rw.z);

        std::stringstream address;
        address << "/bars/" << i;
        m_vibeosc.sendOsc(address.str().c_str(), rw.x, rw.y, z);
    }
}


void AnnotationWidget::viewCallback(){

    for (auto c : greenCircles){
        frameGfxScene->removeItem(c);
    }

    m_barLocations.clear();
    greenCircles.clear();
    calculateBarLocations(m_barLocations);    // Calculate Bar Locations based on Current locations of given circles
    drawBarLocations(m_barLocations);         // Draw Circles representing bar locations
}


void AnnotationWidget::calculateBarLocations(vector<Point3d>& barLocations){

    auto sortFunc = [](const LocationCircle* a, const LocationCircle* b){ return a->x() < b->x(); };

    std::sort(redCircles.begin(), redCircles.end(), sortFunc);
    std::sort(blueCircles.begin(), blueCircles.end(), sortFunc);

    // Add location of White Keys to list
    for(int i=0;i < RED_CIRCLES-1; ++i){

        // find coordinates of the center of first circle used to interpolate locations
        qreal x1 = redCircles[i]->x() + (redCircles[i]->width() / 2);
        qreal y1 = redCircles[i]->y() + (redCircles[i]->height() / 2);
        qreal z1 = m_frame.depthFrame.at<depth_type>(y1, x1);

        // find coordinates of the center of second circle used to interpolate locations
        qreal x2 = redCircles[i+1]->x() + (redCircles[i+1]->width() / 2);
        qreal y2 = redCircles[i+1]->y() + (redCircles[i+1]->height() / 2);
        qreal z2 = m_frame.depthFrame.at<depth_type>(y2, x2);

        // convert points to real world data space
        auto temp1 = utils::kinect2realworld( Point3d(x1, y1, z1) );
        auto temp2 = utils::kinect2realworld( Point3d(x2, y2, z2) );

        // Find the distance between the centers of each bar
        qreal XDist = temp2.x - temp1.x;
        qreal YDist = temp2.y - temp1.y;
        qreal ZDist = temp2.z - temp1.z;
        qreal newXDist = XDist / 7.0;
        qreal newYDist = YDist / 7.0;
        qreal newZDist = ZDist / 7.0;

        // Add locations for this segment of the white keys
        barLocations.push_back(Point3d(x1, y1, z1));

        for(int j=0; j < 6; ++j){
            qreal newX = temp1.x+newXDist*(j+1) - (redCircles[i]->width() / 2);
            qreal newY = temp1.y+newYDist*(j+1) - (redCircles[i]->height() / 2);
            qreal newZ = temp1.z+newZDist*(j+1);
            barLocations.push_back(utils::realworld2Kinect(Point3d(newX, newY, newZ)));
        }
    }
    qreal x = redCircles[RED_CIRCLES-1]->x() + (redCircles[RED_CIRCLES-1]->width() / 2);
    qreal y = redCircles[RED_CIRCLES-1]->y() + (redCircles[RED_CIRCLES-1]->height() / 2);
    qreal z = m_frame.depthFrame.at<depth_type>(y, x);
    barLocations.push_back(Point3d(x, y, z));

    // Add location of Black Keys to list
    for(int i=0;i < BLU_CIRCLES-1; i+=2){

        // find coordinates of the center of first circle used to interpolate locations
        qreal x1 = blueCircles[i]->x() + blueCircles[i]->width() / 2;
        qreal y1 = blueCircles[i]->y() + blueCircles[i]->height() / 2;
        qreal z1 = m_frame.depthFrame.at<depth_type>(y1, x1);

        // find coordinates of the center of second circle used to interpolate locations
        qreal x2 = blueCircles[i+1]->x() + blueCircles[i+1]->width() / 2;
        qreal y2 = blueCircles[i+1]->y() + blueCircles[i+1]->height() / 2;
        qreal z2 = m_frame.depthFrame.at<depth_type>(y2, x2);

        // Convert to real world space
        auto temp1 = utils::kinect2realworld( Point3d(x1, y1, z1) );
        auto temp2 = utils::kinect2realworld( Point3d(x2, y2, z2) );

        qreal XDist = temp2.x - temp1.x;
        qreal YDist = temp2.y - temp1.y;
        qreal ZDist = temp2.z - temp1.z;
        qreal newXDist = XDist / 5.0;
        qreal newYDist = YDist / 5.0;
        qreal newZDist = ZDist / 5.0;

        // Add locations for this segment of the black keys
        barLocations.push_back(Point3d(x1, y1, z1));
        for(int j=0; j < 4; ++j){

            if (j != 2){
                qreal newX = temp1.x+newXDist*(j+1) - blueCircles[i]->width() / 2;
                qreal newY = temp1.y+newYDist*(j+1) - blueCircles[i]->height() / 2;
                qreal newZ = temp1.z+newZDist*(j+1);
                barLocations.push_back(utils::realworld2Kinect(Point3d(newX, newY, newZ)));
            }
        }
        barLocations.push_back(Point3d(x2, y2, z2));
    }
}


void AnnotationWidget::drawBarLocations(const vector<Point3d>& barLocations){

    for (auto p : barLocations)
    {
        LocationCircle* circle = new LocationCircle(CIRCLE_RADIUS, CIRCLE_RADIUS, QColor(0,255,0));
        circle->setParentItem(m_pMapItem);
        circle->setPos(QPointF(p.x - CIRCLE_RADIUS/2, p.y - CIRCLE_RADIUS/2));
        greenCircles.push_back(circle);
    }
}
