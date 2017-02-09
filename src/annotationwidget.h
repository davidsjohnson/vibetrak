#ifndef ANNOTATIONWIDGET_H
#define ANNOTATIONWIDGET_H

#include <vector>
#include <algorithm>

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "videostream.h"
#include "streamwidget.h"
#include "locationcircle.h"
#include "vibeosc.h"

#define RED_CIRCLES 4
#define BLU_CIRCLES 6
#define CIRCLE_RADIUS 10

class AnnotationWidget : public QWidget
{

    Q_OBJECT

public:
    explicit AnnotationWidget(VideoStream* stream, StreamWidget* main, QWidget* parent = 0);

private:
    VideoStream* m_stream;
    StreamWidget* m_main;
    QGraphicsPixmapItem* m_pMapItem;
    QGraphicsScene* frameGfxScene;
    VibeFrame m_frame;

    VibeOsc m_vibeosc;

    vector<LocationCircle*> redCircles;
    vector<LocationCircle*> blueCircles;
    vector<Point3d> m_barLocations;
    vector<LocationCircle*> greenCircles;

    void addAnnotationCircles();

    void calculateBarLocations(vector<Point3d> &barLocations);
    void drawBarLocations(const vector<Point3d> &barLocations);
protected:
    void closeEvent(QCloseEvent* event);

signals:


public slots:
    void nextFrameCallback();
    void closeCallback();
    void sendCallback();
    void viewCallback();

};

#endif // ANNOTATIONWIDGET_H
