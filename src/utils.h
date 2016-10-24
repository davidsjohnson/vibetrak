#ifndef UTILS_H
#define UTILS_H

#include <opencv/cv.h>
#include <opencv2/imgproc.hpp>

// Value Obtained from Kinect v2
#define MIN_PIXEL_VALUE 0
#define MAX_PIXEL_VALUE 10000

void depth2Gray(const cv::Mat& src, cv::Mat& dst)
{
    cv::normalize(src, dst, 0, 255, cv::NORM_MINMAX, -1);
}

#endif // UTILS_H
