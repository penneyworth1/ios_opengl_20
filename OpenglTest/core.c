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
GLuint _translationUniform;
GLuint _rotationUniform;

GLfloat screenWidthInPixels;
GLfloat screenHeightInPixels;

float timeDiff = 0;
float lastTimeStamp = 0;

float translationMatrix[16] = {0};
float angle = 0;
float rotAxis[3] = {.5,1,.5};
float quat[4] = {0}; //rotation quaternion
float rotationMatrix[16] = {0};
float xx,yy,zz,xy,xz,yz,wx,wy,wz;

typedef struct
{
    float Position[3];
    float Color[4];
} Vertex;

const Vertex Vertices[] = {
    {{1, -1, 1}, {1, 0, 0, 1}},
    {{1, 1, 1}, {0, 1, 0, 1}},
    {{-1, 1, 1}, {0, 0, 1, 1}},
    {{-1, -1, 1}, {1, 1, 0, 1}},
    {{1, -1, -1}, {1, 0, 1, 1}},
    {{1, 1, -1}, {1, 1, 1, 1}},
    {{-1, 1, -1}, {0, 1, 1, 1}},
    {{-1, -1, -1}, {0, 0, 0, 1}}
};

const GLubyte Indices[] = {
    // Front
    0, 1, 2,
    2, 3, 0,
    // Back
    4, 6, 5,
    4, 7, 6,
    // Left
    2, 7, 3,
    7, 6, 2,
    // Right
    0, 4, 1,
    4, 1, 5,
    // Top
    6, 2, 1,
    1, 6, 5,
    // Bottom
    0, 3, 7,
    0, 7, 4
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
void setTranslationMatrix(float* m, float tx, float ty, float tz)
{ //glsl mat4 structure expects entries in the order of columns
    m[0]  = 1; m[4]  = 0; m[8]  = 0;  m[12] = tx;
    m[1]  = 0; m[5]  = 1; m[9]  = 0;  m[13] = ty;
    m[2]  = 0; m[6]  = 0; m[10] = 1;  m[14] = tz;
    m[3]  = 0; m[7]  = 0; m[11] = 0;  m[15] =  1;
}
void setRotationMatrx(float* m, float* q)
{
    float norm = sqrtf(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
    q[0] =  q[0] / norm;
    q[1] =  q[1] / norm;
    q[2] =  q[2] / norm;
    q[3] =  q[3] / norm;
    
    xx = q[0] * q[0];
    yy = q[1] * q[1];
    zz = q[2] * q[2];
    xy = q[0] * q[1];
    xz = q[0] * q[2];
    yz = q[1] * q[2];
    wx = q[3] * q[0];
    wy = q[3] * q[1];
    wz = q[3] * q[2];
    
    m[0]  = 1 - 2 * (yy + zz); m[4] =     2 * (xy - wz); m[8]  =     2 * (xz + wy);  m[12] = 0;
    m[1]  =     2 * (xy + wz); m[5] = 1 - 2 * (xx + zz); m[9]  =     2 * (yz - wx);  m[13] = 0;
    m[2]  =     2 * (xz - wy); m[6] =     2 * (yz + wx); m[10] = 1 - 2 * (xx + yy);  m[14] = 0;
    m[3]  =                 0; m[7] =                 0; m[11] = 0                ;  m[15] = 1;

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
    "attribute vec4 Position; \n"
    "attribute vec4 SourceColor; \n"
    "varying vec4 DestinationColor; \n"
    "uniform mat4 Projection; \n"
    "uniform mat4 Translation; \n"
    "uniform mat4 Rotation; \n"
    "void main(void) \n"
    "{ \n"
    "    DestinationColor = SourceColor; \n"
    "    gl_Position = Projection * Translation * Rotation * Position; \n"
    "} \0";
    
    GLchar fragmentShaderString[] =
    "varying lowp vec4 DestinationColor; \n"
    "void main(void) \n"
    "{ \n"
    "    gl_FragColor = DestinationColor; \n"
    "} \0";
    
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
    _translationUniform = glGetUniformLocation(programHandle, "Translation");
    _projectionUniform = glGetUniformLocation(programHandle, "Projection");
    _rotationUniform = glGetUniformLocation(programHandle, "Rotation");
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
    
    float perspectiveMatrix[16] = {0};
    float fov=30.0f; // in degrees
    float aspect=((float)screenWidthInPixels)/screenHeightInPixels;
    float znear=10.0f;
    float zfar=100.0f;
    buildPerspProjMat(perspectiveMatrix, fov, aspect, znear, zfar);
    glUniformMatrix4fv(_projectionUniform, 1, 0, perspectiveMatrix);
    glViewport(0, 0, screenWidthInPixels, screenHeightInPixels);
    
    glClearColor(0, 104.0/255.0, 55.0/255.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    
    //Normalize rot axis
    float axisNorm = sqrtf(rotAxis[0]*rotAxis[0] + rotAxis[1]*rotAxis[1] + rotAxis[2]*rotAxis[2]);
    rotAxis[0] =  rotAxis[0] / axisNorm;
    rotAxis[1] =  rotAxis[1] / axisNorm;
    rotAxis[2] =  rotAxis[2] / axisNorm;
    
}

void renderScene(double timeStamp)
{
    timeDiff = timeStamp - lastTimeStamp;
    lastTimeStamp = timeStamp;
 
    float sinHalfAngle = sinf(angle/2);
    quat[0] = rotAxis[0] * sinHalfAngle;
    quat[1] = rotAxis[1] * sinHalfAngle;
    quat[2] = rotAxis[2] * sinHalfAngle;
    quat[3] = cosf(angle/2);
    setRotationMatrx(rotationMatrix, quat);
    angle += 2.0f * timeDiff; if(angle > 2*pi) angle = 0;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    setTranslationMatrix(translationMatrix, 0, 0, -13);// + 4*sin(timeStamp));
    glUniformMatrix4fv(_translationUniform, 1, 0, translationMatrix);
    glUniformMatrix4fv(_rotationUniform, 1, 0, rotationMatrix);
    
    glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
    
    glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_BYTE, 0);
    
    
}



