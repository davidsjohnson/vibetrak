#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include <opencv/cv.h>

#include "videostream.h"
#include "vibeframe.h"
#include "frameprocessor.h"

class ColorDetector : public FrameProcessor
{
public:
    ColorDetector();
    ~ColorDetector(){}

    void run(VibeFrame& frame);
    void setColorValues(VideoStream& colorStream);
    void setColorValues(int lowH, int highH, int lowS, int highS, int lowV, int highV, int thresh);


private:
    // Red HSV Values
    int m_lowH;
    int m_highH;

    int m_lowS;
    int m_highS;

    int m_lowV;
    int m_highV;

    int m_thresh;

    vector<cv::Rect> findBoundingBoxes(cv::Mat src);
};

#endif // COLORDETECTOR_H
