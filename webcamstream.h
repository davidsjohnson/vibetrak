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

    bool next(cv::Mat& frame);
    bool previous(cv::Mat& frame);

    void play();

protected:

private:
    cv::VideoCapture m_capture;
};

#endif // WEBCAMSTREAM_H
