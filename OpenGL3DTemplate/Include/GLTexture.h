#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <windows.h>
#include "glut.h"

class GLTexture
{
public:
    char* texturename;
    unsigned int texture[1];
    int width;
    int height;

    GLTexture();
    virtual ~GLTexture();

    void Load(char* name); // Main function to load BMP
    void Use();            // Bind texture
    void BuildColorTexture(unsigned char r, unsigned char g, unsigned char b);
};

#endif