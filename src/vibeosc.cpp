#include "vibeosc.h"

VibeOsc::VibeOsc(const char* ipAddress, int port):
    m_transmitSocket( IpEndpointName(ipAddress, port) )
{
}


//template<typename T, typename... Ts>
//void VibeOsc::buildPacket(osc::OutboundPacketStream& p, T arg, Ts... args)
//{
//    p << arg;
//    buildPacket(p, args...);
//}


//template<typename T, typename... Ts>
//void VibeOsc::sendOsc(const char* address, T arg, Ts... args)
//{

//    char buffer[OUTPUT_BUFFER_SIZE];
//    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

//    p << osc::BeginMessage( address );
//    buildPacket(p, arg, args...);
//    p << osc::EndMessage;

//    m_transmitSocket.Send( p.Data(), p.Size() );
//}
