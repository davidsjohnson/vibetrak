#include "onistream.h"

#include <opencv/cv.h>
#include <opencv2/imgproc.hpp>

#define TEXTURE_SIZE	512

#define MIN_NUM_CHUNKS(data_size, chunk_size)	((((data_size)-1) / (chunk_size) + 1))
#define MIN_CHUNKS_SIZE(data_size, chunk_size)	(MIN_NUM_CHUNKS(data_size, chunk_size) * (chunk_size))


/*
 * Initializes OpenNI with the first device it finds
*/
OniStream::OniStream(QObject* parent):
VideoStream(parent), m_streams(NULL)
{
    m_opened = (init() == openni::STATUS_OK);
}

/*
 * Initializes OpenNI with a specific ONI file
*/
OniStream::OniStream(const char* oniFile, QObject* parent):
VideoStream(parent), m_streams(NULL)
{
    m_opened = (init(oniFile) == openni::STATUS_OK);
    m_recording = false;
}

OniStream::~OniStream()
{
    if (m_streams != NULL)
    {
        delete []m_streams;
    }
    printf("OniStream Closed Successfully\n\n");
}


/*
 * Initializes OpenNI with a color and/or depth stream depending
 * on the capabilities of the device
 */
openni::Status OniStream::init(const char* deviceUri)
{

    if (deviceUri==openni::ANY_DEVICE)
        m_live = true;
    else
        m_live = false;

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
    if (!m_depthStream.isValid() && !m_colorStream.isValid())
    {
        printf("OniViewer: No Valid Streams. Exiting.\n");
        openni::OpenNI::shutdown();
        return openni::STATUS_ERROR;
    }

    openni::VideoMode depthVideoMode;
    openni::VideoMode colorVideoMode;

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

    // Get and Print video modes of each stream in device
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

    //Print Pixel values
    printf("Min Pixel Value: %d | Max Pixel Value: %d", m_depthStream.getMinPixelValue(), m_depthStream.getMaxPixelValue());
    printf("\n");

    m_streams = new openni::VideoStream*[2];
    m_streams[0] = &m_colorStream;
    m_streams[1] = &m_depthStream;

    return openni::STATUS_OK;
}


/*
 * Initializes and creates a recorder for recording session
 * to an ONI File
 */
openni::Status OniStream::initRecorder(string filename)
{
    openni::Status rc = openni::STATUS_OK;

    rc = m_recorder.create(filename.c_str());
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


/*
 * Returns true if this is a live stream and false otherwise.
 * Needed since a live stream cannot go in reverse
 */
bool OniStream::isLive()
{
    return m_live;
}


/*
 * Returns true if this is strem is opened successfully and false otherwise.
 */

/*
 * Generates an OpenCV mat with the data from current frame
 * The Mat is in BGR format
 * UPDATE: How to handle color vs depth (or should we care)
 */
bool OniStream::next(VibeFrame& frame)
{
    // Wait for an available stream and check which stream was updated
    int changedIndex;
    openni::Status rc = openni::OpenNI::waitForAnyStream(m_streams, 2, &changedIndex);
    if (rc != openni::STATUS_OK)
    {
        printf("OniViewer: Wait Failed\n");
        return false;
    }

    // Check which stream was updated
    switch (changedIndex)
    {
    case 0:
//        printf("#### Color ####\n");
        m_colorStream.readFrame(&m_colorFrame);
        break;
    case 1:
//        printf("#### Depth ####\n");
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
        cv::normalize(temp(r), frame.depthFrame, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    }

    // Process Color Frame
    if (m_colorFrame.isValid()){
        const openni::RGB888Pixel* pImgBuffer = (const openni::RGB888Pixel*)m_colorFrame.getData();

//        frame.colorFrame.create(m_colorHeight, m_colorWidth, CV_8UC3);
//        memcpy(frame.colorFrame.data, pImgBuffer, m_colorFrame.getDataSize());

        frame.colorFrame.create(424, 512, CV_8UC3);
        memcpy(frame.colorFrame.data, pImgBuffer, 424*512*sizeof(openni::RGB888Pixel));
        cv::cvtColor(frame.colorFrame, frame.colorFrame, cv::COLOR_BGR2RGB);
    }

    return true;
}

/*
 * Returns previous frame data if data stream isn't live
 */
bool OniStream::previous(VibeFrame& frame)
{
   return false;
}

/*
 * Initializes (if necessary) and starts recording the session
 */
void OniStream::record(string filename)
{
    openni::Status rc = openni::STATUS_OK;
    if( !m_recorder.isValid()){
        rc = initRecorder(filename);
    }

    if (rc == openni::STATUS_OK)
    {
        rc = m_recorder.start();
        m_recording = true;
    }
}

/*
 * Stops Recording
 * If record is started after stopping
 * the ONI file will be appended to
 */
void OniStream::stopRecording()
{
    m_recorder.stop();
    m_recorder.destroy();
    m_recording = false;
}
