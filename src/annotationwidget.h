#ifndef ANNOTATIONWIDGET_H
#define ANNOTATIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QPixmap>

#include "videostream.h"
#include "streamwidget.h"

class AnnotationWidget : public QWidget
{

    Q_OBJECT

public:
    explicit AnnotationWidget(VideoStream* stream, StreamWidget* main, QWidget* parent = 0);

private:
    VideoStream* m_stream;
    StreamWidget* m_main;

protected:
    void closeEvent(QCloseEvent* event);

signals:


public slots:
    void nextFrameCallback();
    void closeCallback();
    void sendCallback();
    void viewCallback();

};

#endif // ANNOTATIONWIDGET_H
