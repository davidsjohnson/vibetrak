#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include "vibeframe.h"

class NoMalletsException : public std::exception{};


class FrameProcessor
{
public:
    virtual ~FrameProcessor(){}
    virtual void run(VibeFrame& frame){}
};

#endif // FRAMEPROCESSOR_H
