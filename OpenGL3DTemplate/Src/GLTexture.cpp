#include "GLTexture.h"
#include <stdio.h>

GLTexture::GLTexture() {
    texture[0] = 0;
}

GLTexture::~GLTexture() {
}

void GLTexture::Use() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
}

void GLTexture::BuildColorTexture(unsigned char r, unsigned char g, unsigned char b) {
    unsigned char data[12];
    for (int i = 0; i < 12; i += 3) {
        data[i] = r; data[i + 1] = g; data[i + 2] = b;
    }
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void GLTexture::Load(char* name) {
    // 1. Use Windows API to load the BMP (No Glaux needed!)
    // We use LoadImageA to explicitly force ANSI mode, fixing your character set error
    HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    if (hBitmap == NULL) {
        printf("ERROR: Could not load texture: %s\n", name);
        return;
    }

    // 2. Get Bitmap Info
    BITMAP bmp;
    GetObject(hBitmap, sizeof(bmp), &bmp);
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    // 3. Generate Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Using GL_BGR_EXT usually fixes the "Blue Face" issue on Windows
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

    DeleteObject(hBitmap);
}