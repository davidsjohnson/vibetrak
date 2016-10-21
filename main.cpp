#include <QCoreApplication>
#include <iostream>

#include "webcamstream.h"
#include "onistream.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
//    return a.exec();


//    WebcamStream webcam;

//    OniStream oniStream;
    OniStream oniStream("onistream.oni");

    if (oniStream.isOpened())
    {
        printf("OniStream Opened Successfully\n");
    }

    if (oniStream.isOpened())
    {
        while (true)
        {
            cv::Mat depthFrame, colorFrame;
            oniStream.next(depthFrame, colorFrame);

            if (depthFrame.size != NULL)
                cv::imshow("Depth", depthFrame);
            if (colorFrame.data != NULL){
                cv::Mat c(960, 540, CV_8UC3);
                cv::resize(colorFrame, c, cv::Size(960, 540));
                cv::imshow("Color", c);
            }

            if (cv::waitKey(20) >=0 ) break;
        }
    }

}
