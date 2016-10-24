#ifndef ONISTREAM_H
#define ONISTREAM_H

#include <OpenNI.h>
#include <opencv/cv.h>

#include "videostream.h"

class OniStream : public VideoStream
{
public:
    OniStream();
    OniStream(const char* oniFile);
    ~OniStream();

    bool isLive();
    bool isOpened();

    bool next(cv::Mat &frame);
    bool next(cv::Mat& depthFrame, cv::Mat &colorFrame);
    bool previous(cv::Mat& frame);

    void record();
    void stopRecording();

protected:

private:

    openni::Status init(const char* oniFile=openni::ANY_DEVICE);
    openni::Status initRecorder();

    bool                    m_opened;
    openni::Device          m_device;
    openni::VideoStream     m_depthStream;
    openni::VideoStream     m_colorStream;
    openni::VideoStream**   m_streams;

    openni::Recorder        m_recorder;

    openni::VideoFrameRef   m_depthFrame;
    openni::VideoFrameRef   m_colorFrame;

    int                     m_depthWidth;
    int                     m_depthHeight;
    int                     m_colorWidth;
    int                     m_colorHeight;
};

#endif // ONISTREAM_H
