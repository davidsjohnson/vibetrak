#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include <opencv/cv.h>

#include "videostream.h"
#include "vibeframe.h"
#include "frameprocessor.h"

/*!
 * \brief The ColorDetector class
 */
class ColorDetector : public FrameProcessor
{
public:
    /*!
     * \brief ColorDetector initializes HSV Values for color detection
     */
    ColorDetector();

    ~ColorDetector(){}

    /*!
     * \brief run performs color detection on the current frame
     * \param frame VibeFrame to detect color and append mallet locations
     */
    void run(VibeFrame& frame);

    /*!
     * \brief setColorValues Run this function to find and set HSV values
     * \param colorStream
     */
    void setColorValues(VideoStream& colorStream);

    /*!
     * \brief setColorValues Enter HSV values
     * \param lowH min Hue
     * \param highH max Hue
     * \param lowS min Saturation
     * \param highS max Saturation
     * \param lowV min Value
     * \param highV max Value
     * \param thresh Canny Threshold
     */
    void setColorValues(int lowH, int highH, int lowS, int highS, int lowV, int highV, int thresh);


private:
    // HSV Values for thresholding
    int m_lowH;
    int m_highH;

    int m_lowS;
    int m_highS;

    int m_lowV;
    int m_highV;

    // Canny Threshold
    int m_thresh;

    /*!
     * \brief findBoundingBoxes finds the locations of mallets using canny edge detection
     * \param src cv::Mat containing the Thresholded image
     * \return returns a vector<cv::Rect> containing the bounding boxes of all objects found
     */
    vector<cv::Rect> findBoundingBoxes(cv::Mat src);
};

#endif // COLORDETECTOR_H
