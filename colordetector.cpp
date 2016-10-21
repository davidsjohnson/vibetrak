#include "colordetector.h"
#include "videostream.h"

#include <opencv2/imgproc.hpp>

ColorDetector::ColorDetector():
    m_lowH(0),  m_lowS(0),  m_lowV(0),
    m_highH(179), m_highS(255),  m_highV(255)
{
}

void ColorDetector::setColorValues(VideoStream& colorStream)
{
    cv::namedWindow("HSV Control");
    cv::createTrackbar("Set H Low", "HSV Control", &m_lowH, 179);
    cv::createTrackbar("Set H High", "HSV Control", &m_highH, 179);

    cv::createTrackbar("Set S Low", "HSV Control", &m_lowS, 255);
    cv::createTrackbar("Set S High", "HSV Control", &m_highS, 255);

    cv::createTrackbar("Set V Low", "HSV Control", &m_lowV, 255);
    cv::createTrackbar("Set V High", "HSV Control", &m_highV, 255);

    while(true){
        cv::Mat img;
        colorStream.next(img);

        run(img, img);

        cv::imshow("Threshold Values", img);
        if (cv::waitKey(30) >= 0) break;
    }

    printf("Hue:\n\tLow: %d\n\tHigh: %d\n", m_lowH, m_highH);
    printf("Saturation:\n\tLow: %d\n\tHigh: %d\n", m_lowS, m_highS);
    printf("Value:\n\tLow: %d\n\tHigh: %d\n", m_lowV, m_highV);
}

void ColorDetector::setColorValues(int lowH, int highH, int lowS, int highS, int lowV, int highV)
{
    m_lowH=lowH;
    m_highH=highH;
    m_lowS=lowS;
    m_highS=highS;
    m_lowV=lowV;
    m_highV=highV;
}

void ColorDetector::run(const cv::Mat& src, cv::Mat& dst)
{
    // Check image is BGR
    if (src.type() != CV_8UC3)
        throw std::invalid_argument("src shoud be a BGR Image");

    // Convert image to HSV
    cv::cvtColor(src, dst, cv::COLOR_BGR2HSV);

    // threshold the images with HSV values
    cv::inRange(dst, cv::Scalar(m_lowH, m_lowS, m_lowV),  cv::Scalar(m_highH, m_highS, m_highV), dst);

    // Morphological opening and closing to reduce noise
    int kernelSize = 7;
    cv::Mat kernel(kernelSize, kernelSize, CV_8U, 1);
    cv::morphologyEx(dst, dst, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(dst, dst, cv::MORPH_CLOSE, kernel);

    // returns binary image indicated location of specific color
}
