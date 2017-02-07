#include "utils.h"

//#include <opencv/cv.h>
//#include <opencv2/imgproc.hpp>
//#include <QImage>

//#include "vibeframe.h"

namespace utils {


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
        else if (src.type() == CV_16UC1)
        {
            cv::Rect r(0, 0, 512, 424);
            cv::Mat temp(512, 424, CV_8UC1);
            cv::normalize(src(r), temp, 0, 255, cv::NORM_MINMAX, CV_8UC1);
            QImage dst = QImage(temp.data, temp.cols, temp.rows, static_cast<int>(temp.step), QImage::Format_Grayscale8);
            return dst;
        }
        else
        {
            return QImage(512, 480, QImage::Format_RGB888);
        }
    }
}
