#define BOOST_TEST_MODULE VibeTest
#include <boost/test/included/unit_test.hpp>

#include "webcamstream.h"
#include "onistream.h"
#include "vibeframe.h"
#include "colordetector.h"
#include "vibeosc.h"


//BOOST_AUTO_TEST_CASE(webcam_test)
//{
//    WebcamStream webcam;
//    BOOST_CHECK(webcam.isLive());
//    BOOST_REQUIRE(webcam.isOpened());

//    cv::Mat frame;
//    for (int i = 0; i < 100; ++i)
//    {
//        BOOST_CHECK(webcam.next(frame));
//        BOOST_CHECK(frame.size > 0);
//    }
//}

BOOST_AUTO_TEST_CASE(oni_test)
{
    OniStream oni("/Users/fortjay81/Projects/build-VibeTrak-Desktop_Qt_5_7_0_clang_64bit-Debug/onistream_test.oni");
    BOOST_CHECK(oni.isLive());
    BOOST_REQUIRE(oni.isOpened());

    VibeFrame frame;
    for (int i = 0; i < 100; i++)
    {
        BOOST_CHECK(oni.next(frame));
        if (i > 10)
        {
            BOOST_CHECK(frame.colorFrame.rows > 0);
        }
    }
}

BOOST_AUTO_TEST_CASE(frame_test)
{
//    OniStream oni;
    OniStream oni("/Users/fortjay81/Projects/build-VibeTrak-Desktop_Qt_5_7_0_clang_64bit-Debug/onistream_test.oni");
    ColorDetector cd;
    cd.setColorValues(110, 130, 50, 255, 50, 255, 100); //magic numbers from running set color values with webcam...

    BOOST_REQUIRE(oni.isOpened());

    VibeFrame frame;

    for (int i = 0; i < 100; ++i)
    {
        oni.next(frame);
        cd.run(frame);
    }

    BOOST_REQUIRE(frame.depthFrame.size > 0);
    BOOST_REQUIRE(frame.colorFrame.size > 0);

    BOOST_CHECK_EQUAL(frame.depthFrame.rows, frame.colorFrame.rows);
    BOOST_CHECK_EQUAL(frame.depthFrame.cols, frame.colorFrame.cols);

    BOOST_CHECK(frame.mallets.size() > 0);

}

BOOST_AUTO_TEST_CASE(osc_test)
{
    VibeOsc* sender = new VibeOsc("127.0.0.1", 50000);
    BOOST_REQUIRE(sender!=nullptr);
    sender->sendOsc("/address", "message", 1, 5, 10, 6.688);
}
