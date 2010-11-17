#include "pointbuffer.h"

#include <assert.h>
#include <limits>

bool                                    GLBufferExtension::s_vboInitialized = false;
GLBufferExtension::GenBuffersPtr        GLBufferExtension::s_glGenBuffers = 0;
GLBufferExtension::BindBuffersPtr       GLBufferExtension::s_glBindBuffer = 0;
GLBufferExtension::BufferDataPtr        GLBufferExtension::s_glBufferData = 0;
GLBufferExtension::DeleteBuffersPtr     GLBufferExtension::s_glDeleteBuffers = 0;
GLBufferExtension::PointParameterFPtr   GLBufferExtension::s_glPointParameterf = 0;
GLBufferExtension::PointParameterFVPtr  GLBufferExtension::s_glPointParameterfv = 0;

GLBufferExtension::GLBufferExtension(const QGLContext *context)
{
    initBufferExtension(context);
}

bool GLBufferExtension::genBuffers(GLsizei n, GLuint *buffers)
{
    if (s_glGenBuffers == 0)
        return false;
    s_glGenBuffers(n, buffers);
    return true;
}

bool GLBufferExtension::bindBuffer(GLenum target, GLuint buffer)
{
    if (s_glBindBuffer == 0)
        return false;
    s_glBindBuffer(target, buffer);
    return true;
}

bool GLBufferExtension::bufferData(GLenum target, GLsizeiptrARB size,
                                   const GLvoid *data, GLenum usage)
{
    if (s_glBufferData == 0)
        return false;
    s_glBufferData(target, size, data, usage);
    return true;
}

bool GLBufferExtension::deleteBuffers(GLsizei n, const GLuint *buffers)
{
    if (s_glDeleteBuffers == 0)
        return false;
    s_glDeleteBuffers(n, buffers);
    return true;
}

bool GLBufferExtension::pointParameterF(GLenum pname, GLfloat param)
{
    if (s_glPointParameterf == 0)
        return false;
    s_glPointParameterf(pname, param);
    return true;
}

bool GLBufferExtension::pointParameterFV(GLenum pname, GLfloat *param)
{
    if (s_glPointParameterfv == 0)
        return false;
    s_glPointParameterfv(pname, param);
    return true;
}

void GLBufferExtension::initBufferExtension(const QGLContext *context)
{
    if (s_vboInitialized)
        return;
    if (resolveExtension(context, "glGenBuffersARB", &s_glGenBuffers) &&
        resolveExtension(context, "glBindBufferARB", &s_glBindBuffer) &&
        resolveExtension(context, "glBufferDataARB", &s_glBufferData) &&
        resolveExtension(context, "glDeleteBuffersARB", &s_glDeleteBuffers) &&
        resolveExtension(context, "glPointParameterfARB", &s_glPointParameterf) &&
        resolveExtension(context, "glPointParameterfvARB", &s_glPointParameterfv))
        s_vboInitialized = true;
    if (!s_vboInitialized)
        qDebug("Failed to initialize VBO extension");
}

template <typename ExtensionProto>
bool GLBufferExtension::resolveExtension(const QGLContext *context,
                                         const char *extensionName,
                                         ExtensionProto *extensionPtr)
{
  *extensionPtr = reinterpret_cast<ExtensionProto>(
        context->getProcAddress(extensionName));
  return (*extensionPtr != 0);
}

Point::Point()
    : x(0.0f), y(0.0f), z(0.0f), r(1.0f), g(1.0f), b(1.0f), a(1.0f)
{ }

Point::Point(float _x, float _y, float _z,
             float _r, float _g, float _b, float _a)
  : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), a(_a)
{ }

PointBuffer::PointBuffer(const QGLContext *context)
    : GLBufferExtension(context), m_glBuffer(0), m_dirty(true),
      m_cloudTexture(0)
{
    genBuffers(1, &m_glBuffer);
    QImage img;
    bool loaded = img.load("cloud.png");
    if (loaded)
        m_cloudTexture = const_cast<QGLContext *>(context)->bindTexture(img);
    else
        qFatal("Failed to load cloud.png texture");
}

PointBuffer::~PointBuffer()
{
    deleteBuffers(1, &m_glBuffer);
}

size_t PointBuffer::size() const
{
    return m_pointList.size();
}

void PointBuffer::clear()
{
    m_pointList.clear();
    m_dirty = true;
}

void PointBuffer::addPoint(const Point &point)
{
    m_pointList.push_back(point);
    m_dirty = true;
}

void PointBuffer::render()
{
    bindBuffer(GL_ARRAY_BUFFER, m_glBuffer);
    if (m_dirty)
    {
        // upload point data
        bufferData(GL_ARRAY_BUFFER, size() * sizeof(Point),
            m_pointList.data(), GL_STATIC_DRAW);
        m_dirty = false;
    }

    // enable additive blending
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_cloudTexture);
    glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    float quadratic[] =  { 0.0f, 0.0f, 0.01f };
    pointParameterFV(GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic);
    pointParameterF(GL_POINT_SIZE_MIN_ARB, 0.0);
    pointParameterF(GL_POINT_SIZE_MAX_ARB, 64.0);

    glEnable(GL_POINT_SPRITE_ARB);
    glEnable(GL_POINT_SMOOTH );
    glPointSize(64.0);

    glVertexPointer(3, GL_FLOAT, sizeof(Point), vertexOffset(0));
    glColorPointer(4, GL_FLOAT, sizeof(Point), vertexOffset(sizeof(float) * 3));
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_POINTS, 0, size());
}

GLvoid *PointBuffer::vertexOffset(unsigned int offset)
{
    return (char *)0 + offset;
}
