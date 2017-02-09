#include "sendmalletcoords.h"
#include "utils.h"

#include <opencv/cv.h>
#include <opencv2/imgproc.hpp>

using namespace  utils;


SendMalletCoords::SendMalletCoords():
    m_vibeosc()
{
}


void SendMalletCoords::run(VibeFrame &frame)
{
    auto locations = frame.getMalletLocations();

    if (locations.size() == 4)   //only send data for frames with 4 mallets TODO - Find a better way to handle this....
    {

        std::stringstream startAddress;
        startAddress << "/mallets/start";
        m_vibeosc.sendOsc(startAddress.str().c_str(), 0);

        int i = 0;
        for (Point3d l : locations)
        {

            cv::circle(frame.depthFrame, cv::Point(l.x, l.y), 5, 255, 2);
            cv::circle(frame.colorFrame, cv::Point(l.x, l.y), 5, 255, 3);

            Point3d rw = kinect2realworld(l);

            std::stringstream address;
            address << "/mallets/" << i;
            m_vibeosc.sendOsc(address.str().c_str(), rw.x, rw.y, rw.z);
            i++;
        }

        std::stringstream endAddress;
        endAddress << "/mallets/end";
        m_vibeosc.sendOsc(endAddress.str().c_str(), 0);
    }
    else
    {
        throw NoMalletsException();
    }
}
