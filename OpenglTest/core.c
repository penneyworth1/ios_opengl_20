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
GLuint _normalSlot;
GLuint uniform_mvpMatrix;
GLuint uniform_rotMatrix;
GLuint uniform_ambientMaterial;
GLuint uniform_lightPosition;
GLuint uniform_specularMaterial;
GLuint uniform_shininess;
GLuint uniform_lightDirection;

GLfloat screenWidthInPixels;
GLfloat screenHeightInPixels;

float timeDiff = 0;
float lastTimeStamp = 0;

float modelMatrix[16] = {0};
float perspectiveMatrix[16] = {0};
float translationMatrix[16] = {0};
float rotationMatrix[16] = {0};
float transRotMatrix[16] = {0}; //To store the result of the translation matrix multiplied by the rotation matrix
float angle = 0;
float rotAxis[3] = {1,5,.5};
float quat[4] = {0}; //rotation quaternion
float xx,yy,zz,xy,xz,yz,wx,wy,wz;
float lightPosition[3] = {0,0,-1};
float ambientMaterial[3] = {0,0.1,0.1};
float specularMaterial[3] = {0,1,0.7};
float shininess = 15;

typedef struct
{
    float Position[3];
    float Normal[3];
    float DiffuseMaterial[3];
} Vertex;

const Vertex Vertices[] = {
    
    {{0.0769, -0.6768, 2.7951},{-0.9762,0.1267,0.1760},{255,152,0}},
        {{0.1266, -0.7442, 2.8066},{-0.1966,-0.9612,0.1934},{255,152,0}},
        {{0.1514, -0.6745, 2.8661},{-0.1963,-0.0009,0.9805},{255,152,0}},
        {{0.0828, -0.7244, 2.8009},{-0.8041,-0.5798,0.1311},{255,152,0}},
        {{0.1390, -0.7238, 2.8533},{-0.1404,-0.5976,0.7894},{255,152,0}},
        {{0.1008, -0.6693, 2.8444},{-0.6604,0.1965,0.7247},{255,152,0}},
        {{0.0928, -0.7162, 2.8465},{-0.6129,-0.3697,0.6983},{255,152,0}},
        {{0.0769, -0.6768, -2.5026},{-0.9762,0.1267,-0.1760},{255,152,0}},
    {{0.1514, -0.6745, -2.5736},{-0.1963,-0.0009,-0.9805},{255,152,0}},
    {{0.1266, -0.7442, -2.5141},{-0.1966,-0.9612,-0.1934},{255,152,0}},
    {{0.1008, -0.6693, -2.5519},{-0.6604,0.1965,-0.7247},{255,152,0}},
    {{0.1390, -0.7238, -2.5608},{-0.1404,-0.5976,-0.7894},{255,152,0}},
    {{0.0828, -0.7244, -2.5083},{-0.8041,-0.5798,-0.1311},{255,152,0}},
    {{0.0928, -0.7162, -2.5539},{-0.6129,-0.3697,-0.6983},{255,152,0}},
    {{1.8486, -0.6745, -2.5736},{0.1963,-0.0009,-0.9805},{255,152,0}},
    {{1.9231, -0.6768, -2.5026},{0.9762,0.1267,-0.1760},{255,152,0}},
    {{1.8734, -0.7442, -2.5141},{0.1966,-0.9612,-0.1934},{255,152,0}},
    {{1.8992, -0.6693, -2.5519},{0.6604,0.1965,-0.7247},{255,152,0}},
    {{1.9172, -0.7244, -2.5083},{0.8041,-0.5798,-0.1311},{255,152,0}},
    {{1.8610, -0.7238, -2.5608},{0.1404,-0.5976,-0.7894},{255,152,0}},
    {{1.9072, -0.7162, -2.5539},{0.6130,-0.3697,-0.6983},{255,152,0}},
    {{1.9231, -0.6768, 2.7951},{0.9762,0.1267,0.1760},{255,152,0}},
    {{1.8486, -0.6745, 2.8661},{0.1963,-0.0009,0.9805},{255,152,0}},
    {{1.8734, -0.7442, 2.8066},{0.1966,-0.9612,0.1934},{255,152,0}},
    {{1.8992, -0.6693, 2.8444},{0.6604,0.1965,0.7247},{255,152,0}},
    {{1.8610, -0.7238, 2.8533},{0.1404,-0.5976,0.7894},{255,152,0}},
    {{1.9172, -0.7244, 2.8009},{0.8041,-0.5798,0.1311},{255,152,0}},
    {{1.9072, -0.7162, 2.8465},{0.6129,-0.3697,0.6983},{255,152,0}},
    {{0.3621, 0.0070, 2.7575},{-0.1361,0.3250,0.9358},{255,152,0}},
    {{0.3804, 0.0767, 2.6758},{-0.1513,0.9748,0.1639},{255,152,0}},
    {{0.2890, 0.0093, 2.6858},{-0.8766,0.4518,0.1658},{255,152,0}},
    {{0.3712, 0.0563, 2.7290},{-0.0623,0.7869,0.6139},{255,152,0}},
    {{0.3244, 0.0570, 2.6808},{-0.5821,0.8073,0.0967},{255,152,0}},
    {{0.3122, 0.0145, 2.7354},{-0.6641,0.3794,0.6442},{255,152,0}},
    {{0.3261, 0.0565, 2.7261},{-0.5041,0.7478,0.4321},{255,152,0}},
    {{0.2890, 0.0093, -2.3932},{-0.8766,0.4518,-0.1658},{255,152,0}},
    {{0.3804, 0.0767, -2.3833},{-0.1513,0.9748,-0.1639},{255,152,0}},
    {{0.3621, 0.0070, -2.4650},{-0.1361,0.3250,-0.9358},{255,152,0}},
    {{0.3244, 0.0570, -2.3883},{-0.5821,0.8073,-0.0967},{255,152,0}},
    {{0.3712, 0.0563, -2.4365},{-0.0623,0.7869,-0.6139},{255,152,0}},
    {{0.3122, 0.0145, -2.4429},{-0.6641,0.3794,-0.6442},{255,152,0}},
    {{0.3261, 0.0565, -2.4336},{-0.5041,0.7478,-0.4321},{255,152,0}},
    {{1.6379, 0.0070, -2.4650},{0.1361,0.3250,-0.9358},{255,152,0}},
    {{1.6196, 0.0767, -2.3833},{0.1513,0.9748,-0.1639},{255,152,0}},
    {{1.7110, 0.0093, -2.3932},{0.8766,0.4518,-0.1658},{255,152,0}},
    {{1.6288, 0.0563, -2.4365},{0.0623,0.7869,-0.6139},{255,152,0}},
    {{1.6756, 0.0570, -2.3883},{0.5821,0.8073,-0.0967},{255,152,0}},
    {{1.6878, 0.0145, -2.4429},{0.6641,0.3794,-0.6442},{255,152,0}},
    {{1.6739, 0.0565, -2.4336},{0.5041,0.7478,-0.4321},{255,152,0}},
    {{1.7110, 0.0093, 2.6858},{0.8766,0.4518,0.1658},{255,152,0}},
    {{1.6196, 0.0767, 2.6758},{0.1513,0.9748,0.1639},{255,152,0}},
    {{1.6379, 0.0070, 2.7575},{0.1361,0.3250,0.9358},{255,152,0}},
    {{1.6756, 0.0570, 2.6808},{0.5821,0.8073,0.0967},{255,152,0}},
    {{1.6288, 0.0563, 2.7290},{0.0623,0.7869,0.6139},{255,152,0}},
    {{1.6878, 0.0145, 2.7354},{0.6641,0.3794,0.6442},{255,152,0}},
    {{1.6739, 0.0565, 2.7261},{0.5041,0.7478,0.4321},{255,152,0}},
    {{0.0769, -0.6768, 2.7951},{-0.9762,0.1267,0.1760},{255,152,0}},
    {{0.1266, -0.7442, 2.8066},{-0.1966,-0.9612,0.1934},{255,152,0}},
    {{0.1514, -0.6745, 2.8661},{-0.1963,-0.0009,0.9805},{255,152,0}},
    {{0.0828, -0.7244, 2.8009},{-0.8041,-0.5798,0.1311},{255,152,0}},
    {{0.1390, -0.7238, 2.8533},{-0.1404,-0.5976,0.7894},{255,152,0}},
    {{0.1008, -0.6693, 2.8444},{-0.6604,0.1965,0.7247},{255,152,0}},
    {{0.0928, -0.7162, 2.8465},{-0.6129,-0.3697,0.6983},{255,152,0}},
    {{0.0769, -0.6768, -2.5026},{-0.9762,0.1267,-0.1760},{255,152,0}},
    {{0.1514, -0.6745, -2.5736},{-0.1963,-0.0009,-0.9805},{255,152,0}},
    {{0.1266, -0.7442, -2.5141},{-0.1966,-0.9612,-0.1934},{255,152,0}},
    {{0.1008, -0.6693, -2.5519},{-0.6604,0.1965,-0.7247},{255,152,0}},
    {{0.1390, -0.7238, -2.5608},{-0.1404,-0.5976,-0.7894},{255,152,0}},
    {{0.0828, -0.7244, -2.5083},{-0.8041,-0.5798,-0.1311},{255,152,0}},
    {{0.0928, -0.7162, -2.5539},{-0.6129,-0.3697,-0.6983},{255,152,0}},
    {{1.8486, -0.6745, -2.5736},{0.1963,-0.0009,-0.9805},{255,152,0}},
    {{1.9231, -0.6768, -2.5026},{0.9762,0.1267,-0.1760},{255,152,0}},
    {{1.8734, -0.7442, -2.5141},{0.1966,-0.9612,-0.1934},{255,152,0}},
    {{1.8992, -0.6693, -2.5519},{0.6604,0.1965,-0.7247},{255,152,0}},
    {{1.9172, -0.7244, -2.5083},{0.8041,-0.5798,-0.1311},{255,152,0}},
    {{1.8610, -0.7238, -2.5608},{0.1404,-0.5976,-0.7894},{255,152,0}},
    {{1.9072, -0.7162, -2.5539},{0.6130,-0.3697,-0.6983},{255,152,0}},
    {{1.9231, -0.6768, 2.7951},{0.9762,0.1267,0.1760},{255,152,0}},
    {{1.8486, -0.6745, 2.8661},{0.1963,-0.0009,0.9805},{255,152,0}},
    {{1.8734, -0.7442, 2.8066},{0.1966,-0.9612,0.1934},{255,152,0}},
    {{1.8992, -0.6693, 2.8444},{0.6604,0.1965,0.7247},{255,152,0}},
    {{1.8610, -0.7238, 2.8533},{0.1404,-0.5976,0.7894},{255,152,0}},
    {{1.9172, -0.7244, 2.8009},{0.8041,-0.5798,0.1311},{255,152,0}},
    {{1.9072, -0.7162, 2.8465},{0.6129,-0.3697,0.6983},{255,152,0}},
    {{0.3621, 0.0070, 2.7575},{-0.1361,0.3250,0.9358},{255,152,0}},
    {{0.3804, 0.0767, 2.6758},{-0.1513,0.9748,0.1639},{255,152,0}},
    {{0.2890, 0.0093, 2.6858},{-0.8766,0.4518,0.1658},{255,152,0}},
    {{0.3712, 0.0563, 2.7290},{-0.0623,0.7869,0.6139},{255,152,0}},
    {{0.3244, 0.0570, 2.6808},{-0.5821,0.8073,0.0967},{255,152,0}},
    {{0.3122, 0.0145, 2.7354},{-0.6641,0.3794,0.6442},{255,152,0}},
    {{0.3261, 0.0565, 2.7261},{-0.5041,0.7478,0.4321},{255,152,0}},
    {{0.2890, 0.0093, -2.3932},{-0.8766,0.4518,-0.1658},{255,152,0}},
    {{0.3804, 0.0767, -2.3833},{-0.1513,0.9748,-0.1639},{255,152,0}},
    {{0.3621, 0.0070, -2.4650},{-0.1361,0.3250,-0.9358},{255,152,0}},
    {{0.3244, 0.0570, -2.3883},{-0.5821,0.8073,-0.0967},{255,152,0}},
    {{0.3712, 0.0563, -2.4365},{-0.0623,0.7869,-0.6139},{255,152,0}},
    {{0.3122, 0.0145, -2.4429},{-0.6641,0.3794,-0.6442},{255,152,0}},
    {{0.3261, 0.0565, -2.4336},{-0.5041,0.7478,-0.4321},{255,152,0}},
    {{1.6379, 0.0070, -2.4650},{0.1361,0.3250,-0.9358},{255,152,0}},
    {{1.6196, 0.0767, -2.3833},{0.1513,0.9748,-0.1639},{255,152,0}},
    {{1.7110, 0.0093, -2.3932},{0.8766,0.4518,-0.1658},{255,152,0}},
    {{1.6288, 0.0563, -2.4365},{0.0623,0.7869,-0.6139},{255,152,0}},
    {{1.6756, 0.0570, -2.3883},{0.5821,0.8073,-0.0967},{255,152,0}},
    
    {{1.6878, 0.0145, -2.4429},{0.6641,0.3794,-0.6442},{255,152,0}},
    
    {{1.6739, 0.0565, -2.4336},{0.5041,0.7478,-0.4321},{255,152,0}},
    
    {{1.7110, 0.0093, 2.6858},{0.8766,0.4518,0.1658},{255,152,0}},
    
    {{1.6196, 0.0767, 2.6758},{0.1513,0.9748,0.1639},{255,152,0}},
    
    {{1.6379, 0.0070, 2.7575},{0.1361,0.3250,0.9358},{255,152,0}},
    
    {{1.6756, 0.0570, 2.6808},{0.5821,0.8073,0.0967},{255,152,0}},
    
    {{1.6288, 0.0563, 2.7290},{0.0623,0.7869,0.6139},{255,152,0}},
    
    {{1.6878, 0.0145, 2.7354},{0.6641,0.3794,0.6442},{255,152,0}},
    
    {{1.6739, 0.0565, 2.7261},{0.5041,0.7478,0.4321},{255,152,0}},
    
    {{-1.9281, -0.6768, 2.7710},{-0.9762,0.1267,0.1760},{255,152,0}},
    
    {{-1.8783, -0.7442, 2.7826},{-0.1966,-0.9612,0.1934},{255,152,0}},
    
    {{-1.8536, -0.6745, 2.8421},{-0.1963,-0.0009,0.9805},{255,152,0}},
    
    {{-1.9221, -0.7244, 2.7768},{-0.8041,-0.5798,0.1311},{255,152,0}},
    
    {{-1.8660, -0.7238, 2.8293},{-0.1404,-0.5976,0.7894},{255,152,0}},
    
    {{-1.9042, -0.6693, 2.8203},{-0.6604,0.1965,0.7247},{255,152,0}},
    
    {{-1.9122, -0.7162, 2.8224},{-0.6129,-0.3697,0.6983},{255,152,0}},
    
    {{-1.9281, -0.6768, -2.5266},{-0.9762,0.1267,-0.1760},{255,152,0}},
    
    {{-1.8536, -0.6745, -2.5976},{-0.1963,-0.0009,-0.9805},{255,152,0}},
    
    {{-1.8783, -0.7442, -2.5382},{-0.1966,-0.9612,-0.1934},{255,152,0}},
    
    {{-1.9042, -0.6693, -2.5759},{-0.6604,0.1965,-0.7247},{255,152,0}},
    
    {{-1.8660, -0.7238, -2.5848},{-0.1404,-0.5976,-0.7894},{255,152,0}},
    
    {{-1.9221, -0.7244, -2.5324},{-0.8041,-0.5798,-0.1311},{255,152,0}},
    
    {{-1.9122, -0.7162, -2.5780},{-0.6129,-0.3697,-0.6983},{255,152,0}},
    
    {{-0.1564, -0.6745, -2.5976},{0.1963,-0.0009,-0.9805},{255,152,0}},
    
    {{-0.0819, -0.6768, -2.5266},{0.9762,0.1267,-0.1760},{255,152,0}},
    
    {{-0.1316, -0.7442, -2.5382},{0.1966,-0.9612,-0.1934},{255,152,0}},
    
    {{-0.1058, -0.6693, -2.5759},{0.6604,0.1965,-0.7247},{255,152,0}},
    
    {{-0.0878, -0.7244, -2.5324},{0.8041,-0.5798,-0.1311},{255,152,0}},
    
    {{-0.1440, -0.7238, -2.5848},{0.1404,-0.5976,-0.7894},{255,152,0}},
    
    {{-0.0977, -0.7162, -2.5780},{0.6130,-0.3697,-0.6983},{255,152,0}},
    
    {{-0.0819, -0.6768, 2.7710},{0.9762,0.1267,0.1760},{255,152,0}},
    
    {{-0.1564, -0.6745, 2.8421},{0.1963,-0.0009,0.9805},{255,152,0}},
    
    {{-0.1316, -0.7442, 2.7826},{0.1966,-0.9612,0.1934},{255,152,0}},
    
    {{-0.1058, -0.6693, 2.8203},{0.6604,0.1965,0.7247},{255,152,0}},
    
    {{-0.1440, -0.7238, 2.8293},{0.1404,-0.5976,0.7894},{255,152,0}},
    
    {{-0.0878, -0.7244, 2.7768},{0.8041,-0.5798,0.1311},{255,152,0}},
    
    {{-0.0977, -0.7162, 2.8224},{0.6129,-0.3697,0.6983},{255,152,0}},
    
    {{-1.6429, 0.0070, 2.7335},{-0.1361,0.3250,0.9358},{255,152,0}},
    
    {{-1.6246, 0.0767, 2.6518},{-0.1513,0.9748,0.1639},{255,152,0}},
    
    {{-1.7160, 0.0093, 2.6617},{-0.8766,0.4518,0.1658},{255,152,0}},
    
    {{-1.6337, 0.0563, 2.7049},{-0.0623,0.7869,0.6139},{255,152,0}},
    
    {{-1.6806, 0.0570, 2.6568},{-0.5821,0.8073,0.0967},{255,152,0}},
    
    {{-1.6928, 0.0145, 2.7114},{-0.6641,0.3794,0.6442},{255,152,0}},
    
    {{-1.6788, 0.0565, 2.7021},{-0.5041,0.7478,0.4321},{255,152,0}},
    
    {{-1.7160, 0.0093, -2.4173},{-0.8766,0.4518,-0.1658},{255,152,0}},
    
    {{-1.6246, 0.0767, -2.4073},{-0.1513,0.9748,-0.1639},{255,152,0}},
    
    {{-1.6429, 0.0070, -2.4890},{-0.1361,0.3250,-0.9358},{255,152,0}},
    
    {{-1.6806, 0.0570, -2.4123},{-0.5821,0.8073,-0.0967},{255,152,0}},
    
    {{-1.6337, 0.0563, -2.4605},{-0.0623,0.7869,-0.6139},{255,152,0}},
    
    {{-1.6928, 0.0145, -2.4669},{-0.6641,0.3794,-0.6442},{255,152,0}},
    
    {{-1.6788, 0.0565, -2.4576},{-0.5041,0.7478,-0.4321},{255,152,0}},
    
    {{-0.3671, 0.0070, -2.4890},{0.1361,0.3250,-0.9358},{255,152,0}},
    
    {{-0.3854, 0.0767, -2.4073},{0.1513,0.9748,-0.1639},{255,152,0}},
    
    {{-0.2940, 0.0093, -2.4173},{0.8766,0.4518,-0.1658},{255,152,0}},
    
    {{-0.3762, 0.0563, -2.4605},{0.0623,0.7869,-0.6139},{255,152,0}},
    
    {{-0.3294, 0.0570, -2.4123},{0.5821,0.8073,-0.0967},{255,152,0}},
    
    {{-0.3172, 0.0145, -2.4669},{0.6641,0.3794,-0.6442},{255,152,0}},
    
    {{-0.3311, 0.0565, -2.4576},{0.5041,0.7478,-0.4321},{255,152,0}},
    
    {{-0.2940, 0.0093, 2.6617},{0.8766,0.4518,0.1658},{255,152,0}},
    
    {{-0.3854, 0.0767, 2.6518},{0.1513,0.9748,0.1639},{255,152,0}},
    
    {{-0.3671, 0.0070, 2.7335},{0.1361,0.3250,0.9358},{255,152,0}},
    
    {{-0.3294, 0.0570, 2.6568},{0.5821,0.8073,0.0967},{255,152,0}},
    
    {{-0.3762, 0.0563, 2.7049},{0.0623,0.7869,0.6139},{255,152,0}},
    
    {{-0.3172, 0.0145, 2.7114},{0.6641,0.3794,0.6442},{255,152,0}},
    
    {{-0.3311, 0.0565, 2.7021},{0.5041,0.7478,0.4321},{255,152,0}},
    
    {{-0.9231, 0.1660, 2.7951},{-0.9762,0.1267,0.1760},{255,152,0}},
    
    {{-0.8734, 0.0985, 2.8066},{-0.1966,-0.9612,0.1934},{255,152,0}},
    
    {{-0.8486, 0.1682, 2.8661},{-0.1963,-0.0009,0.9805},{255,152,0}},
    
    {{-0.9172, 0.1183, 2.8009},{-0.8041,-0.5798,0.1311},{255,152,0}},
    
    {{-0.8610, 0.1189, 2.8533},{-0.1404,-0.5976,0.7894},{255,152,0}},
    
    {{-0.8992, 0.1734, 2.8444},{-0.6604,0.1965,0.7247},{255,152,0}},
    
    {{-0.9072, 0.1265, 2.8465},{-0.6129,-0.3697,0.6983},{255,152,0}},
    
    {{-0.9231, 0.1660, -2.5026},{-0.9762,0.1267,-0.1760},{255,152,0}},
    
    {{-0.8486, 0.1682, -2.5736},{-0.1963,-0.0009,-0.9805},{255,152,0}},
    
    {{-0.8734, 0.0985, -2.5141},{-0.1966,-0.9612,-0.1934},{255,152,0}},
    
    {{-0.8992, 0.1734, -2.5519},{-0.6604,0.1965,-0.7247},{255,152,0}},
    
    {{-0.8610, 0.1189, -2.5608},{-0.1404,-0.5976,-0.7894},{255,152,0}},
    
    {{-0.9172, 0.1183, -2.5083},{-0.8041,-0.5798,-0.1311},{255,152,0}},
    
    {{-0.9072, 0.1265, -2.5539},{-0.6129,-0.3697,-0.6983},{255,152,0}},
    
    {{0.8486, 0.1682, -2.5736},{0.1963,-0.0009,-0.9805},{255,152,0}},
    
    {{0.9231, 0.1660, -2.5026},{0.9762,0.1267,-0.1760},{255,152,0}},
    
    {{0.8734, 0.0985, -2.5141},{0.1966,-0.9612,-0.1934},{255,152,0}},
    
    {{0.8992, 0.1734, -2.5519},{0.6604,0.1965,-0.7247},{255,152,0}},
    
    {{0.9172, 0.1183, -2.5083},{0.8041,-0.5798,-0.1311},{255,152,0}},
    
    {{0.8610, 0.1189, -2.5608},{0.1404,-0.5976,-0.7894},{255,152,0}},
    
    {{0.9072, 0.1265, -2.5539},{0.6130,-0.3697,-0.6983},{255,152,0}},
    
    {{0.9231, 0.1660, 2.7951},{0.9762,0.1267,0.1760},{255,152,0}},
    
    {{0.8486, 0.1682, 2.8661},{0.1963,-0.0009,0.9805},{255,152,0}},
    
    {{0.8734, 0.0985, 2.8066},{0.1966,-0.9612,0.1934},{255,152,0}},
    
    {{0.8992, 0.1734, 2.8444},{0.6604,0.1965,0.7247},{255,152,0}},
    
    {{0.8610, 0.1189, 2.8533},{0.1404,-0.5976,0.7894},{255,152,0}},
    
    {{0.9172, 0.1183, 2.8009},{0.8041,-0.5798,0.1311},{255,152,0}},
    
    {{0.9072, 0.1265, 2.8465},{0.6129,-0.3697,0.6983},{255,152,0}},
    
    {{-0.6379, 0.8497, 2.7575},{-0.1361,0.3250,0.9358},{255,152,0}},
    
    {{-0.6196, 0.9194, 2.6758},{-0.1513,0.9748,0.1639},{255,152,0}},
    
    {{-0.7110, 0.8520, 2.6858},{-0.8766,0.4518,0.1658},{255,152,0}},
    
    {{-0.6288, 0.8990, 2.7290},{-0.0623,0.7869,0.6139},{255,152,0}},
    
    {{-0.6756, 0.8997, 2.6808},{-0.5821,0.8073,0.0967},{255,152,0}},
    
    {{-0.6878, 0.8572, 2.7354},{-0.6641,0.3794,0.6442},{255,152,0}},
    
    {{-0.6739, 0.8992, 2.7261},{-0.5041,0.7478,0.4321},{255,152,0}},
    
    {{-0.7110, 0.8520, -2.3932},{-0.8766,0.4518,-0.1658},{255,152,0}},
    
    {{-0.6196, 0.9194, -2.3833},{-0.1513,0.9748,-0.1639},{255,152,0}},
    
    {{-0.6379, 0.8497, -2.4650},{-0.1361,0.3250,-0.9358},{255,152,0}},
    
    {{-0.6756, 0.8997, -2.3883},{-0.5821,0.8073,-0.0967},{255,152,0}},
    
    {{-0.6288, 0.8990, -2.4365},{-0.0623,0.7869,-0.6139},{255,152,0}},
    
    {{-0.6878, 0.8572, -2.4429},{-0.6641,0.3795,-0.6442},{255,152,0}},
    
    {{-0.6739, 0.8992, -2.4336},{-0.5041,0.7478,-0.4321},{255,152,0}},
    
    {{0.6379, 0.8497, -2.4650},{0.1361,0.3250,-0.9358},{255,152,0}},
    
    {{0.6196, 0.9194, -2.3833},{0.1513,0.9748,-0.1639},{255,152,0}},
    
    {{0.7110, 0.8520, -2.3932},{0.8766,0.4518,-0.1658},{255,152,0}},
    
    {{0.6288, 0.8990, -2.4365},{0.0623,0.7869,-0.6139},{255,152,0}},
    
    {{0.6756, 0.8997, -2.3883},{0.5821,0.8073,-0.0967},{255,152,0}},
    
    {{0.6878, 0.8572, -2.4429},{0.6641,0.3795,-0.6442},{255,152,0}},
    
    {{0.6739, 0.8992, -2.4336},{0.5041,0.7478,-0.4321},{255,152,0}},
    
    {{0.7110, 0.8520, 2.6858},{0.8766,0.4518,0.1658},{255,152,0}},
    
    {{0.6196, 0.9194, 2.6758},{0.1513,0.9748,0.1639},{255,152,0}},
    
    {{0.6379, 0.8497, 2.7575},{0.1361,0.3250,0.9358},{255,152,0}},
    
    {{0.6756, 0.8997, 2.6808},{0.5821,0.8073,0.0967},{255,152,0}},
    
    {{0.6288, 0.8990, 2.7290},{0.0623,0.7869,0.6139},{255,152,0}},
    
    {{0.6878, 0.8572, 2.7354},{0.6641,0.3794,0.6442},{255,152,0}},
    
    {{0.6739, 0.8992, 2.7261},{0.5041,0.7478,0.4321},{255,152,0}},
    
    {{-0.9231, 0.1660, 2.7951},{-0.9762,0.1267,0.1760},{255,152,0}},
    
    {{-0.8734, 0.0985, 2.8066},{-0.1966,-0.9612,0.1934},{255,152,0}},
    
    {{-0.8486, 0.1682, 2.8661},{-0.1963,-0.0009,0.9805},{255,152,0}},
    
    {{-0.9172, 0.1183, 2.8009},{-0.8041,-0.5798,0.1311},{255,152,0}},
    
    {{-0.8610, 0.1189, 2.8533},{-0.1404,-0.5976,0.7894},{255,152,0}},
    
    {{-0.8992, 0.1734, 2.8444},{-0.6604,0.1965,0.7247},{255,152,0}},
    
    {{-0.9072, 0.1265, 2.8465},{-0.6129,-0.3697,0.6983},{255,152,0}},
    
    {{-0.9231, 0.1660, -2.5026},{-0.9762,0.1267,-0.1760},{255,152,0}},
    
    {{-0.8486, 0.1682, -2.5736},{-0.1963,-0.0009,-0.9805},{255,152,0}},
    
    {{-0.8734, 0.0985, -2.5141},{-0.1966,-0.9612,-0.1934},{255,152,0}},
    
    {{-0.8992, 0.1734, -2.5519},{-0.6604,0.1965,-0.7247},{255,152,0}},
    
    {{-0.8610, 0.1189, -2.5608},{-0.1404,-0.5976,-0.7894},{255,152,0}},
    
    {{-0.9172, 0.1183, -2.5083},{-0.8041,-0.5798,-0.1311},{255,152,0}},
    
    {{-0.9072, 0.1265, -2.5539},{-0.6129,-0.3697,-0.6983},{255,152,0}},
    
    {{0.8486, 0.1682, -2.5736},{0.1963,-0.0009,-0.9805},{255,152,0}},
    
    {{0.9231, 0.1660, -2.5026},{0.9762,0.1267,-0.1760},{255,152,0}},
    
    {{0.8734, 0.0985, -2.5141},{0.1966,-0.9612,-0.1934},{255,152,0}},
    
    {{0.8992, 0.1734, -2.5519},{0.6604,0.1965,-0.7247},{255,152,0}},
    
    {{0.9172, 0.1183, -2.5083},{0.8041,-0.5798,-0.1311},{255,152,0}},
    
    {{0.8610, 0.1189, -2.5608},{0.1404,-0.5976,-0.7894},{255,152,0}},
    
    {{0.9072, 0.1265, -2.5539},{0.6130,-0.3697,-0.6983},{255,152,0}},
    
    {{0.9231, 0.1660, 2.7951},{0.9762,0.1267,0.1760},{255,152,0}},
    
    {{0.8486, 0.1682, 2.8661},{0.1963,-0.0009,0.9805},{255,152,0}},
    
    {{0.8734, 0.0985, 2.8066},{0.1966,-0.9612,0.1934},{255,152,0}},
    
    {{0.8992, 0.1734, 2.8444},{0.6604,0.1965,0.7247},{255,152,0}},
    
    {{0.8610, 0.1189, 2.8533},{0.1404,-0.5976,0.7894},{255,152,0}},
    
    {{0.9172, 0.1183, 2.8009},{0.8041,-0.5798,0.1311},{255,152,0}},
    
    {{0.9072, 0.1265, 2.8465},{0.6129,-0.3697,0.6983},{255,152,0}},
    
    {{-0.6379, 0.8497, 2.7575},{-0.1361,0.3250,0.9358},{255,152,0}},
    
    {{-0.6196, 0.9194, 2.6758},{-0.1513,0.9748,0.1639},{255,152,0}},
    
    {{-0.7110, 0.8520, 2.6858},{-0.8766,0.4518,0.1658},{255,152,0}},
    
    {{-0.6288, 0.8990, 2.7290},{-0.0623,0.7869,0.6139},{255,152,0}},
    
    {{-0.6756, 0.8997, 2.6808},{-0.5821,0.8073,0.0967},{255,152,0}},
    
    {{-0.6878, 0.8572, 2.7354},{-0.6641,0.3794,0.6442},{255,152,0}},
    
    {{-0.6739, 0.8992, 2.7261},{-0.5041,0.7478,0.4321},{255,152,0}},
    
    {{-0.7110, 0.8520, -2.3932},{-0.8766,0.4518,-0.1658},{255,152,0}},
    
    {{-0.6196, 0.9194, -2.3833},{-0.1513,0.9748,-0.1639},{255,152,0}},
    
    {{-0.6379, 0.8497, -2.4650},{-0.1361,0.3250,-0.9358},{255,152,0}},
    
    {{-0.6756, 0.8997, -2.3883},{-0.5821,0.8073,-0.0967},{255,152,0}},
    
    {{-0.6288, 0.8990, -2.4365},{-0.0623,0.7869,-0.6139},{255,152,0}},
    
    {{-0.6878, 0.8572, -2.4429},{-0.6641,0.3795,-0.6442},{255,152,0}},
    
    {{-0.6739, 0.8992, -2.4336},{-0.5041,0.7478,-0.4321},{255,152,0}},
    
    {{0.6379, 0.8497, -2.4650},{0.1361,0.3250,-0.9358},{255,152,0}},
    
    {{0.6196, 0.9194, -2.3833},{0.1513,0.9748,-0.1639},{255,152,0}},
    
    {{0.7110, 0.8520, -2.3932},{0.8766,0.4518,-0.1658},{255,152,0}},
    
    {{0.6288, 0.8990, -2.4365},{0.0623,0.7869,-0.6139},{255,152,0}},
    
    {{0.6756, 0.8997, -2.3883},{0.5821,0.8073,-0.0967},{255,152,0}},
    
    {{0.6878, 0.8572, -2.4429},{0.6641,0.3795,-0.6442},{255,152,0}},
    
    {{0.6739, 0.8992, -2.4336},{0.5041,0.7478,-0.4321},{255,152,0}},
    
    {{0.7110, 0.8520, 2.6858},{0.8766,0.4518,0.1658},{255,152,0}},
    
    {{0.6196, 0.9194, 2.6758},{0.1513,0.9748,0.1639},{255,152,0}},
    
    {{0.6379, 0.8497, 2.7575},{0.1361,0.3250,0.9358},{255,152,0}},
    
    {{0.6756, 0.8997, 2.6808},{0.5821,0.8073,0.0967},{255,152,0}},
    
    {{0.6288, 0.8990, 2.7290},{0.0623,0.7869,0.6139},{255,152,0}},
    
    {{0.6878, 0.8572, 2.7354},{0.6641,0.3794,0.6442},{255,152,0}},
    
    {{0.6739, 0.8992, 2.7261},{0.5041,0.7478,0.4321},{255,152,0}}
    
};



const GLuint Indices[] = {
    51,28,2,
    44,49,15,
    50,43,36,
    37,42,14,
    30,35,0,
    0,3,6,
    1,4,6,
    2,5,4,7,10,13,8,11,10,9,12,13,14,17,19,15,18,20,16,19,20,21,24,27,22,25,24,23,26,27,28,31,34,29,32,31,30,33,32,35,38,40,36,39,38,37,40,41,42,45,48,43,46,45,44,47,46,49,52,54,50,53,52,51,54,55,29,36,38,32,38,35,37,8,40,40,10,35,9,1,3,12,3,0,2,28,33,5,33,30,36,43,39,39,45,42,44,15,17,47,17,14,16,9,11,19,11,8,43,50,52,46,52,44,51,22,54,54,24,49,23,16,18,26,18,15,50,29,31,53,31,28,1,23,25,4,25,2,1,9,16,107,84,58,100,105,71,106,99,92,93,98,70,86,91,56,56,59,62,57,60,62,58,61,60,63,66,69,64,67,66,65,68,69,70,73,75,71,74,76,72,75,76,77,80,83,78,81,80,79,82,83,84,87,90,85,88,87,86,89,88,91,94,96,92,95,94,93,96,97,98,101,104,99,102,101,100,103,102,105,108,110,106,109,108,107,110,111,85,92,94,88,94,91,93,64,96,96,66,91,65,57,59,68,59,56,58,84,89,61,89,86,92,99,95,95,101,98,100,71,73,103,73,70,72,65,67,75,67,64,99,106,108,102,108,100,107,78,110,110,80,105,79,72,74,82,74,71,106,85,87,109,87,84,57,79,81,60,81,58,57,65,72,163,140,114,156,161,127,162,155,148,149,154,126,142,147,119,112,115,118,113,116,118,114,117,116,119,122,125,120,123,122,121,124,125,126,129,131,127,130,132,128,131,132,133,136,139,134,137,136,135,138,139,140,143,146,141,144,143,142,145,144,147,150,152,148,151,150,149,152,153,154,157,160,155,158,157,156,159,158,161,164,166,162,165,164,163,166,167,141,148,150,144,150,142,149,120,152,152,122,147,121,113,115,124,115,112,114,140,145,117,145,142,148,155,157,151,157,154,156,127,129,159,129,126,128,121,123,131,123,120,155,162,164,158,164,156,163,134,166,166,136,161,135,128,130,138,130,127,162,141,143,165,143,140,113,135,116,116,137,134,113,121,128,219,196,170,212,217,189,218,211,204,205,210,182,198,203,175,168,171,174,169,172,174,170,173,172,175,178,181,176,179,178,177,180,181,182,185,187,183,186,188,184,187,188,189,192,195,190,193,192,191,194,195,196,199,202,197,200,199,198,201,200,203,206,208,204,207,206,205,208,209,210,213,216,211,214,213,212,215,214,217,220,222,218,221,220,219,222,223,197,204,206,200,206,198,205,176,208,208,178,203,177,169,171,180,171,168,170,196,201,173,201,198,204,211,207,207,213,210,212,183,185,215,185,182,184,177,187,187,179,176,211,218,220,214,220,212,219,190,222,222,192,217,191,184,186,194,186,183,218,197,199,221,199,196,169,191,193,172,193,170,169,177,184,275,252,226,268,273,245,274,267,260,261,266,238,254,259,231,224,227,230,225,228,230,226,229,228,231,234,237,232,235,234,233,236,237,238,241,243,239,242,244,240,243,244,245,248,251,246,249,248,247,250,251,252,255,258,253,256,255,254,257,256,259,262,264,260,263,262,261,264,265,266,269,272,267,270,269,268,271,270,273,276,278,274,277,276,275,278,279,253,260,262,256,262,254,261,232,264,264,234,259,233,225,227,236,227,224,226,252,257,229,257,254,260,267,263,263,269,266,268,239,241,271,241,238,240,233,243,243,235,232,267,274,276,270,276,268,275,246,278,278,248,273,247,240,242,250,242,239,274,253,255,277,255,252,225,247,249,228,249,226,225,233,240,22,51,2,35,7,0,29,50,36,8,37,14,43,45,39,5,0,6,3,1,6,5,6,4,12,7,13,11,13,10,11,9,13,17,20,19,17,15,20,18,16,20,26,21,27,25,27,24,25,23,27,33,28,34,32,34,31,33,34,32,38,41,40,39,41,38,39,37,41,47,42,48,46,48,45,47,48,46,52,55,54,53,55,52,53,51,55,32,29,38,30,32,35,8,10,40,10,7,35,12,9,3,7,12,0,5,2,33,0,5,30,52,49,44,37,39,42,47,44,17,42,47,14,19,16,11,14,19,8,46,43,52,25,22,2,22,24,54,24,21,49,26,23,18,21,26,15,53,50,31,51,53,28,4,1,25,105,77,71,23,1,16,78,107,58,91,63,56,85,106,92,64,93,70,99,101,95,61,56,62,59,57,62,61,62,60,68,63,69,67,69,66,67,65,69,73,76,75,73,71,76,74,72,76,82,77,83,81,83,80,81,79,83,89,84,90,88,90,87,89,90,88,94,97,96,95,97,94,95,93,97,103,98,104,102,104,101,103,104,102,108,111,110,109,111,108,109,107,111,88,85,94,86,88,91,64,66,96,66,63,91,68,65,59,63,68,56,61,58,89,56,61,86,108,105,100,93,95,98,103,100,73,98,103,70,75,72,67,70,75,64,102,99,108,81,78,58,78,80,110,80,77,105,82,79,74,77,82,71,109,106,87,107,109,84,60,57,81,161,133,127,79,57,72,134,163,114,150,147,142,141,162,148,120,149,126,112,142,119,117,112,118,115,113,118,117,118,116,124,119,125,123,125,122,123,121,125,129,132,131,129,127,132,130,128,132,138,133,139,137,139,136,137,135,139,145,140,146,144,146,143,145,146,144,150,153,152,151,153,150,151,149,153,159,154,160,158,160,157,159,160,158,164,167,166,165,167,164,165,163,167,144,141,150,164,161,156,120,122,152,122,119,147,124,121,115,119,124,112,117,114,145,112,117,142,151,148,157,149,151,154,159,156,129,154,159,126,131,128,123,126,131,120,158,155,164,135,137,116,134,136,166,136,133,161,138,135,130,133,138,127,165,162,143,163,165,140,206,203,198,114,116,134,135,113,128,190,219,170,183,212,189,197,218,204,176,205,182,168,198,175,173,168,174,171,169,174,173,174,172,180,175,181,179,181,178,179,177,181,185,188,187,185,183,188,186,184,188,194,189,195,193,195,192,193,191,195,201,196,202,200,202,199,201,202,200,206,209,208,207,209,206,207,205,209,215,210,216,214,216,213,215,216,214,220,223,222,221,223,220,221,219,223,200,197,206,211,213,207,176,178,208,178,175,203,180,177,171,175,180,168,173,170,201,168,173,198,177,179,187,205,207,210,215,212,185,210,215,182,220,217,212,182,187,176,214,211,220,193,190,170,190,192,222,192,189,217,194,191,186,189,194,183,221,218,199,219,221,196,172,169,193,262,259,254,191,169,184,246,275,226,239,268,245,253,274,260,232,261,238,224,254,231,229,224,230,227,225,230,229,230,228,236,231,237,235,237,234,235,233,237,241,244,243,241,239,244,242,240,244,250,245,251,249,251,248,249,247,251,257,252,258,256,258,255,257,258,256,262,265,264,263,265,262,263,261,265,271,266,272,270,272,269,271,272,270,276,279,278,277,279,276,277,275,279,256,253,262,267,269,263,232,234,264,234,231,259,236,233,227,231,236,224,229,226,257,224,229,254,233,235,243,261,263,266,271,268,241,266,271,238,276,273,268,238,243,232,270,267,276,249,246,226,246,248,278,248,245,273,250,247,242,245,250,239,277,274,255,275,277,252,228,225,249,247,225,240,49,21,15
    
};

void buildPerspProjMat(float* m, float fov, float aspect, float znear, float zfar)
{
    float xymax = znear * tan(fov* PI_OVER_360);
    float depth = zfar - znear;
    float q = -(zfar + znear) / depth;
    float qn = -2 * (zfar * znear) / depth;
    float w = znear / (xymax * aspect);
    float h = znear / xymax;
    m[0]  = w; m[1]  = 0; m[2]  = 0;  m[3]  =  0;
    m[4]  = 0; m[5]  = h; m[6]  = 0;  m[7]  =  0;
    m[8]  = 0; m[9]  = 0; m[10] = q;  m[11] = -1;
    m[12] = 0; m[13] = 0; m[14] = qn; m[15] =  0;
}
void setTranslationMatrix(float* m, float tx, float ty, float tz)
{
    m[0]  = 1; m[4]  = 0; m[8]  = 0;  m[12] = tx;
    m[1]  = 0; m[5]  = 1; m[9]  = 0;  m[13] = ty;
    m[2]  = 0; m[6]  = 0; m[10] = 1;  m[14] = tz;
    m[3]  = 0; m[7]  = 0; m[11] = 0;  m[15] =  1;
}
void setRotationMatrx(float* m, const float* q)
{
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
void matMult4x4by4x4(float* m, const float* a, const float* b)
{
    m[0]  = a[0]*b[0]  + a[4]*b[1]  + a[8]* b[2]  + a[12]*b[3];
    m[1]  = a[1]*b[0]  + a[5]*b[1]  + a[9]* b[2]  + a[13]*b[3];
    m[2]  = a[2]*b[0]  + a[6]*b[1]  + a[10]*b[2]  + a[14]*b[3];
    m[3]  = a[3]*b[0]  + a[7]*b[1]  + a[11]*b[2]  + a[15]*b[3];
    m[4]  = a[0]*b[4]  + a[4]*b[5]  + a[8]* b[6]  + a[12]*b[7];
    m[5]  = a[1]*b[4]  + a[5]*b[5]  + a[9]* b[6]  + a[13]*b[7];
    m[6]  = a[2]*b[4]  + a[6]*b[5]  + a[10]*b[6]  + a[14]*b[7];
    m[7]  = a[3]*b[4]  + a[7]*b[5]  + a[11]*b[6]  + a[15]*b[7];
    m[8]  = a[0]*b[8]  + a[4]*b[9]  + a[8]* b[10] + a[12]*b[11];
    m[9]  = a[1]*b[8]  + a[5]*b[9]  + a[9]* b[10] + a[13]*b[11];
    m[10] = a[2]*b[8]  + a[6]*b[9]  + a[10]*b[10] + a[14]*b[11];
    m[11] = a[3]*b[8]  + a[7]*b[9]  + a[11]*b[10] + a[15]*b[11];
    m[12] = a[0]*b[12] + a[4]*b[13] + a[8]* b[14] + a[12]*b[15];
    m[13] = a[1]*b[12] + a[5]*b[13] + a[9]* b[14] + a[13]*b[15];
    m[14] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14]*b[15];
    m[15] = a[3]*b[12] + a[7]*b[13] + a[11]*b[14] + a[15]*b[15];
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
        exit(1);
    }
    return shaderHandle;
}
void compileShaders()
{
    GLchar vertexShaderString[] =
    "attribute vec4 a_position; \n"
    "attribute vec3 a_normal; \n"
    "attribute vec3 a_diffuseMaterial; \n"
    "varying vec3 v_diffuseMaterial; \n"
    //"varying vec3 v_rotatedNormal; \n"
    "uniform mat4 u_mvpMatrix; \n"
    "uniform mat4 u_rotMatrix; \n"
    "uniform vec3 u_lightDirection; \n"
    "void main(void) \n"
    "{ \n"
    //"    v_rotatedNormal = vec3(u_rotMatrix * vec4(a_normal, 0.0)); \n"
    
    "    vec3 rotatedNormal = vec3(u_rotMatrix * vec4(a_normal, 0.0)); \n"
    "    highp vec3 N = normalize(rotatedNormal); \n"
    "    highp vec3 L = normalize(u_lightDirection); \n"
    "    float df = max(0.0, dot(N, -L));"
    
    "    v_diffuseMaterial = df * a_diffuseMaterial; \n"
    "    gl_Position = u_mvpMatrix * a_position; \n"
    "} \0";
    
    GLchar fragmentShaderString[] =
    "varying lowp vec3 v_diffuseMaterial; \n"
    "void main(void) \n"
    "{ \n"
    "    gl_FragColor = vec4(v_diffuseMaterial, 1); \n"
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
        exit(1);
    }
    glUseProgram(programHandle);
    _positionSlot = glGetAttribLocation(programHandle, "a_position");
    _normalSlot = glGetAttribLocation(programHandle, "a_normal");
    _colorSlot = glGetAttribLocation(programHandle, "a_diffuseMaterial");
    uniform_mvpMatrix = glGetUniformLocation(programHandle, "u_mvpMatrix");
    uniform_rotMatrix = glGetUniformLocation(programHandle, "u_rotMatrix");
    
    uniform_lightDirection = glGetUniformLocation(programHandle, "u_lightDirection");
    
//    uniform_ambientMaterial = glGetUniformLocation(programHandle, "u_AmbientMaterial");
//    uniform_lightPosition = glGetUniformLocation(programHandle, "u_LightPosition");
//    uniform_specularMaterial = glGetUniformLocation(programHandle, "u_SpecularMaterial");
//    uniform_shininess = glGetUniformLocation(programHandle, "u_Shininess");
    
    glEnableVertexAttribArray(_positionSlot);
    glEnableVertexAttribArray(_normalSlot);
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
    
    float fov=30.0f; // in degrees
    float aspect=((float)screenWidthInPixels)/screenHeightInPixels;
    float znear=10.0f;
    float zfar=100.0f;
    buildPerspProjMat(perspectiveMatrix, fov, aspect, znear, zfar);
    glViewport(0, 0, screenWidthInPixels, screenHeightInPixels);
    
    glClearColor(0, 50.0/255.0, 55.0/255.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    
    //Normalize rot axis
    float axisNorm = sqrtf(rotAxis[0]*rotAxis[0] + rotAxis[1]*rotAxis[1] + rotAxis[2]*rotAxis[2]);
    rotAxis[0] =  rotAxis[0] / axisNorm;
    rotAxis[1] =  rotAxis[1] / axisNorm;
    rotAxis[2] =  rotAxis[2] / axisNorm;
    
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
}

void renderScene(int timeDiffMillies)
{
    angle += 0.001 * timeDiffMillies; if(angle > 2*pi) angle = 0;
    
    //Build a quaternion for the current angle.
    float sinHalfAngle = sinf(angle/2);
    quat[3] = cosf(angle/2);
    quat[0] = rotAxis[0] * sinHalfAngle;
    quat[1] = rotAxis[1] * sinHalfAngle;
    quat[2] = rotAxis[2] * sinHalfAngle;
    
    setRotationMatrx(rotationMatrix, quat);
    setTranslationMatrix(translationMatrix, cos(angle), sin(angle), -17 + 5*sin(angle));
    
    //Combine transformation matrices so the shader does not recalculate the combined transformation for each vertex.
    matMult4x4by4x4(transRotMatrix, translationMatrix, rotationMatrix);
    matMult4x4by4x4(modelMatrix, perspectiveMatrix, transRotMatrix);
    
    //Upload transformation matrix to shader.
    glUniformMatrix4fv(uniform_rotMatrix, 1, 0, rotationMatrix);
    glUniformMatrix4fv(uniform_mvpMatrix, 1, 0, modelMatrix);
    
    //Lighting uniforms
    glUniform3f(uniform_lightDirection, -1, -2, -.1);
    //glUniform3f(uniform_lightPosition, 0, 10, -1);
    //glUniform3f(uniform_ambientMaterial, 1, 0, 0);
    //glUniform3fv(uniform_specularMaterial, 1, specularMaterial);
    //glUniform1f(uniform_shininess, shininess);
    
    glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(_normalSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
    glVertexAttribPointer(_colorSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 6));
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_INT, 0);
}



