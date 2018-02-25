#-------------------------------------------------
#
# Project created by QtCreator 2014-01-22T03:05:37
#
#-------------------------------------------------

QT       += widgets opengl xml

TARGET = PhGLib
TEMPLATE = lib
CONFIG += staticlib c++11

INCLUDEPATH += /usr/local/include /home/phg/SDKs/glew-1.12.0/include
LIBS += -L/usr/local/lib -L/home/phg/SDKs/glew-1.12.0/lib -lGLEW

SOURCES += \
    include/Geometry/MeshWriter.cpp \
    include/Geometry/MeshViewer.cpp \
    include/Geometry/MeshLoader.cpp \
    include/Geometry/Mesh.cpp \
    include/IO/FileMapper.cpp \
    include/OpenGL/glutilities.cpp \
    include/OpenGL/glTrackball.cpp \
    include/OpenGL/glEnv.cpp \
    include/OpenGL/glcanvaswindow.cpp \
    include/OpenGL/gl3dcanvas.cpp \
    include/OpenGL/gl2dcanvas.cpp \
    include/Utils/Timer.cpp \
    include/Utils/stringutils.cpp \
    include/Utils/fileutils.cpp \
    include/OpenGL/fbo.cpp

HEADERS += \
    include/phgutils.h \
    include/Core/Tree.hpp \
    include/Geometry/vector.hpp \
    include/Geometry/shape.h \
    include/Geometry/point.hpp \
    include/Geometry/MeshWriter.h \
    include/Geometry/MeshViewer.h \
    include/Geometry/MeshLoader.h \
    include/Geometry/MeshBase.hpp \
    include/Geometry/Mesh.h \
    include/Geometry/matrix.hpp \
    include/Geometry/mathutils.hpp \
    include/Geometry/geometryutils.hpp \
    include/Geometry/AABB.hpp \
    include/IO/FileMapper.h \
    include/IO/arrayallocator.h \
    include/Math/VectorBase.hpp \
    include/Math/Tensor.hpp \
    include/Math/Optimization.hpp \
    include/Math/MatrixBase.hpp \
    include/Math/mathutils.hpp \
    include/Math/DenseVector.hpp \
    include/Math/DenseMatrix.hpp \
    include/Math/denseblas.h \
    include/OpenGL/glutilities.h \
    include/OpenGL/glTrackball.h \
    include/OpenGL/glEnv.h \
    include/OpenGL/glcanvaswindow.h \
    include/OpenGL/gl3dcanvas.h \
    include/OpenGL/gl2dcanvas.h \
    include/Utils/utility.hpp \
    include/Utils/Timer.h \
    include/Utils/stringutils.h \
    include/Utils/singleton.hpp \
    include/Utils/fileutils.h \
    include/OpenGL/fbo.h

install_it.path = lib
install_it.files += build/libPhGLib.a
INSTALLS += install_it
