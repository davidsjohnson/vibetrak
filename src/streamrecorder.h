#ifndef STREAMRECORDER_H
#define STREAMRECORDER_H

#include <iostream> // for standard I/O
#include <string>   // for strings

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write

#include "frameprocessor.h"
#include "vibeframe.h"


class StreamRecorder : public FrameProcessor
{

    cv::VideoWriter m_VideoOutput;

public:
    StreamRecorder();
    ~StreamRecorder(){ m_VideoOutput.release(); printf("Recorder Closed");}
    void run(VibeFrame& frame);
};

#endif // STREAMRECORDER_H
