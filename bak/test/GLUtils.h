#ifndef GLUTILS_H
#define GLUTILS_H

#include <string>

#include <FDGL/OpenGLTexture.h>
#include <FDGL/OpenGLShaderProgram.h>

#include <assimp/texture.h>

std::string loadFile(const std::string &filePath);

FDGL::OpenGLTextureObjectWrapper loadTexture(const std::string &filePath);

FDGL::OpenGLTextureObjectWrapper loadTexture(const aiTexture *input);

FDGL::OpenGLShaderProgramWrapper createShaderProgram();

FDGL::OpenGLShaderProgramWrapper createLightShaderProgram();


#endif // GLUTILS_H
