#ifndef UTILS_H
#define UTILS_H

#include <opencv/cv.h>
#include <opencv2/imgproc.hpp>
#include <QImage>

// Value Obtained from Kinect v2
#define MIN_PIXEL_VALUE 0
#define MAX_PIXEL_VALUE 10000

void depth2Gray(const cv::Mat& src, cv::Mat& dst)
{
    cv::normalize(src, dst, 0, 255, cv::NORM_MINMAX, -1);
}

QImage Mat2QImage(const cv::Mat& src)
{
    if (src.type() == CV_8UC3)
    {
        QImage dst = QImage(src.data, src.cols, src.rows,  static_cast<int>(src.step), QImage::Format_RGB888);
        return dst.rgbSwapped();
    }
    else
    {
        return QImage(512, 480, QImage::Format_RGB888);
    }
}

#endif // UTILS_H
