#include "webcamstream.h"

WebcamStream::WebcamStream():
    m_capture(0)
{

}

bool WebcamStream::isOpened()
{
    return m_capture.isOpened();
}


bool WebcamStream::isLive()
{
    return true;
}

void WebcamStream::play()
{
    while(true)
    {
        cv::Mat frame;
        m_capture >> frame;

        cv::imshow("Webcam", frame);
        if (cv::waitKey(20) >= 0) break;
    }
}

bool WebcamStream::next(cv::Mat& frame)
{
    m_capture >> frame;
    return true;
}

bool WebcamStream::previous(cv::Mat& frame)
{
    frame = NULL;\

    return true;
}
