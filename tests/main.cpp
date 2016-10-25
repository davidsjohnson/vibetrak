#define BOOST_TEST_MODULE VibeTest
#include <boost/test/included/unit_test.hpp>

#include "webcamstream.h"
#include "onistream.h"
#include "vibeframe.h"


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

    cv::Mat frame;
    for (int i = 0; i < 100; i++)
    {
        BOOST_CHECK(oni.next(frame));
        BOOST_CHECK(frame.rows > 0);
    }
}

BOOST_AUTO_TEST_CASE(frame_test)
{
//    OniStream oni;
    OniStream oni("/Users/fortjay81/Projects/build-VibeTrak-Desktop_Qt_5_7_0_clang_64bit-Debug/onistream_test.oni");
    BOOST_REQUIRE(oni.isOpened());

    cv::Mat depthFrame;
    cv::Mat colorFrame;

    for (int i = 0; i < 10; ++i)
    {
        oni.next(depthFrame, colorFrame);
    }

    BOOST_REQUIRE(depthFrame.size > 0);
    BOOST_REQUIRE(colorFrame.size > 0);

    VibeFrame frame(depthFrame, colorFrame);
    BOOST_REQUIRE(frame.depthFrame.rows > 0);
    BOOST_REQUIRE(frame.colorFrame.rows > 0);
    BOOST_CHECK_EQUAL(frame.depthFrame.rows, frame.colorFrame.rows);
    BOOST_CHECK_EQUAL(frame.depthFrame.cols, frame.colorFrame.cols);

}
