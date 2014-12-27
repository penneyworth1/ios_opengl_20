//
//  core.h
//  OpenglTest
//
//  Created by Steven Stewart on 12/24/14.
//  Copyright (c) 2014 Maaz Kamani. All rights reserved.
//

#ifndef __OpenglTest__core__
#define __OpenglTest__core__

/////////iOS-specific inludes
#include <OpenGLES/ES2/gl.h>
/////////

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#endif /* defined(__OpenglTest__core__) */

void buildPerspProjMat(float *m, float fov, float aspect, float znear, float zfar);
void initView(float screenWidthInPixelsPar, float screenHeightInPixelsPar);
void renderScene(long timeDiffMillies);
void compileShaders();
GLuint compileShader(const GLchar* shaderSourceString, GLenum shaderType);