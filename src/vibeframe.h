#ifndef VIBEFRAME_H
#define VIBEFRAME_H

#include <vector>

#include <OpenNI.h>
#include <opencv/cv.h>

using namespace std;

typedef uint16_t depth_type;

class Point3d
{
public:
    float x;
    float y;
    float z;

    Point3d():
        x(0), y(0), z(0)
    {
    }

    Point3d(int x, int y, int z):
        x(x), y(y), z(z)
    {
    }
};


class VibeFrame
{
public:
    VibeFrame();
    VibeFrame(cv::Mat& depthFrame, cv::Mat& colorFrame);

    cv::Mat depthFrame;
    cv::Mat colorFrame;
    vector<cv::Rect> mallets;

    vector<Point3d> getMalletLocations();

private:

};

#endif // VIBEFRAME_H
