#pragma once
#ifdef __APPLE__
#include <OpenGL/GLU.h>
#else
#include <GL/glu.h>
#endif

char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);
GLuint create_program(const char* vertexfile, const char *fragmentfile);
void delete_program(GLuint prog);
GLint get_attrib(GLuint program, const char *name);
GLint get_uniform(GLuint program, const char *name);
