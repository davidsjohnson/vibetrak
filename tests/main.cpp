#define BOOST_TEST_MODULE VibeTest
#include <boost/test/included/unit_test.hpp>

#include "webcamstream.h"
#include "onistream.h"


BOOST_AUTO_TEST_CASE(webcam_test)
{
    WebcamStream webcam;
    BOOST_CHECK(webcam.isLive());
    BOOST_REQUIRE(webcam.isOpened());

    cv::Mat frame;
    for (int i = 0; i < 100; ++i)
    {
        BOOST_CHECK(webcam.next(frame));
        BOOST_CHECK(frame.size > 0);
    }
}

BOOST_AUTO_TEST_CASE(oni_test)
{
    OniStream oni("/Users/fortjay81/Projects/build-VibeTrak-Desktop_Qt_5_7_0_clang_64bit-Debug/onistream_test.oni");
    BOOST_CHECK(oni.isLive());
    BOOST_REQUIRE(oni.isOpened());

    cv::Mat frame;
    for (int i = 0; i < 100; i++)
    {
        BOOST_CHECK(oni.next(frame));
        BOOST_CHECK(frame.size > 0);
    }
}
