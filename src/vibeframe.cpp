#include "vibeframe.h"


VibeFrame::VibeFrame():
    depthFrame(), colorFrame()
{
}

VibeFrame::VibeFrame(cv::Mat& depthFrame, cv::Mat& colorFrame):
    depthFrame(depthFrame), colorFrame(colorFrame)
{

}


vector<Point3d> VibeFrame::getMalletLocations()
{
    vector<Point3d> locations;

    for (cv::Rect r : mallets)
    {
        Point3d p;
        p.x = r.x + (r.width/2);
        p.y = r.y + (r.height/2);
        p.z = depthFrame.at<int>(cv::Point(p.x, p.y));
        locations.push_back(p);
    }
}
