#include "streamrecorder.h"

StreamRecorder::StreamRecorder()
{

    string file = "/Users/fortjay81/Projects/VibeTrak/malletTracker.avi";

    if( remove(file.c_str()) != 0 )
        std::cout << "Error deleting file" << std::endl;
    else
         std::cout << "File successfully deleted" << std::endl;



    m_VideoOutput.open("/Users/fortjay81/Projects/VibeTrak/malletTracker.avi", -1, 30, cv::Size(512, 424));
//    m_VideoOutput.open("/Users/fortjay81/Projects/VibeTrak/malletTracker.avi", CV_FOURCC('M', 'P', '4', '2'), 30, cv::Size(512, 424));

    if (!m_VideoOutput.isOpened()){
        throw std::runtime_error("Video Writer Not opened");
    }
    else{
        std::cout << "Video Writer opened and running" << std::endl;
    }
}

void StreamRecorder::run(VibeFrame &frame){

    if (frame.colorFrame.size() == cv::Size(512, 424)){
        m_VideoOutput << frame.colorFrame;
    }
}
