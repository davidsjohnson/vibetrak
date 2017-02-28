#ifndef ONISTREAM_H
#define ONISTREAM_H

#include <QObject>

#include <OpenNI.h>

#include "videostream.h"
#include "frameprocessor.h"

class OniStream : public VideoStream
{
    Q_OBJECT
public:
    OniStream(QObject* parent=0);
    OniStream(const char* oniFile, QObject* parent=0);
    ~OniStream();

    bool isLive();

    bool next(VibeFrame& frame);
    bool previous(VibeFrame& frame);

    void record(string filename);
    void stopRecording();

protected:

private:

    openni::Status init(const char* oniFile=openni::ANY_DEVICE);
    openni::Status initRecorder(string filename);

    bool                    m_live;

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
