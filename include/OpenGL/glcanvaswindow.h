#ifndef GLCANVASWINDOW_H
#define GLCANVASWINDOW_H

#include "../phgutils.h"

#include <QGLWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

const QGLFormat qglformat_complex(
                QGL::DoubleBuffer       |
                QGL::DepthBuffer        |
                QGL::AccumBuffer        |
                QGL::StencilBuffer      |
                QGL::StereoBuffers      |
                //QGL::SampleBuffers      |
                QGL::Rgba               |
                QGL::AlphaChannel       |
                QGL::DirectRendering    |
                QGL::HasOverlay
                );

const QGLFormat qglformat_2d(
                QGL::DoubleBuffer       |
                QGL::DepthBuffer        |
                QGL::AccumBuffer        |
                QGL::StencilBuffer      |
                //QGL::StereoBuffers      |
                QGL::SampleBuffers      |
                QGL::Rgba               |
                QGL::AlphaChannel       |
                QGL::DirectRendering    |
                QGL::HasOverlay
                );

const QGLFormat qglformat_3d(
                QGL::DoubleBuffer       |
                QGL::DepthBuffer        |
                QGL::AccumBuffer        |
                //QGL::StencilBuffer      |
                //QGL::StereoBuffers      |
                QGL::SampleBuffers      |
                QGL::Rgba               |
                QGL::AlphaChannel       |
                QGL::DirectRendering    |
                QGL::HasOverlay
                );


class GLCanvasWindow:public QGLWidget {
  public:
        GLCanvasWindow(QWidget * parent = 0, const QGLFormat& format = qglformat_complex);
    GLCanvasWindow(int w, int h, QWidget * parent = 0);
    virtual ~GLCanvasWindow();

  protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

  protected:
    virtual void mousePressEvent(QMouseEvent *){};
    virtual void mouseMoveEvent(QMouseEvent *){};
    virtual void mouseReleaseEvent(QMouseEvent *){};

    virtual void keyPressEvent(QKeyEvent *){};
    virtual void keyReleaseEvent(QKeyEvent *){};

    virtual void wheelEvent(QWheelEvent *){};

    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dragLeaveEvent(QDragLeaveEvent *);
    virtual void dragMoveEvent(QDragMoveEvent *);
};

#endif
