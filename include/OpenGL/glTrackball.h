#ifndef _GLTRACKBALL_H
#define _GLTRACKBALL_H

#include "glEnv.h"

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

/*! \brief The trackball class
 *
 * implementation of mouse rotation and translation
 *
 * \sa http://www.opengl.org/wiki/Trackball
 *
 * \todo translation function
 */

#ifdef WIN32
#define TRACKBALL_USE_DOUBLE 0
#else
#define TRACKBALL_USE_DOUBLE 0
#endif

class CGLTrackball
{
public:
#if TRACKBALL_USE_DOUBLE
    typedef double elem_t;
#else
    typedef float elem_t;
#endif

    CGLTrackball(){};
    CGLTrackball(const CGLTrackball& ball):
        R(ball.getRotation()),
        incR(ball.getIncRotation()),
        T(ball.getTranslation()),
        m_x(ball.getX()),
        m_y(ball.getY()),
        m_width(ball.getWidth()),
        m_height(ball.getHeight()),
        m_r(ball.getR()),
        m_scale(ball.getScale())
    {
        for(int i=0;i<16;i++)
            M[i] = ball.M[i];
    }
    void init();

    void setSceneScale(elem_t s) {m_sceneScale = s;}

    void reshape(int w, int h);
    void mouse_rotate(int u, int v);
    void mouse_translate(int u, int v);
    void motion_rotate(int u, int v);
    void motion_translate(int u, int v);
    void joystick(unsigned int mask, int x, int y, int z);
    void wheel(int delta);

    void applyRotation();
    void applyTransform();
    void applyInverseTransform();
    void applyInverseRotation();

    elem_t getX() const {return m_x;};
    elem_t getY()  const {return m_y;};
    elem_t getWidth()  const {return m_width;};
    elem_t getHeight()  const {return m_height;};
    elem_t getR()  const {return m_r;};
    elem_t getScale()  const {return m_scale;}
    QVector3D getNormDir();
    QQuaternion getRotation() const ;
    QVector3D   getTranslation() const ;
    QQuaternion getIncRotation() const ;

    void reset();
    void loadStatus(const char *filename);
    void saveStatus(const char *filename);

    void setRotation(const QQuaternion &Q);
    void rotate(const QQuaternion &Q);

    elem_t* getMatrix();
    elem_t* getInverseMatrix();

    elem_t M[16];

protected:
    QQuaternion R; 		//!< Rotation
    QQuaternion incR; 	//!< Increase Rotation
    QVector3D T; 		//!< Translation

    void m_Quaternion2Matrix();

private:
    int m_x, m_y;
    int m_width, m_height;
    elem_t m_r; 		    //!< min(m_width, m_height)
    elem_t m_scale;
    elem_t m_sceneScale;
};

#endif
