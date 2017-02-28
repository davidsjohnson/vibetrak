#ifndef UTILS_H
#define UTILS_H

#include <iostream>

#include <opencv/cv.h>
#include <opencv2/imgproc.hpp>
#include <QImage>

#include "vibeframe.h"

// Value Obtained from Kinect v2
#define MIN_PIXEL_VALUE 0
#define MAX_PIXEL_VALUE 10000

namespace utils {

    void depth2Gray(const cv::Mat& src, cv::Mat& dst);
    QImage Mat2QImage(const cv::Mat& src);
    Point3d kinect2realworld(Point3d kinectPoint);
    Point3d realworld2Kinect(Point3d realworld);
    QImage Depth2QImage(const cv::Mat& src);

}

#endif // UTILS_H
