#ifndef SENDMALLETCOORDS_H
#define SENDMALLETCOORDS_H

#include "vibeframe.h"
#include "frameprocessor.h"
#include "vibeosc.h"

class SendMalletCoords : public FrameProcessor
{
public:
    SendMalletCoords();
    ~SendMalletCoords(){}
    void run(VibeFrame& frame);

private:

    VibeOsc m_vibeosc;
};

#endif // SENDMALLETCOORDS_H
