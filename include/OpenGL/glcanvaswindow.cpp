#include "glcanvaswindow.h"

GLCanvasWindow::GLCanvasWindow(QWidget * parent, const QGLFormat& format):
QGLWidget(format, parent)
{
}

GLCanvasWindow::~GLCanvasWindow()
{
}

void
 GLCanvasWindow::initializeGL()
{
	glewInit();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glShadeModel(GL_SMOOTH);

    setMouseTracking(true);
}

void GLCanvasWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, 10.0, -10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void GLCanvasWindow::paintGL()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLCanvasWindow::enterEvent(QEvent *e)
{
    QGLWidget::enterEvent(e);
    grabKeyboard();
    setFocus();
}

void GLCanvasWindow::leaveEvent(QEvent *e)
{
    QGLWidget::leaveEvent(e);
    releaseKeyboard();
}

void GLCanvasWindow::dragEnterEvent(QDragEnterEvent *e)
{
    QGLWidget::dragEnterEvent(e);
}

void GLCanvasWindow::dragLeaveEvent(QDragLeaveEvent *e)
{
    QGLWidget::dragLeaveEvent(e);
}

void GLCanvasWindow::dragMoveEvent(QDragMoveEvent *e)
{
    QGLWidget::dragMoveEvent(e);
}

