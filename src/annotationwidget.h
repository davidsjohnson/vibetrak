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

/*
 * AnnotationWidget is a child QT Widget for annotating the location
 * of the Vibraphone bars using the content from the VideoStream
 */

class AnnotationWidget : public QWidget
{

    Q_OBJECT

public:

    /*!
      \brief Constructor for AnnotationWidget
      \param stream the VideoStream being annotated
      \param main the main Window of the application
      \param parent the parent widget; 0 by default
    */
    explicit AnnotationWidget(VideoStream* stream, StreamWidget* main, QWidget* parent = 0);

private:
    VideoStream* m_stream;              ///< main video stream from main window
    StreamWidget* m_main;               ///< main window

    bool m_bDepthImage = false;

    QGraphicsPixmapItem* m_pMapItem;    ///< used to draw frame
    QGraphicsScene* frameGfxScene;      ///< Scene container for frame (needed when drawing circles)
    VibeFrame m_frame;                  ///< The current from from the stream

    VibeOsc m_vibeosc;                  ///< An OSC Client to send loctions

    vector<Point3d> m_barLocations;     ///< Location of all bars

    // Circles for drawing on dispay to annotate bars
    vector<LocationCircle*> redCircles;     // White Bars
    vector<LocationCircle*> blueCircles;    // Black Bars
    vector<LocationCircle*> greenCircles;   // calculated locations

    /*!
     * \brief addAnnotationCircles adds the initial annotation
     * cirlces to the current frame
     */
    void addAnnotationCircles();

    /*!
     * \brief calculateBarLocations calculates the location of all remaining bars
     * based on the location of the user placed circles
     * \param barLocations a vector<Point3d> to store new bar locations
     */
    void calculateBarLocations(vector<Point3d> &barLocations);

    /*!
     * \brief drawBarLocations draws circles at the given bar locations
     * \param barLocations a vector<Point3d) containing bar locations
     */
    void drawBarLocations(const vector<Point3d> &barLocations);
protected:

    /*!
     * \brief closeEvent callback triggered by application closing
     * \param event the event details
     */
    void closeEvent(QCloseEvent* event);

signals:


public slots:

    /*!
     * \brief nextFrameCallback handles the Next Button to move to the next frame
     */
    void nextFrameCallback();

    void depthFrameCallback();

    /*!
     * \brief closeCallback closes the widget and starts the main window
     */
    void closeCallback();

    /*!
     * \brief sendCallback handles the send button to send bar locations via OSC
     */
    void sendCallback();

    /*!
     * \brief viewCallback handles the view button to calculate and draw the
     * location of all bars based on user place circles
     */
    void viewCallback();

};

#endif // ANNOTATIONWIDGET_H
