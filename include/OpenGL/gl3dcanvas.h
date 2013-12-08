#ifndef GL3DCANVAS_H
#define GL3DCANVAS_H

#include "glcanvaswindow.h"
#include "glTrackball.h"

#include <iostream>
using namespace std;

class GL3DCanvas : public GLCanvasWindow
{
public:
    enum ProjectionMode
    {
        ORTHONGONAL,
        FRUSTUM,
        PERSPECTIVE
    };

    enum MouseState
    {
        UP,
        DOWN,
        WHEEL,
        STANDBY
    };

    enum MouseInteractionMode
    {
        VIEW_TRANSFORM,
        INTERACTION
    };

    GL3DCanvas(QWidget* parent = 0, const QGLFormat& format = qglformat_3d, ProjectionMode mode = PERSPECTIVE);
    virtual ~GL3DCanvas(){};

    ProjectionMode getProjectionMode() {return projectionMode;};
    void setProjectionMode(ProjectionMode mode){projectionMode = mode;resizeGL(width(), height());updateGL();};

public slots:
    void switchInteractionState();

    void setSceneScale(double s);
	void setCameraPos(float x, float y, float z);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    virtual void wheelEvent(QWheelEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mousePressEvent(QMouseEvent * e);
    virtual void mouseReleaseEvent(QMouseEvent * e);
	
	virtual void keyPressEvent(QKeyEvent *e);

    QPointF transformMousePositionFromInput(const QPointF&) const;
protected:
    ProjectionMode projectionMode;
    MouseState mouseState;
    MouseInteractionMode mouseInteractionMode;
    CGLTrackball trackBall;
    QVector3D cameraPos;
    double sceneScale;
};

#endif // GL3DCANVAS_H
