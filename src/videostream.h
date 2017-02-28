#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <QObject>

#include "vibeframe.h"
#include "frameprocessor.h"

class VideoStream : public QObject
{
    Q_OBJECT

public:

    VideoStream(QObject* parent): QObject(parent){}

    virtual ~VideoStream() = default;

    virtual bool isLive() = 0;

    bool isOpened() {return m_opened;}
    bool isRecording() {return m_recording;}

    virtual bool next(VibeFrame& frame) = 0;
    virtual bool previous(VibeFrame& frame) = 0;
    virtual void record(string filename) = 0;
    virtual void stopRecording() = 0;

    void addProcessor(FrameProcessor* proc) {m_processors.push_back(proc);}
    std::vector<FrameProcessor*> getProcessors(){return m_processors;}
    void setProcessors(std::vector<FrameProcessor*> processors){m_processors = vector<FrameProcessor*>(processors);}



protected:
    bool                    m_opened;
    bool                    m_recording;
    std::vector<FrameProcessor*> m_processors;
};

#endif // VIDEOSTREAM_H
