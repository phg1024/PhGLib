#include "glutilities.h"

namespace PhGUtils
{

void drawBoundingBox(const Point3d &pMin, const Point3d &pMax)
{
    Point3d pts[8];
    pts[0] = Point3d(pMin.x, pMin.y, pMin.z);
    pts[1] = Point3d(pMax.x, pMin.y, pMin.z);
    pts[2] = Point3d(pMin.x, pMax.y, pMin.z);
    pts[3] = Point3d(pMin.x, pMin.y, pMax.z);

    pts[4] = Point3d(pMax.x, pMax.y, pMin.z);
    pts[5] = Point3d(pMax.x, pMin.y, pMax.z);
    pts[6] = Point3d(pMax.x, pMax.y, pMax.z);
    pts[7] = Point3d(pMin.x, pMax.y, pMax.z);

    drawLine(pts[0], pts[1]);
    drawLine(pts[0], pts[2]);
    drawLine(pts[0], pts[3]);

    drawLine(pts[6], pts[5]);
    drawLine(pts[6], pts[4]);
    drawLine(pts[6], pts[7]);

    drawLine(pts[2], pts[4]);
    drawLine(pts[2], pts[7]);
    drawLine(pts[3], pts[5]);

    drawLine(pts[3], pts[7]);
    drawLine(pts[4], pts[1]);
    drawLine(pts[5], pts[1]);
}

void drawLine(const Point3d& p1, const Point3d& p2, double lineWidth)
{
    float preLineWidth;
    glGetFloatv(GL_LINE_WIDTH, &preLineWidth);
    glLineWidth(lineWidth);
    glBegin(GL_LINES);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    glEnd();
    glLineWidth(preLineWidth);
}

void drawSphere(const Point3d& center,
                double radius)
{
    glPushMatrix();
    glTranslated(center.x, center.y, center.z);
    glutSolidSphere(radius, 32, 32);
    glPopMatrix();
}

void drawArrow(const Point3d &source, const Point3d &dir)
{
    Point3d end = source + dir;
    drawLine(source, end);
    drawLine(source + 0.9 * (end - source), source + 0.95 * (end - source), 3.0);
    drawLine(source + 0.95 * (end - source), source + 0.975 * (end - source), 2.0);
}

void drawRectangle(Point3d corners[4], double lineWidth)
{
    for(int cur=0;cur<4;cur++)
    {
        int next = (cur+1) % 4;
        drawLine(corners[cur], corners[next], lineWidth);
    }
}

void fillRectangle(Point3d corners[4])
{
    glPushMatrix();
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++)
        glVertex3d(corners[i].x, corners[i].y, corners[i].z);
    glEnd();
    glPopMatrix();
}


}
