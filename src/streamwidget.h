#ifndef STREAMWIDGET_H
#define STREAMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "videostream.h"
#include "onistream.h"

class StreamWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString class READ cssClass)

public:
    explicit StreamWidget(VideoStream* stream, QWidget *parent = 0);
//    ~StreamWidget();
    void start();
    void stop();

    QString cssClass() { return QString("StreamWidget"); }

private:
    void openOniFile();

    QLabel* m_lFrame;
    VideoStream* m_stream;
    QTimer* m_timer;

    QPushButton* m_btnRecord;

signals:

public slots:
    void run();
    void handleOpen();
    void handleOpenAnnotator();
    void handleRecord();
    void handleLive();
    void closing();


};

#endif // STREAMWIDGET_H
