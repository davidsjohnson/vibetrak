#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include <opencv/cv.h>

#include "videostream.h"

class ColorDetector
{
public:
    ColorDetector();

    void run(const cv::Mat& src, cv::Mat& dst);
    void setColorValues(VideoStream& colorStream);
    void setColorValues(int lowH, int highH, int lowS, int highS, int lowV, int highV);

private:
    // Red HSV Values
    int m_lowH;
    int m_highH;

    int m_lowS;
    int m_highS;

    int m_lowV;
    int m_highV;
};

#endif // COLORDETECTOR_H
