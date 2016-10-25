#include "onistream.h"

#define TEXTURE_SIZE	512

#define MIN_NUM_CHUNKS(data_size, chunk_size)	((((data_size)-1) / (chunk_size) + 1))
#define MIN_CHUNKS_SIZE(data_size, chunk_size)	(MIN_NUM_CHUNKS(data_size, chunk_size) * (chunk_size))


OniStream::OniStream():
m_streams(NULL)
{
    m_opened = (init() == openni::STATUS_OK); // && initRecorder() == openni::STATUS_OK);
    record(); // Always start recording.  FOR NOW....
}

OniStream::OniStream(const char* oniFile):
m_streams(NULL)
{
    m_opened = (init(oniFile) == openni::STATUS_OK);
}

OniStream::~OniStream()
{

    if (m_streams != NULL)
    {
        delete []m_streams;
    }

    printf("OniStream Closed Successfully\n\n");
}

openni::Status OniStream::init(const char* deviceUri)
{
    openni::Status rc = openni::STATUS_OK;

    // Initialize and Open the device
    rc = openni::OpenNI::initialize();
    printf("OniViewer:  After initialization:\n%s\n", openni::OpenNI::getExtendedError());

    rc = m_device.open(deviceUri);
    if (rc != openni::STATUS_OK)
    {
        printf("OniViewer:  Device failed to open:\n%s\n", openni::OpenNI::getExtendedError());
        openni::OpenNI::shutdown();
        return rc;
    }

    // Grab the Depth Stream
    rc = m_depthStream.create(m_device, openni::SENSOR_DEPTH);
    if (rc == openni::STATUS_OK)
    {
        rc = m_depthStream.start();
        if (rc != openni::STATUS_OK)
        {
            printf("OniViewer: Couldn't start Depth Stream:\n%s\n", openni::OpenNI::getExtendedError());
            m_depthStream.destroy();
        }
    }
    else
    {
        printf("OniViewer: Coudn't find Depth Stream:\n%s\n", openni::OpenNI::getExtendedError());
    }

    // Grab the Color Stream
    rc = m_colorStream.create(m_device, openni::SENSOR_COLOR);
    //m_colorStream.setCropping(0,0, 640, 480);
    if (rc == openni::STATUS_OK)
    {
        rc = m_colorStream.start();
        if (rc != openni::STATUS_OK)
        {
            printf("OniViewer: Couldn't start Color Stream:\n%s\n", openni::OpenNI::getExtendedError());
            m_colorStream.destroy();
        }
    }
    else
    {
        printf("OniViewer: Coudn't find Color Stream:\n%s\n", openni::OpenNI::getExtendedError());
    }

    // Verify at least one stream is valid
    openni::VideoMode depthVideoMode;
    openni::VideoMode colorVideoMode;

    if (!m_depthStream.isValid() && !m_colorStream.isValid())
    {
        printf("OniViewer: No Valid Streams. Exiting.\n");
        openni::OpenNI::shutdown();
        return openni::STATUS_ERROR;
    }

    if (m_depthStream.isValid())
    {
        depthVideoMode = m_depthStream.getVideoMode();
        m_depthWidth = depthVideoMode.getResolutionX();
        m_depthHeight = depthVideoMode.getResolutionY();
    }

    if (m_colorStream.isValid())
    {
        colorVideoMode = m_colorStream.getVideoMode();
        m_colorWidth = colorVideoMode.getResolutionX();
        m_colorHeight = colorVideoMode.getResolutionY();
    }

    // Set Depth to Color Regstration if supported
    bool registration_supported = m_device.isImageRegistrationModeSupported(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
    if (registration_supported)
    {
        rc = m_device.setDepthColorSyncEnabled(true);
        m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
        printf("\tOniStream: Image Registration Depth to Color Set Successfully\n");
    }

    // Get video modes of device
    printf("--Color Modes--\n");
    const openni::SensorInfo* s_info = m_device.getSensorInfo(openni::SENSOR_COLOR);
    const openni::Array<openni::VideoMode>& modes_color = s_info->getSupportedVideoModes();

    for (int i=0; i< modes_color.getSize(); ++i)
    {
        printf("\t%i: %ix%i, %i fps, %i format\n", i, modes_color[i].getResolutionX(), modes_color[i].getResolutionY(),
            modes_color[i].getFps(), modes_color[i].getPixelFormat());
        i++;
    }
    printf("\n");

    printf("--Depth Modes and Info--\n");
    s_info = m_device.getSensorInfo(openni::SENSOR_DEPTH);
    const openni::Array<openni::VideoMode>& modes_depth = s_info->getSupportedVideoModes();

    for (int i=0; i< modes_depth.getSize(); ++i)
    {
        printf("\t%i: %ix%i, %i fps, %i format\n", i, modes_depth[i].getResolutionX(), modes_depth[i].getResolutionY(),
            modes_depth[i].getFps(), modes_depth[i].getPixelFormat());
        i++;
    }
    printf("Min Pixel Value: %d | Max Pixel Value: %d", m_depthStream.getMinPixelValue(), m_depthStream.getMaxPixelValue());
    printf("\n");

    m_streams = new openni::VideoStream*[2];
    m_streams[0] = &m_colorStream;
    m_streams[1] = &m_depthStream;

    return openni::STATUS_OK;
}

openni::Status OniStream::initRecorder()
{
    /*****************************
    Initialize and Create Recorder
    *****************************/

    openni::Status rc = openni::STATUS_OK;

    rc = m_recorder.create("onistream.oni");
    if (rc != openni::STATUS_OK)
    {
        printf("OniStream: Unable to Create Recorder");
        return rc;
    }

    // Attach the Depth Stream
    rc = m_recorder.attach(m_depthStream);
    if (rc != openni::STATUS_OK)
    {
        printf("OniStream: Unable to Create Recording; Depth Stream Not Attached");
        return rc;
    }

    // Attach the Color Stream
    rc = m_recorder.attach(m_colorStream, true);
    if (rc != openni::STATUS_OK)
    {
        printf("OniStream: Unable to Create Recording; Color Stream Not Attached");
        return rc;
    }

    return openni::STATUS_OK;
}

bool OniStream::isLive()
{
    return true;
}

bool OniStream::isOpened()
{
    return m_opened;
}

bool OniStream::next(cv::Mat& frame)
{
    int changedIndex;
    openni::Status rc = openni::OpenNI::waitForAnyStream(m_streams, 2, &changedIndex);
    if (rc != openni::STATUS_OK)
    {
        printf("OniViewer: Wait Failed\n");
        return false;
    }

//    switch (changedIndex)
//    {
//    case 0:
//        m_colorStream.readFrame(&m_depthFrame);
//        break;
//    case 1:
//        m_depthStream.readFrame(&m_colorFrame);
//        break;
//    default:
//        printf("OniViewer: Error in Wait\n");
//    }

    m_colorStream.readFrame(&m_depthFrame);
    m_depthStream.readFrame(&m_colorFrame);

    // Process Depth Frame
//    if (m_depthFrame.isValid())
//    {
//        const openni::DepthPixel* pImgBuffer = (const openni::DepthPixel*)m_depthFrame.getData();

//        // Copy data buffer to an OpenCV Mat object
//        cv::Mat temp(m_depthHeight, m_depthWidth, CV_16UC1);
//        memcpy(temp.data, pImgBuffer, m_depthFrame.getDataSize());

//        frame.create(m_depthHeight, m_depthWidqth, CV_8UC1);
//        cv::normalize(temp, frame, 0, 255, cv::NORM_MINMAX, CV_8UC1);
//    }

    if (m_colorFrame.isValid()){
        const openni::RGB888Pixel* pImgBuffer = (const openni::RGB888Pixel*)m_colorFrame.getData();

        frame.create(m_colorHeight, m_colorWidth, CV_8UC3);
        memcpy(frame.data, pImgBuffer, m_colorFrame.getDataSize());
    }

    return true;
}

bool OniStream::next(cv::Mat& depthFrame, cv::Mat& colorFrame)
{
    int changedIndex;
    openni::Status rc = openni::OpenNI::waitForAnyStream(m_streams, 2, &changedIndex);
    if (rc != openni::STATUS_OK)
    {
        printf("OniViewer: Wait Failed\n");
        return false;
    }

    switch (changedIndex)
    {
    case 0:
        m_colorStream.readFrame(&m_colorFrame);
        break;
    case 1:
        m_depthStream.readFrame(&m_depthFrame);
        break;
    default:
        printf("OniViewer: Error in Wait\n");
    }

    // Process Depth Frame
    if (m_depthFrame.isValid())
    {
        const openni::DepthPixel* pImgBuffer = (const openni::DepthPixel*)m_depthFrame.getData();

        // Copy data buffer to an OpenCV Mat object
        cv::Mat temp(480, 640, CV_16UC1);
        memcpy(temp.data, pImgBuffer, 640*480*sizeof(openni::DepthPixel));

        cv::Rect r(0, 0, 512, 424);
        cv::normalize(temp(r), depthFrame, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    }

    if (m_colorFrame.isValid()){
        const openni::RGB888Pixel* pImgBuffer = (const openni::RGB888Pixel*)m_colorFrame.getData();

//        colorFrame.create(m_colorHeight, m_colorWidth, CV_8UC3);
//        memcpy(colorFrame.data, pImgBuffer, m_colorFrame.getDataSize());

        colorFrame.create(424, 512, CV_8UC3);
        memcpy(colorFrame.data, pImgBuffer, 512*424*sizeof(openni::RGB888Pixel));
    }


    return true;
}


bool OniStream::previous(cv::Mat& frame)
{
   return false;
}

void OniStream::record()
{
    openni::Status rc = openni::STATUS_OK;
    if( !m_recorder.isValid()){
        rc = initRecorder();
    }

    if (rc == openni::STATUS_OK)
    {
        rc = m_recorder.start();
    }
}

void OniStream::stopRecording()
{
    m_recorder.stop();
}
