#include "gl3dcanvas.h"

GL3DCanvas::GL3DCanvas(QWidget* parent, const QGLFormat& format, ProjectionMode mode):
	GLCanvasWindow(parent, format),
	projectionMode(mode),
	mouseState(UP),
	mouseInteractionMode(VIEW_TRANSFORM)
{
	sceneScale = 100.0;
	cameraPos = QVector3D(0, 0, sceneScale * 2);
}

void GL3DCanvas::initializeGL()
{
	GLCanvasWindow::initializeGL();
	trackBall.init();
	trackBall.setSceneScale(sceneScale);
}

void GL3DCanvas::resizeGL(int w, int h)
{
	trackBall.reshape(w, h);

	glViewport (0, 0, w, h);

	glMatrixMode (GL_PROJECTION);

	glLoadIdentity ();

	switch(projectionMode)
	{
	case ORTHONGONAL:
		{
			glOrtho (-1.0 * sceneScale, 1.0 * sceneScale, 
				-1.0 * sceneScale, 1.0 * sceneScale, 
				-1.0 * sceneScale, 1.0 * sceneScale); 
			break;
		}
	case FRUSTUM:
		{
			glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 1.0);
			break;
		}
	case PERSPECTIVE:
		{
			gluPerspective(60.0, (float)width()/(float)height(), 0.0001f, sceneScale * 10.0);
			break;
		}
	default:
		break;
	}

	glMatrixMode (GL_MODELVIEW);

	glLoadIdentity ();

	if(projectionMode==PERSPECTIVE)
	{
		QVector3D scaledCameraPos = cameraPos / trackBall.getScale();
		gluLookAt(
			scaledCameraPos.x(), scaledCameraPos.y(), scaledCameraPos.z(),
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
	}

}

void GL3DCanvas::paintGL()
{
	GLCanvasWindow::paintGL();

	resizeGL(width(), height());
	glLoadIdentity();

	resizeGL(width(), height());
	//trackBall.printMatrix();
	glMatrixMode(GL_MODELVIEW);

	trackBall.applyTransform();
}

QPointF GL3DCanvas::transformMousePositionFromInput(const QPointF & pos) const
{
	QPointF p;
	p.setX(pos.x() / (float)width());
	p.setY(1.0 - pos.y() / (float)height());
	return p;
}

void GL3DCanvas::wheelEvent(QWheelEvent *e)
{
	MouseState preState = mouseState;
	mouseState = WHEEL;
	switch(mouseInteractionMode)
	{
	case VIEW_TRANSFORM:
		{
			trackBall.wheel(e->delta());
			updateGL();
			break;
		}
	case INTERACTION:
		break;
	default:
		break;
	}
	mouseState = preState;
}

void GL3DCanvas::mousePressEvent(QMouseEvent *e)
{
	mouseState = DOWN;
	switch(mouseInteractionMode)
	{
	case VIEW_TRANSFORM:
		{
			switch(e->buttons() & 0xF)
			{
			case Qt::MidButton:
				{
					trackBall.mouse_translate(e->x(),e->y());
					break;
				}
			case Qt::LeftButton:
				{
					trackBall.mouse_rotate(e->x(), e->y());
					break;
				}
			default:
				break;
			}            
			break;
		}
	case INTERACTION:
		break;
	}
	update();
}

void GL3DCanvas::mouseReleaseEvent(QMouseEvent *)
{
	mouseState = UP;
}

void GL3DCanvas::switchInteractionState()
{
	if( mouseInteractionMode == VIEW_TRANSFORM )
	{
		cout << "Switching to interaction mode." << endl;
		mouseInteractionMode = INTERACTION;
	}
	else
	{
		cout << "Switching to view transform mode." << endl;
		mouseInteractionMode = VIEW_TRANSFORM;
	}
}

void GL3DCanvas::setSceneScale(double s)
{
	sceneScale = s;
	cameraPos = QVector3D(0, 0, sceneScale * 2);
	resizeGL(width(), height());
}

void GL3DCanvas::setCameraPos( float x, float y, float z )
{
	cameraPos = QVector3D(x, y, z);
	resizeGL(width(), height());
}

void GL3DCanvas::mouseMoveEvent(QMouseEvent *e)
{
	switch(mouseInteractionMode)
	{
	case VIEW_TRANSFORM:
		{
			switch(e->buttons() & 0xF)
			{
			case Qt::MidButton:
				{
					trackBall.motion_translate(e->x(),e->y());
					update();
					break;
				}
			case Qt::LeftButton:
				{
					trackBall.motion_rotate(e->x(), e->y());
					update();
					break;
				}
			default:
				break;
			}
			break;
		}
	case INTERACTION:
		break;
	}    
}

void GL3DCanvas::keyPressEvent( QKeyEvent *e )
{
	switch(e->key()) {
	case Qt::Key_Up:
		{
			cameraPos.setZ(cameraPos.z() - 0.01 * sceneScale);			
			// update the projection matrix
			resizeGL(width(), height());
			update();
			break;
		}
	case Qt::Key_Down:
		{
			cameraPos.setZ(cameraPos.z() + 0.01 * sceneScale);			
			// update the projection matrix
			resizeGL(width(), height());
			update();
			break;
		}
	}
}
