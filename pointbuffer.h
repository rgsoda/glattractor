#ifndef POINTBUFFER_H
#define POINTBUFFER_H

#include <QVector>
#include <QtOpenGL>

class GLBufferExtension
{
protected:
    GLBufferExtension(const QGLContext *context);

    bool genBuffers(GLsizei n, GLuint *buffers);
    bool bindBuffer(GLenum target, GLuint buffer);
    bool bufferData(GLenum target, GLsizeiptrARB size,
                    const GLvoid *data, GLenum usage);
    bool deleteBuffers(GLsizei n, const GLuint *buffers);
    bool pointParameterF(GLenum pname, GLfloat param);
    bool pointParameterFV(GLenum pname, GLfloat *param);

    private:
    typedef void (*GenBuffersPtr)(GLsizei n, GLuint *buffers);
    typedef void (*BindBuffersPtr)(GLenum target, GLuint buffer);
    typedef void (*BufferDataPtr)(GLenum target, GLsizeiptrARB size,
                                  const GLvoid *data, GLenum usage);
    typedef void (*DeleteBuffersPtr)(GLsizei n, const GLuint *buffers);
    typedef void (*PointParameterFPtr)(GLenum pname, GLfloat param);
    typedef void (*PointParameterFVPtr)(GLenum pname,
                                        const GLfloat *params);

    static void initBufferExtension(const QGLContext *context);
    template <typename ExtensionProto>
    static bool resolveExtension(const QGLContext *context,
                                 const char *extensionName,
                                 ExtensionProto *extensionPtr);

    static bool s_vboInitialized;
    static GenBuffersPtr        s_glGenBuffers;
    static BindBuffersPtr       s_glBindBuffer;
    static BufferDataPtr        s_glBufferData;
    static DeleteBuffersPtr     s_glDeleteBuffers;
    static PointParameterFPtr   s_glPointParameterf;
    static PointParameterFVPtr  s_glPointParameterfv;
};

struct Point
{
    Point();
    Point(float x, float y, float z,
          float r, float g, float b, float a = 1.0f);

    float x, y, z;
    float r, g, b, a;
    float padding[1]; // ATI suggests that vertices are aligned to 32 bytes
};

class PointBuffer : protected GLBufferExtension
{
public:
    PointBuffer(const QGLContext *context);
    ~PointBuffer();

    size_t size() const;

    void clear();

    void addPoint(const Point &point);

    void render();

private:
    GLvoid *vertexOffset(unsigned int offset);

    QVector<Point> m_pointList;
    GLuint m_glBuffer;
    bool m_dirty;
    GLuint m_cloudTexture;
};

#endif // POINTBUFFER_H
