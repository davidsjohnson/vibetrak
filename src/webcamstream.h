#ifndef WEBCAMSTREAM_H
#define WEBCAMSTREAM_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "videostream.h"

class WebcamStream : public VideoStream
{
public:
    WebcamStream();

    bool isLive();
    bool isOpened();

    bool next(VibeFrame& frame);
    bool previous(VibeFrame& frame);

    void play();

protected:

private:
    cv::VideoCapture m_capture;
};

#endif // WEBCAMSTREAM_H
