#include <functional>

#include "unistd.h"

#include <GL/glew.h>

#include "klib/logger.h"
#include "klib/thread.h"

#include "window.h"
#include "shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "libkbvh2/bvh.h"
#include <tr1/memory>

#include "camera.h"

WindowX11 win;


Shader shader, shader1;
static GLuint vboVert, vboIndices;
static GLuint vao;

Camera cam(win);

float rotationAngle = 0.0f;

glm::mat4 model = glm::mat4(1.0);

float angle = 0.0;
float angleX = 0.0f;
float angleChngSpeed = 0.0f;
float angleChngSpeedX = 0.0f;

static short bvh_elements = 0;

bool gogogo = true;

Bvh* bvh;
int frame=1, frameChange=0;
glm::vec3 bvh_scale = glm::vec3(0.25, 0.25, 0.25);

const Logger& operator<<(const Logger& logger, const glm::vec4& vec)
{
    return klog << vec.x << vec.y << vec.z << vec.w;
}

const Logger& operator<<(const Logger& logger, const glm::mat4& mat)
{
    for( int i = 0; i < mat.length(); i++ )
        ((Logger&)logger).l() << mat[i];
    return logger;
}

void tmpProcess(JOINT*            joint,
                std::vector<glm::vec4>& vertices,
                std::vector<GLshort>&   indices,
                GLshort                 parentIndex = 0)
{
    // translated vertex is last column(row of transponse) joint's matrix
    glm::vec4 translatedVertex = joint->matrix[3];

    vertices.push_back(translatedVertex);

    GLshort myindex = vertices.size() - 1;

    if( parentIndex != myindex )
    {
        indices.push_back(parentIndex);
        indices.push_back(myindex);
    }

    for(auto& child : joint->children)
        tmpProcess(child, vertices, indices, myindex);
}

void update()
{
    if( frameChange )
        frame = frame + frameChange;
    else
        return;

    bvh->moveTo(frame % bvh->getNumFrames());

    std::vector<glm::vec4> vertices;
    std::vector<GLshort> bvhindices;

    tmpProcess((JOINT*)bvh->getRootJoint(), vertices, bvhindices);

    glBindBuffer(GL_ARRAY_BUFFER, vboVert);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void newinit()
{
    glEnable(GL_DEPTH_TEST);

    klog.l() << "UCITAVA BVH";

    bvh = new Bvh;
//    bvh->load("/home/edin/dev/data/02_03.bvh");
    bvh->load("../data/bvh/0008_ChaCha001.bvh");
//   bvh->load("../data/bvh/example3.bvh");

    bvh->testOutput();
    bvh->printJoint(bvh->getRootJoint());
    bvh->moveTo(frame++);

    std::vector<glm::vec4> vertices;
    std::vector<GLshort> bvhindices;

    tmpProcess((JOINT*)bvh->getRootJoint(), vertices, bvhindices);

    bvh_elements = bvhindices.size();

    glGenBuffers(1, &vboVert);
    glBindBuffer(GL_ARRAY_BUFFER, vboVert);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bvhindices[0]) * bvhindices.size(), &bvhindices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLint sLocPosition = shader1.attribute("position");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vboVert);
    glEnableVertexAttribArray(sLocPosition);
    glVertexAttribPointer(sLocPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);

    glBindVertexArray(0);
}

void display()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 mvp = cam.matrix() * glm::translate(model, glm::vec3(15, 0, 0));

    shader1.use();

    glm::mat4 _model = glm::scale(model, bvh_scale);

    mvp = cam.matrix() * _model;
    glUniformMatrix4fv(shader1.uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));

    update();
    glBindVertexArray(vao);

    glPointSize(5.0);
    glDrawElements(GL_LINES, bvh_elements, GL_UNSIGNED_SHORT, (GLvoid*)0);
    glLineWidth(3.0);
    glDrawElements(GL_POINTS, bvh_elements, GL_UNSIGNED_SHORT, (GLvoid*)0);

    glBindVertexArray(0);

    shader1.unuse();
}

void* chng(void*)
{
    while(true)
    {
        rotationAngle = ((int)rotationAngle + 5) % 360;
        usleep(1000000.0f / 24.0f);
        klog.i("t") << rotationAngle;
    }
}

void eventHandler(XEvent& event)
{

    switch(event.type)
    {
    case Expose:
        cam.aspect( (float)win.width / (float)win.height );
        break;
    default:
    case KeyPress:
    case KeyRelease:


        if( event.type == KeyPress )
        {
            klog << "XKeyEvent\t";
            char buffer[20];
            int bufsize= 20;
            KeySym key;
            XComposeStatus compose;
            int charcount = XLookupString((XKeyEvent*)&event, buffer, bufsize, &key, &compose);

            switch(key)
            {
            case XK_Left:
                if( angleChngSpeed > -80.0f )
                    angleChngSpeed -= 0.5f;

                break;
            case XK_Right:
                if( angleChngSpeed < 80.0f )
                    angleChngSpeed += 0.5f;
                break;
            case XK_Up:
                if( angleChngSpeedX < 80.f )
                    angleChngSpeedX += 0.5f;
                break;
            case XK_Down:
                if(angleChngSpeedX > -80.0f)
                    angleChngSpeedX -= 0.5f;
                break;
            case XK_b:
                frameChange++;
                klog.l() << "frame:" << frame;
                break;
            case XK_v:
                frameChange = 0;
                klog.l() << "frame:" << frame;
                break;
            case XK_c:
                frameChange--;
                klog.l() << "frame:" << frame;
                break;
            case XK_n:
                frameChange = 1;
                update();
                frameChange = 0;
                break;
            case XK_x:
                frameChange = -1;
                update();
                frameChange = 0;
            case XK_g:
                break;
            case XK_r:
            case XK_R:

                klog << "Stop";

                angleChngSpeed = angleChngSpeedX = 0.0f;
                break;
            case XK_q:
                exit(1);
                break;
            }
        }

        angle = ((int)(angle + angleChngSpeed)) % 360;
        angleX = ((int)(angleX + angleChngSpeedX)) % 360;

        model = glm::mat4(1.0);
        model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
        model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
        model = glm::rotate(model, angleX, glm::vec3(1, 0, 0));
    }
}

void* tp (void* )
{
    XEvent exppp;

    float fps = 48.0f;
    int step = int(1000000 / fps);

    while (1)
    {
        usleep (step);
        memset(&exppp, 0, sizeof(exppp));
        exppp.type = Expose;
        exppp.xexpose.window = win.win;
        XSendEvent(win.display,win.win,False,ExposureMask,&exppp);
        XFlush(win.display);
    }
    return NULL;
}

int main()
{
    XInitThreads();
    win.init();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDepthRange(0.0f, 1.0f);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    shader.loadShader("shader2");
    shader.attribute("position");
    shader.attribute("color");
    shader.attribute("vUV");
    shader.uniform("textureMap");
    shader.uniform("MVP");

    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    model = glm::translate(model, glm::vec3(0.0, 0.0, -2.0));

    shader1.loadShader("shader1");


    newinit();

    Thread t(tp);
    t.start();

    win.setDraw(&display);
    win.setEventHandler(&eventHandler);

    klog << "\n";
    win.main_loop();

    return 0;
}
