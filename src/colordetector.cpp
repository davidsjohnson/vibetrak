#include "colordetector.h"
#include "videostream.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

ColorDetector::ColorDetector():
    m_lowH(0), m_highH(179),
    m_lowS(0), m_highS(255),
    m_lowV(0), m_highV(255),
    m_thresh(100)
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
    cv::createTrackbar("Canny Threshold", "HSV Control", &m_thresh,255);

    while(true){
        VibeFrame frame;
        colorStream.next(frame);

        if (!frame.colorFrame.empty()){
            run(frame);
            cv::imshow("Threshold Values", frame.colorFrame);
            if (cv::waitKey(1) >= 0) break;
        }
    }

    printf("Hue:\n\tLow: %d\n\tHigh: %d\n", m_lowH, m_highH);
    printf("Saturation:\n\tLow: %d\n\tHigh: %d\n", m_lowS, m_highS);
    printf("Value:\n\tLow: %d\n\tHigh: %d\n", m_lowV, m_highV);
}

void ColorDetector::setColorValues(int lowH, int highH, int lowS, int highS, int lowV, int highV, int thresh)
{
    m_lowH=lowH;
    m_highH=highH;
    m_lowS=lowS;
    m_highS=highS;
    m_lowV=lowV;
    m_highV=highV;
    m_thresh=thresh;
}

void ColorDetector::run(VibeFrame& frame)
{
    // Check image is BGR
    if (!frame.colorFrame.empty() )
    {
        if (frame.colorFrame.type() != CV_8UC3)
            throw std::invalid_argument("src shoud be a BGR Image");

        // Convert image to HSV
        cv::Mat dst;
        cv::cvtColor(frame.colorFrame, dst, cv::COLOR_BGR2HSV);

        // find pixels in the range of HSV values
        cv::inRange(dst, cv::Scalar(m_lowH, m_lowS, m_lowV),  cv::Scalar(m_highH, m_highS, m_highV), dst);

        // Morphological opening and closing to reduce noise
        int kernelSize = 5;
        cv::Mat kernel(kernelSize, kernelSize, CV_8U, 1);
        cv::morphologyEx(dst, dst, cv::MORPH_OPEN, kernel);
        cv::morphologyEx(dst, dst, cv::MORPH_CLOSE, kernel);

        frame.mallets = findBoundingBoxes(dst);
    }

}

vector<cv::Rect> ColorDetector::findBoundingBoxes(cv::Mat src)
{
    // Find the Contours
    cv::Mat cannyOutput;
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;

    cv::Canny(src, cannyOutput, m_thresh, m_thresh*2);
    cv::findContours(cannyOutput, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    //Approximate Contours to Polys to find bounding boxes
    vector<vector<cv::Point>> contour_polys(contours.size());
    vector<cv::Rect> boundingRects(contours.size());
    for (unsigned int i = 0; i < contours.size(); ++i)
    {
        cv::approxPolyDP(cv::Mat(contours[i]), contour_polys[i], 3, true);
        boundingRects[i] = cv::boundingRect(cv::Mat(contour_polys[i]));
    }

    return boundingRects;
}
