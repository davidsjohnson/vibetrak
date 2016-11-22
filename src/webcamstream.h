#ifndef WEBCAMSTREAM_H
#define WEBCAMSTREAM_H

#include <QObject>

#include <opencv2/opencv.hpp>

#include "videostream.h"

class WebcamStream : public VideoStream
{
    Q_OBJECT
public:
    WebcamStream(VideoStream* parent=0);

    bool isLive();
    bool isOpened();

    bool next(VibeFrame& frame);
    bool previous(VibeFrame& frame);

//    void play();

protected:

private:
    cv::VideoCapture m_capture;
};

#endif // WEBCAMSTREAM_H
