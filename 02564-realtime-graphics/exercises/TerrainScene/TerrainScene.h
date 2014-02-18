#ifndef __TERRAIN_SCENE_H
#define __TERRAIN_SCENE_H

#include <GLGraphics/GLHeader.h>
#include <GLGraphics/ThreeDObject.h>
#include <GLGraphics/ShaderProgram.h>
#include "GBuffer.h"

class TerrainScene : public QGLWidget
{
    Q_OBJECT

    QTimer *timer;

    float ax, ay, dist;
    float ang_x;
    float ang_y;
    int mouse_x0,mouse_y0;
    int window_width;
    int window_height;

    GLGraphics::ShaderProgramDraw shader;
    GLGraphics::ThreeDObject cow;
    GLuint m_vertexBuffer;

public:
    TerrainScene( const QGLFormat& format, QWidget* parent = 0);

public slots:
    void animate();

protected:
    void set_light_and_camera(GLGraphics::ShaderProgramDraw& shader_prog);
    void render_direct(bool reload);
    void render_direct_wireframe(bool reload);
    void render_direct_fur(bool reload);
    void render_to_gbuffer(GBuffer& gbuffer, bool reload);
    void render_deferred_toon(bool reload);
    void render_deferred_ssao(bool reload);
#ifdef SOLUTION_CODE
    void render_indirect();
#endif
     void initializeGL();
     void resizeGL( int w, int h );
     void paintGL();
     void mousePressEvent(QMouseEvent *);
     void mouseReleaseEvent(QMouseEvent *);
     void mouseMoveEvent(QMouseEvent *);
     void keyPressEvent(QKeyEvent *);
     void keyReleaseEvent(QKeyEvent *);
};


#endif // INTROSCENE_H
