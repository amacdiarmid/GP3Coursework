#ifndef _COMMON_H
#define _COMMON_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <random>

using namespace std;

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>

//maths headers
#include <glm/glm.hpp>

using namespace glm;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float SCREEN_WIDTH = 640;
const float SCREEN_HEIGHT = 480;
const float FOV = 45;
const float NEAR_CLIPPINGPLANE = 0.1f;
const float FAR_CLIPPINGPLANE = 500.0f;

const string ASSET_PATH = "assets";
const string SHADER_PATH = "/shaders";
const string TEXTURE_PATH = "/textures";
const string FONT_PATH = "/fonts";
const string MODEL_PATH = "/models";
const string SCENE_PATH = "/scenes";

void checkError(const char *file, int line);

extern bool GameRunning;

#ifdef _DEBUG
#define CHECK_GL_ERROR() checkError(__FILE__, __LINE__)
#else
#define CHECK_GL_ERROR()
#endif

#endif
