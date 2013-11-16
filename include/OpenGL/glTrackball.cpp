#include "glTrackball.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

void CGLTrackball::init()
{
    R = QQuaternion(1.0, 0.0, 0.0, 0.0); 	// no rotation
    T = QVector3D(0.0, 0.0, 0.0);           // no translation

    m_x = m_y = 0;
    m_r = 0;
    m_width = m_height = 0;
    m_scale = 1.0;
    m_sceneScale = 1.0;

    m_Quaternion2Matrix();
}

void CGLTrackball::reset()
{
    R = QQuaternion(1.0, 0.0, 0.0, 0.0);
    m_scale = 1.0;
    m_Quaternion2Matrix();
}

QVector3D CGLTrackball::getNormDir()
{
    return R.vector();
}

QQuaternion CGLTrackball::getRotation() const
{
    return R;
}

QQuaternion CGLTrackball::getIncRotation() const
{
    return incR;
}

QVector3D CGLTrackball::getTranslation() const
{
    return T;
}

void CGLTrackball::reshape(int w, int h)
{
    m_width = w;
    m_height = h;

    m_r = min(m_width, m_height) / 2.f;
}

void CGLTrackball::mouse_rotate(int u, int v)
{
    v = m_height - v;

    m_x = u;
    m_y = v;
}

void CGLTrackball::mouse_translate(int u, int v)
{
    v = m_height - v;

    m_x = u;
    m_y = v;
}

void CGLTrackball::motion_rotate(int u, int v)
{
    QVector3D N;
    QVector3D v0(0, 0, 0), v1(0, 0, 0);
    float theta;

    v = m_height - v;

    if (u<0 || u>m_width || v<0 || v>m_height) return;

    v0.setX(m_x - m_width/2.f);
    v0.setY(m_y - m_height/2.f);
    v0.setZ(0.f);

    if (v0.length() < 2) return;
    v0.setZ(m_r*m_r/2.f / v0.length());

    v1.setX(u - m_width/2.f);
    v1.setY(v - m_height/2.f);
    v1.setZ(0.f);

    if (v1.length() < 2) return;
    v1.setZ(m_r*m_r/2.f / v1.length());

    v0.normalize();
    v1.normalize();
    N = QVector3D::crossProduct(v0, v1);
    N.normalize();

    float dotProduct = QVector3D::dotProduct(v0, v1);

    if(dotProduct > 1.0)
        dotProduct = 1.0;

    if(dotProduct < -1.0)
        dotProduct = -1.0;

    theta = acos(dotProduct);

    incR = QQuaternion(cos(theta/2.f), N*sin(theta/2.f));

    R = incR*R;

    m_Quaternion2Matrix();

    m_x = u, m_y = v;
}

void CGLTrackball::motion_translate(int u, int v)
{
    v = m_height - v;

    // a straight forward scheme
    QVector3D screenMovementVector = QVector3D((float)(u - m_x)/(float)m_width, (float)(v - m_y)/(float)m_height, 0.0);

    m_Quaternion2Matrix();

    QMatrix4x4 modelMatrix = QMatrix4x4(M);
    QVector3D movementVector = modelMatrix * screenMovementVector;

    T += movementVector / m_scale * m_sceneScale;

    m_x = u, m_y = v;
}

void CGLTrackball::rotate(const QQuaternion &Q)
{
    R = Q*R;

    m_Quaternion2Matrix();
}

void CGLTrackball::setRotation(const QQuaternion &Q)
{
    R = Q;

    m_Quaternion2Matrix();
}

void CGLTrackball::wheel(int delta)
{
    if (delta > 0)
        m_scale *= 1.025;
    else
        m_scale *= 0.975;
}

void CGLTrackball::applyRotation()
{
    m_Quaternion2Matrix();
    glMultMatrixf(M);
}

void CGLTrackball::applyTransform()
{
    m_Quaternion2Matrix();
    glMultMatrixf(M);
//    glScalef(m_scale, m_scale, m_scale);
    glTranslatef(T.x(), T.y(), T.z());
}

void CGLTrackball::applyInverseTransform()
{
    QQuaternion S = R;
    R.setScalar(-R.scalar());
    m_Quaternion2Matrix();
    R = S;

    glMultMatrixf(M);
    glScalef(1.f/m_scale, 1.f/m_scale, 1.f/m_scale);
}

float* CGLTrackball::getMatrix()
{
    m_Quaternion2Matrix();
    return M;
}

float* CGLTrackball::getInverseMatrix()
{
    QQuaternion S = R;
    R.setScalar(-R.scalar());
    m_Quaternion2Matrix();
    R = S;
    return M;
}

void CGLTrackball::applyInverseRotation()
{
    QQuaternion S = R;
    R.setScalar(-R.scalar());
    m_Quaternion2Matrix();
    R = S;

    glMultMatrixf(M);
}

void CGLTrackball::m_Quaternion2Matrix()
{
    float q0 = R.scalar(),
    q1 = R.x(),
    q2 = R.y(),
    q3 = R.z();

    M[0] = q0*q0 + q1*q1 - q2*q2 - q3*q3;
    M[1] = 2*(q1*q2 + q0*q3);
    M[2] = 2*(q1*q3 - q0*q2);
    M[3] = 0;

    M[4] = 2*(q1*q2 - q0*q3);
    M[5] = q0*q0 + q2*q2 - q1*q1 - q3*q3;
    M[6] = 2*(q2*q3 + q0*q1);
    M[7] = 0;

    M[8] = 2*(q1*q3 + q0*q2);
    M[9] = 2*(q2*q3 - q0*q1);
    M[10]= q0*q0 + q3*q3 - q1*q1 - q2*q2;
    M[11]= 0;

    M[12]= 0;
    M[13]= 0;
    M[14]= 0;
    M[15]= 1;
}

void CGLTrackball::loadStatus(const char *filename)
{
    ifstream ifs;
    float x, y, z, w, s;

    ifs.open(filename);

    if (!ifs) {
        cerr << "[CGLTrackball::loadStatus] cannot open trackball file. " << endl;
        return;
    }

    ifs >> x >> y >> z >> w >> s;

    R = QQuaternion(w, x, y, z);
    m_scale = s;

    ifs.close();

    m_Quaternion2Matrix();
}

void CGLTrackball::saveStatus(const char *filename)
{
    ofstream ofs;

    ofs.open(filename);

    if (!ofs) {
        cerr << "[CGLTraclball::saveStatus] cannot save trackball file. " << endl;
        return;
    }

    ofs << R.x() << '\t' << R.y() << '\t' << R.z() << '\t' << R.scalar() << '\t' << m_scale;
    ofs.close();
}
