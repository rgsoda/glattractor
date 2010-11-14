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

    private:
    static void initBufferExtension(const QGLContext *context);
    template <typename ExtensionProto>
    static bool resolveExtension(const QGLContext *context,
                                 const char *extensionName,
                                 ExtensionProto *extensionPtr);

    static bool s_vboInitialized;
    static PFNGLGENBUFFERSARBPROC     s_glGenBuffers;
    static PFNGLBINDBUFFERARBPROC     s_glBindBuffer;
    static PFNGLBUFFERDATAARBPROC     s_glBufferData;
    static PFNGLDELETEBUFFERSARBPROC  s_glDeleteBuffers;
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
};

#endif // POINTBUFFER_H
