#ifndef OSC_H
#define OSC_H

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include <string>

#define OUTPUT_BUFFER_SIZE 1024

class VibeOsc
{
public:
    VibeOsc(const char*  ipAddress="127.0.0.1", int port=10101);

    template<typename T, typename... Ts>
    void sendOsc(const char* address, T arg, Ts... args)
    {
        char buffer[OUTPUT_BUFFER_SIZE];
        osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

        p << osc::BeginMessage( address );
        buildPacket(p, arg, args...);
        p << osc::EndMessage;

        m_transmitSocket.Send( p.Data(), p.Size() );
    }

private:

    UdpTransmitSocket m_transmitSocket;

    void buildPacket(osc::OutboundPacketStream &p){}

    template<typename T, typename... Ts>
    void buildPacket(osc::OutboundPacketStream& p, T arg, Ts... args)
    {
        p << arg;
        buildPacket(p, args...);
    }

};

#endif // OSC_H
