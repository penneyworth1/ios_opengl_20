//
//  core.c
//  OpenglTest
//
//  Created by Steven Stewart on 12/24/14.
//  Copyright (c) 2014 Maaz Kamani. All rights reserved.
//

#include "core.h"

#define pi 3.14159265358979323846264338327
#define PI_OVER_360 0.0087266

GLuint _positionSlot;
GLuint _colorSlot;
GLuint _projectionUniform;

GLfloat screenWidthInPixels;
GLfloat screenHeightInPixels;

typedef struct
{
    float Position[3];
    float Color[4];
} Vertex;

const Vertex Vertices[] =
{
    {{1, -1, -7}, {1, 0, 0, 1}},
    {{1, 1, -7}, {0, 1, 0, 1}},
    {{-1, 1, -7}, {0, 0, 1, 1}},
    {{-1, -1, -7}, {0, 0, 0, 1}}
};

const GLubyte Indices[] =
{
    0, 1, 2,
    2, 3, 0
};

void buildPerspProjMat(float* m, float fov, float aspect, float znear, float zfar)
{
    float xymax = znear * tan(fov * PI_OVER_360);
    float ymin = -xymax;
    float xmin = -xymax;
    float width = xymax - xmin;
    float height = xymax - ymin;
    float depth = zfar - znear;
    float q = -(zfar + znear) / depth;
    float qn = -2 * (zfar * znear) / depth;
    float w = 2 * znear / width;
    w = w / aspect;
    float h = 2 * znear / height;
    m[0]  = w; m[1]  = 0; m[2]  = 0;  m[3]  =  0;
    m[4]  = 0; m[5]  = h; m[6]  = 0;  m[7]  =  0;
    m[8]  = 0; m[9]  = 0; m[10] = q;  m[11] = -1;
    m[12] = 0; m[13] = 0; m[14] = qn; m[15] =  0;
}

GLuint compileShader(const GLchar* shaderSourceString, GLenum shaderType)
{
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &shaderSourceString, NULL);
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE)
    {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        printf(messages);
        exit(1);
    }
    
    return shaderHandle;
}
void compileShaders()
{
    GLchar vertexShaderString[] =
    "attribute vec4 Position;                    \n"
    "attribute vec4 SourceColor;                 \n"
    "varying vec4 DestinationColor;              \n"
    "uniform mat4 Projection;                    \n"
    "void main(void)                             \n"
    "{                                           \n"
    "    DestinationColor = SourceColor;         \n"
    "    gl_Position = Projection * Position;    \n"
    "}                                           \0";
    
    GLchar fragmentShaderString[] =
    "varying lowp vec4 DestinationColor;  \n"
    "void main(void)                      \n"
    "{                                    \n"
    "    gl_FragColor = DestinationColor; \n"
    "}                                    \0";
    
    GLuint compiledVertexShaderReference = compileShader(vertexShaderString, GL_VERTEX_SHADER);
    GLuint compiledFragmentShaderReference = compileShader(fragmentShaderString, GL_FRAGMENT_SHADER);
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, compiledVertexShaderReference);
    glAttachShader(programHandle, compiledFragmentShaderReference);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE)
    {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        printf(messages);
        exit(1);
    }
    
    glUseProgram(programHandle);
    
    _positionSlot = glGetAttribLocation(programHandle, "Position");
    _colorSlot = glGetAttribLocation(programHandle, "SourceColor");
    _projectionUniform = glGetUniformLocation(programHandle, "Projection");
    glEnableVertexAttribArray(_positionSlot);
    glEnableVertexAttribArray(_colorSlot);
    
    
}

void setupVBOs()
{
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    
}


void initView(float screenWidthInPixelsPar, float screenHeightInPixelsPar)
{
    compileShaders();
    setupVBOs();
    
    screenWidthInPixels = screenWidthInPixelsPar;
    screenHeightInPixels = screenHeightInPixelsPar;
    
    float m[16] = {0};
    float fov=60.0f; // in degrees
    float aspect=((float)screenWidthInPixels)/screenHeightInPixels;
    float znear=1.0f;
    float zfar=100.0f;
    buildPerspProjMat(m, fov, aspect, znear, zfar);
    glUniformMatrix4fv(_projectionUniform, 1, 0, m);
    glViewport(0, 0, screenWidthInPixels, screenHeightInPixels);
    
    glClearColor(0, 104.0/255.0, 55.0/255.0, 1.0);
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
    
    glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_BYTE, 0);
}
