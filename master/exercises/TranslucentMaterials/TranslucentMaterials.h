#ifndef __TERRAIN_SCENE_H
#define __TERRAIN_SCENE_H

#include <GLGraphics/GLHeader.h>
#include <GLGraphics/ThreeDObject.h>
#include <GLGraphics/ShaderProgram.h>
#include "GBuffer.h"
#include <QGLFormat>
#include "Utils/performancetimer.h"

const int RENDER_MODES = 4;
const int RENDER_METHODS = 2;

class TranslucentParameters
{

public:
    TranslucentParameters::TranslucentParameters() :
        circleradius(1.f),
        samples(10),
        maxsamples(10000),
        epsilon_gbuffer(0.004),
        epsilon_combination(0.0005),
        shadow_bias(0.0003),
        debugOverlayVisible(false), LOD(0.0f),  currentFlags(0), gamma(1.8f), environment(false),
        a(1.0f),
        b(1.0f)
    {}

    enum
    {
        SAMPLES_CHANGED = 0x01,
        INITIALIZATION = 0x02
    };

    int currentFlags;

    float circleradius;
    float epsilon_gbuffer; // avoids artifacts when sampling from gbuffer
    float epsilon_combination; // avoids artifacts when sampling from cubebuffer
    float shadow_bias; // avoids shadow acne
    int samples;
    int maxsamples;

    bool debugOverlayVisible;
    bool environment;

    float LOD;
    float gamma;
    float a,b;
};

class TranslucentMaterials : public QGLWidget
{
    Q_OBJECT

    QTimer *timer;

    float ax, ay, dist;
    float ang_x;
    float ang_y;
    int mouse_x0,mouse_y0;
    int window_width;
    int window_height;

    bool isVertexMode;
    bool isShadow;
    bool isGridVisible;
    bool areAxesVisible;
    int frame;

    GLGraphics::ShaderProgramDraw shader;
    GLGraphics::ThreeDObject cow;
    GLuint m_vertexBuffer;
    CGLA::Vec4f clearColor;
    TranslucentParameters * params;
    PerformanceTimer performanceTimer;

    Mesh::Texture * skybox;
    GLGraphics::ThreeDObject * skybox_cube;

public:

    TranslucentMaterials(QWidget* parent = 0);
    QImage* takeScreenshot();
    CGLA::Vec4f getClearColor() const;
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    GLGraphics::ThreeDObject * getObject(std::string name);
    TranslucentParameters * getParameters();
    enum RenderMode {DRAW_JENSEN=0, DRAW_BETTER=1, DRAW_DIRECTIONAL=2};
    enum RenderMethod {BRUTE_FORCE = 0, CUBEMAP_BASE = 1};
    GLGraphics::ThreeDObject *getDefaultObject();
    void setRenderMode(RenderMode mode);
    GLGraphics::ThreeDObject *setObject(QString & object);

public slots:
    void animate();
    void setClearColor(CGLA::Vec4f & color);
    void setUserPosition(CGLA::Vec3f & position);
    void setUserDirection(CGLA::Vec3f & direction);
    void setLightIntensity(float intensity);
    void setShadows(bool shadows);
    void setVertexPixelMode(bool isVertex);
    void setGridVisible(bool isVisible);
    void setAxesVisible(bool areVisible);

signals:
    void userPositionChanged(CGLA::Vec3f & newPosition);
    void userDirectionChanged(CGLA::Vec3f & newDirection);
    void timeMeasurement(int millis);

protected:
    void set_light_and_camera(GLGraphics::ShaderProgramDraw& shader_prog);
    void render_dirss(bool reload, GLGraphics::ShaderProgramDraw & render_to_cubemap);

    void render_to_gbuffer(GBuffer& gbuffer, bool reload);
    void draw_axes(bool reload);
    void draw_grid(bool reload);
    void draw_bounding_boxes(bool reload);
    void getDiscPoints(std::vector<CGLA::Vec2f> * points, const int n, const int m);
    void getDiscPoints(std::vector<CGLA::Vec2f> * points, const int n, const int m, float sigma_tr, float radius);
    void getDiscPointsSpectral(std::vector<CGLA::Vec3f> * points, const int n, const int m, CGLA::Vec3f sigma_tr, float radius);

    void initialize();
     void initializeGL();
     void resizeGL( int w, int h );
     void paintGL();
     void mousePressEvent(QMouseEvent *);
     void mouseReleaseEvent(QMouseEvent *);
     void mouseMoveEvent(QMouseEvent *);
     RenderMode render_mode;
     RenderMethod render_method;
     std::vector<GLGraphics::ThreeDObject*> objectPool;
     GLGraphics::ThreeDObject * currentObject;



     int currentFrame;
};

struct CompareThreeD {
    CompareThreeD( std::string s ) : toFind(s) { }

    bool operator() (GLGraphics::ThreeDObject *n)
        { return strcmp(n->name.c_str(),toFind.c_str()) == 0; }

    std::string toFind;
};

#endif // INTROSCENE_H
