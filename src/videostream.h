#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "vibeframe.h"

class VideoStream
{
public:

    virtual bool isLive() = 0;
    virtual bool isOpened() = 0;

    virtual bool next(VibeFrame& frame) = 0;
    virtual bool previous(VibeFrame& frame) = 0;
};

#endif // VIDEOSTREAM_H
