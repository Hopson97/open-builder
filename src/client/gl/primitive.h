#pragma once

#include "vertex_array.h"

gl::VertexArray makeCubeVertexArray(GLfloat width, GLfloat height, GLfloat depth);
gl::VertexArray makeWireCubeVertexArray(GLfloat width, GLfloat height, GLfloat depth);
gl::VertexArray makeQuadVertexArray(GLfloat relativeWidth, GLfloat relativeHeight);
gl::VertexArray makeScreenQuadVertexArray();
