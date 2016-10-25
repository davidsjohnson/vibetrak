#ifndef VIBEFRAME_H
#define VIBEFRAME_H

#include <OpenNI.h>
#include <opencv/cv.h>


class VibeFrame
{
public:
    VibeFrame(cv::Mat& depthFrame, cv::Mat& colorFrame);


    cv::Mat depthFrame;
    cv::Mat colorFrame;


private:

};

#endif // VIBEFRAME_H
