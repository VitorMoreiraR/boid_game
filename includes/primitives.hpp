#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP
#include "colors.hpp"
#include "vector.hpp"
#include <GL/gl.h>
#include <cmath>
#include <string>
#include <vector>

class Primitives {
public:
  vec3 cross(const vec3 &a, const vec3 &b);
  vec3 normalize(const vec3 &v);
  void rect(vec3 p1, vec3 p2, vec3 p3, vec3 p4, color cor);
  void desenhaCubo(float SIZE, Colors colors);
  void desenhaEsfera(GLfloat raio, GLuint nStacks, GLuint nSectors,
                     Colors colors);
  void desenhaCone(GLfloat altura, GLfloat raioBase, GLuint nStacks,
                   GLuint nSectors, Colors colors);
  void desenharPiramide(float baseComprimento, float baseLargura, float altura,
                        Colors colors);
  void desenhaParalelepipedo(float largura, float altura, float profundidade,
                             Colors colors);
  void desenhaTerreno(unsigned int id, Colors colors);
};

#endif