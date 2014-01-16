#include "MeshViewer.h"

namespace PhGUtils {

	MeshViewer::MeshViewer(QWidget* parent):
		GL3DCanvas(parent)
	{
		this->mouseInteractionMode = GL3DCanvas::VIEW_TRANSFORM;
		this->resize(640, 480);
		this->setWindowTitle("Mesh Viewer");
		this->setSceneScale(1.0);
	}


	MeshViewer::~MeshViewer(void)
	{
	}

	void MeshViewer::initializeGL() {
		makeCurrent();
		GL3DCanvas::initializeGL();
		doneCurrent();
	}

	void MeshViewer::paintGL() {
		GL3DCanvas::paintGL();
		glColor4f(0, 0, 0, 0.5);
		enableLighting();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glPushMatrix();
		glTranslatef(-massCenter.x, -massCenter.y, -massCenter.z);
		switch (type)
		{
		case PhGUtils::MeshViewer::QUAD_MESH:
			quad->draw();
			break;
		case PhGUtils::MeshViewer::TRIANGLE_MESH:
			tri->draw();
			break;
		default:
			break;
		}

		// for debug, show the aabb tree
		//aabb->paint();

		GLfloat mat_diffuse[] = {1.0, 0.375, 0.375, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		for(int i=0;i<hints.size();i++) {
			glPushMatrix();
			Point3f p;

			switch (type)
			{
			case PhGUtils::MeshViewer::QUAD_MESH:
				p = quad->vertex(hints[i].first);
				break;
			case PhGUtils::MeshViewer::TRIANGLE_MESH:
				p = tri->vertex(hints[i].first);
				break;
			default:
				break;
			}

			glTranslatef(p.x, p.y, p.z);
			glutSolidSphere(5, 16, 16);

			glPopMatrix();
		}

		glPopMatrix();

		disableLighting();
	}

	void MeshViewer::resizeGL(int w, int h) {
		GL3DCanvas::resizeGL(w, h);
	}

	void MeshViewer::enableLighting()
	{
		GLfloat light_position[] = {2.0 * scale, 2.0 * scale, 2.0 * scale, 1.0};
		GLfloat mat_specular[] = {0.8, 0.8, 0.8, 1.0};
		GLfloat mat_diffuse[] = {0.375, 0.375, 0.375, 1.0};
		GLfloat mat_shininess[] = {25.0};
		GLfloat light_ambient[] = {0.05, 0.05, 0.05, 1.0};
		GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};

		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);

		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
		glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

		light_position[0] = -2.0 * scale;
		glLightfv(GL_LIGHT1, GL_POSITION, light_position);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
		glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}

	void MeshViewer::disableLighting()
	{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);
	}

	void MeshViewer::updateViewingParameters()
	{
		// compute translation and scale
		massCenter = Point3f(
			0.5 * (aabb->root()->aabb.maxX() + aabb->root()->aabb.minX()), 
			0.5 * (aabb->root()->aabb.maxY() + aabb->root()->aabb.minY()), 
			0.5 * (aabb->root()->aabb.maxZ() + aabb->root()->aabb.minZ())
			);

		cout << "mass center = " << massCenter << endl;

		scale = std::max(std::max(aabb->root()->aabb.XRange(), aabb->root()->aabb.YRange()), aabb->root()->aabb.ZRange());
		this->setSceneScale(scale);
		cout << "scale = " << scale << endl;
	}

}