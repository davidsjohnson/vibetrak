#include <QCoreApplication>
#include <iostream>

#include "webcamstream.h"
#include "onistream.h"
#include "colordetector.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
//    return a.exec();


//    WebcamStream webcam;

//    OniStream oniStream;
//    OniStream oniStream("onistream.oni");

//    if (oniStream.isOpened())
//    {
//        printf("OniStream Opened Successfully\n");
//    }

//    if (oniStream.isOpened())
//    {
//        while (true)
//        {
//            cv::Mat depthFrame, colorFrame;
//            oniStream.next(depthFrame, colorFrame);

//            if (depthFrame.size != NULL)
//                cv::imshow("Depth", depthFrame);
//            if (colorFrame.data != NULL){
//                cv::Mat c(960, 540, CV_8UC3);
//                cv::resize(colorFrame, c, cv::Size(960, 540));
//                cv::imshow("Color", c);
//            }

//            if (cv::waitKey(20) >=0 ) break;
//        }
//    }


    WebcamStream webcam;
    ColorDetector colorDetector;

//    colorDetector.setColorValues(webcam);
//    lower_blue = np.array([110,50,50]);
//    upper_blue = np.array([130,255,255]);

    colorDetector.setColorValues(110, 130, 50, 255, 50, 255); //magic numbers from running set color values with webcam...

    if (webcam.isOpened())
    {

        while(true)
        {
            cv::Mat frame;
            cv::Mat mask;
            webcam.next(frame);
            colorDetector.run(frame, mask);

            cv::Mat dst;
            cv::bitwise_and(frame, frame, dst, thresholded);
            cv::imshow("Thresholded Image", dst);
            if (cv::waitKey(20) >= 0) break;
        }

    }

}
