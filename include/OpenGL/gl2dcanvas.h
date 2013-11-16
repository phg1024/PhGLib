#ifndef GL2DCANVAS_H
#define GL2DCANVAS_H

#include "glcanvaswindow.h"

#include <QVector2D>

#include <cassert>
#include <iostream>
using namespace std;

class GL2DCanvas : public GLCanvasWindow
{
public:
    GL2DCanvas(QWidget* parent = 0);
    virtual ~GL2DCanvas();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void wheelEvent(QWheelEvent *);

    virtual void setupViewPort();
    virtual void resetCanvas();

    virtual void keyPressEvent(QKeyEvent *){}
    virtual void keyReleaseEvent(QKeyEvent *){}

    /*
    virtual void dragEnterEvent(QDragEnterEvent *){}
    virtual void dragLeaveEvent(QDragEnterEvent *){}
    virtual void dragMoveEvent(QDragEnterEvent *){}
    */

protected:
    float zoomFactor;
    float leftBoundary, rightBoundary, bottomBoundary, topBoundary;

    bool isTranslating;
    QPointF tStartPoint, tEndPoint;
    QVector2D translateVector;

    QPointF transformMousePositionFromInput(const QPointF& p) const;

    enum MouseState
    {
        UP,
        DOWN
    } mMouseState;

    enum MouseInteractionMode
    {
        VIEW_TRANSFORM,
        INTERACTION
    } mInteractionMode;
};

#endif // GL2DCANVAS_H
