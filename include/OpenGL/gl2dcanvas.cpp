#include "gl2dcanvas.h"
#include <cfloat>

GL2DCanvas::GL2DCanvas(QWidget *parent):
        GLCanvasWindow(parent, qglformat_3d),
        zoomFactor(1.0),
        isTranslating(false),
        translateVector(QVector2D(0,0)),
        mMouseState(UP),
        mInteractionMode(INTERACTION)
{
}

GL2DCanvas::~GL2DCanvas()
{

}

void GL2DCanvas::initializeGL()
{
    GLCanvasWindow::initializeGL();
}

void GL2DCanvas::resizeGL(int w, int h)
{
    glViewport (0, 0, w, h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    float aspectRatio = (float)w / (float)(h==0?FLT_MIN:h);
    leftBoundary = 0.5 * (1.0 - aspectRatio);
    rightBoundary = 0.5 * (1.0 + aspectRatio);
    bottomBoundary = 0.0;
    topBoundary = 1.0;
    glOrtho (leftBoundary, rightBoundary, bottomBoundary, topBoundary, 10.0, -10.0);

    //glOrtho (0.0, 1.0, 0.0, 1.0, 10.0, -10.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}

void GL2DCanvas::paintGL()
{
    GLCanvasWindow::paintGL();
    setupViewPort();
}

void GL2DCanvas::setupViewPort()
{
    glViewport (0, 0, width(), height());

    // set up zooming and panning
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    float aspectRatio = (float)width() / (float)height();
    leftBoundary = 0.5 - aspectRatio / 2.0 / zoomFactor  + translateVector.x();
    rightBoundary = 0.5 + aspectRatio / 2.0 / zoomFactor  + translateVector.x();
    bottomBoundary = 0.5 - 0.5 / zoomFactor  + translateVector.y();
    topBoundary = 0.5 + 0.5 / zoomFactor  + translateVector.y();
    glOrtho (leftBoundary, rightBoundary, bottomBoundary, topBoundary, 10.0, -10.0);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    // set up zooming and panning complete
}

void GL2DCanvas::resetCanvas()
{
    float aspectRatio = (float)width() / (float)height();
    leftBoundary = 0.5 * (1.0 - aspectRatio);
    rightBoundary = 0.5 * (1.0 + aspectRatio);
    bottomBoundary = 0.0;
    topBoundary = 1.0;
    glOrtho (leftBoundary, rightBoundary, bottomBoundary, topBoundary, 10.0, -10.0);

    zoomFactor = 1.0;

    translateVector = QVector2D(0.0, 0.0);

    update();
}

QPointF GL2DCanvas::transformMousePositionFromInput(const QPointF &p) const
{
    float normalizedX = (float)p.x()/(float)width();
    float normalizedY = 1.0 - (float)p.y()/(float)height();
    float transformedX = normalizedX * (rightBoundary - leftBoundary) + leftBoundary;
    float transformedY = normalizedY * (topBoundary - bottomBoundary) + bottomBoundary;
    return QPointF(transformedX, transformedY);
}

void GL2DCanvas::mousePressEvent(QMouseEvent *e)
{
    QPointF p = transformMousePositionFromInput(e->pos());
    mMouseState = DOWN;

    switch(mInteractionMode)
    {
    case VIEW_TRANSFORM:
        {
            tEndPoint = tStartPoint = p;            

            if(e->buttons()&0x4)
            {
                isTranslating = true;
            }
            update();
            e->accept();
            break;
        }
    case INTERACTION:
        {
            e->setAccepted(false);
            break;
        }
    default:
        break;
    }
}

void GL2DCanvas::mouseReleaseEvent(QMouseEvent *e)
{
    mMouseState = UP;
    QPointF p = transformMousePositionFromInput(e->pos());

    switch(mInteractionMode)
    {
    case VIEW_TRANSFORM:
        {
            tStartPoint = tEndPoint;
            tEndPoint = p;

            // hard code
            if(isTranslating)
            {
                QPointF diff = tEndPoint - tStartPoint;

                translateVector.setX(translateVector.x() - diff.x() * zoomFactor);
                translateVector.setY(translateVector.y() - diff.y() * zoomFactor);

                isTranslating = false;
                update();
                e->accept();
            }
            break;
        }
    case INTERACTION:
        {
            e->setAccepted(false);
            break;
        }
    default:
        break;
    }
}

void GL2DCanvas::mouseMoveEvent(QMouseEvent *e)
{
    QPointF p = transformMousePositionFromInput(e->pos());

    switch(mInteractionMode)
    {
    case VIEW_TRANSFORM:
        {
            if(isTranslating)
            {
                //cout<<"Translating the view port ..."<<endl;
                tStartPoint = tEndPoint;
                tEndPoint = p;

                QPointF diff = tEndPoint - tStartPoint;

                cout<<"diff = "<<diff.x()<<","<<diff.y()<<endl;
                translateVector.setX(translateVector.x() - diff.x() * zoomFactor);
                translateVector.setY(translateVector.y() - diff.y() * zoomFactor);
                cout<<"trans = "<<translateVector.x()<<","<<translateVector.y()<<endl;
                update();
                e->accept();
            }
            break;
        }
    case INTERACTION:
        {
            e->setAccepted(false);
            break;
        }
    default:
        break;
    }
}

void GL2DCanvas::wheelEvent(QWheelEvent *e)
{
    switch(mInteractionMode)
    {
    case VIEW_TRANSFORM:
        {
            if(e->delta()>0)
            {
                zoomFactor *= 1.1;
            }
            else
            {
                zoomFactor *= 0.909;
                if(zoomFactor <= 0.1)
                    zoomFactor = 0.1;
            }
            update();
            e->accept();
            break;
        }
    case INTERACTION:
        {
            e->setAccepted(false);
            break;
        }
    default:
        break;
    }
}
