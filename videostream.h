#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

class VideoStream
{
public:

    virtual bool isLive() = 0;
    virtual bool isOpened() = 0;

    virtual bool next(cv::Mat& frame) = 0;
    virtual bool previous(cv::Mat& frame) = 0;
};

#endif // VIDEOSTREAM_H
