#pragma once
#include "colors.hpp"
#include "vector.hpp"
#include <GL/gl.h>
#include <cmath>
#include <string>
#include <vector>

vec3 cross(const vec3 &a, const vec3 &b) {

  return vec3(

      a.y * b.z - a.z * b.y,

      a.z * b.x - a.x * b.z,

      a.x * b.y - a.y * b.x

  );
}

vec3 normalize(const vec3 &v) {

  float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

  if (len == 0)
    return v;

  return vec3(v.x / len, v.y / len, v.z / len);
}

void rect(vec3 p1, vec3 p2, vec3 p3, vec3 p4, color cor) {

  vec3 u = p2 - p1;
  vec3 v = p3 - p1;
  vec3 normal = normalize(cross(u, v));

  glColor3fv(cor);
  glBegin(GL_QUADS);
  glNormal3f(normal.x, normal.y, normal.z);
  glVertex3fv(&p1.x);
  glVertex3fv(&p2.x);
  glVertex3fv(&p3.x);
  glVertex3fv(&p4.x);
  glEnd();
}
void desenhaCubo(float SIZE) {
  float d = SIZE / 2.0;

  glPolygonMode(GL_FRONT, GL_FILL);

  vec3 v1(-d, d, d);  // superior esquerdo
  vec3 v2(-d, -d, d); // inferior esquerdo
  vec3 v3(d, -d, d);  // inferior direito
  vec3 v4(d, d, d);   // superior direito

  vec3 v5(d, d, -d);   // superior esq
  vec3 v6(d, -d, -d);  // inferior esq
  vec3 v7(-d, -d, -d); // inferior dir
  vec3 v8(-d, d, -d);  // sup dir

  float matSpecular[] = {1.f, 1.f, 1.f, 1.f};
  glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
  glMaterialf(GL_FRONT, GL_SHININESS, 128);

  // frente
  glNormal3f(0.f, 0.f, 1.f);
  rect(v1, v2, v3, v4, vermelho);

  // direita
  glNormal3f(1.f, 0.f, 0.f);
  rect(v4, v3, v6, v5, azul);

  // back
  glNormal3f(0.f, 0.f, -1.f);
  rect(v5, v6, v7, v8, amarelo);

  // esquerda
  glNormal3f(-1.f, 0.f, 0.f);
  rect(v1, v8, v7, v2, verde);

  // topo
  glNormal3f(0.f, 1.f, 0.f);
  rect(v1, v4, v5, v8, violeta);

  // bottom
  glNormal3f(0.f, -1.f, 0.f);
  rect(v2, v7, v6, v3, laranja);
}

void desenhaEsfera(GLfloat raio, GLuint nStacks, GLuint nSectors) {
  std::vector<vec3> pontos;
  const GLfloat PI = 3.14159265359;

  GLfloat deltaPhi = PI / nStacks;
  GLfloat deltaTheta = 2 * PI / nSectors;

  for (GLuint i = 0; i <= nStacks; i++) {
    GLfloat phi = -PI / 2.0 + i * deltaPhi;
    GLfloat temp = raio * cos(phi);
    GLfloat y = raio * sin(phi);

    for (GLuint j = 0; j < nSectors; j++) {
      GLfloat theta = j * deltaTheta;
      GLfloat x = temp * sin(theta);
      GLfloat z = temp * cos(theta);

      pontos.push_back(vec3(x, y, z));
    } // next j

  } // next i

  // --- plota pontos
  glColor3fv(vermelho);
  glPointSize(2.5);
  glBegin(GL_POINTS);
  for (GLuint i = 0; i < pontos.size(); i++) {
    glVertex3fv(&pontos[i].x);
  }
  glEnd();
}

void desenhaCone(GLfloat altura, GLfloat raioBase, GLuint nStacks,
                 GLuint nSectors) {
  std::vector<std::vector<GLuint>> indices;
  std::vector<vec3> pontos;
  const GLfloat PI = 3.14159265359;

  GLfloat deltaH = altura / nStacks;
  GLfloat deltaTheta = 2 * PI / nSectors;

  GLfloat h = raioBase;
  GLfloat r = altura / 2.0f;

  vec3 normal_tilt = normalize(vec3(r, h, 0));
  GLfloat normal_y = normal_tilt.y;
  GLfloat normal_horiz = normal_tilt.x;

  for (GLuint i = 0; i <= nStacks; i++) {
    GLfloat y = -altura / 2.0 + i * deltaH;
    GLfloat current_r = raioBase * (1.0 - (GLfloat)i / nStacks);

    std::vector<GLuint> pt;
    for (GLuint j = 0; j < nSectors; j++) {
      GLfloat theta = j * deltaTheta;
      GLfloat x = current_r * cos(theta);
      GLfloat z = current_r * sin(theta);

      pontos.push_back(vec3(x, y, z));
      GLuint index = pontos.size() - 1;
      pt.push_back(index);
    }
    indices.push_back(pt);
  }

  for (GLuint i = 0; i < nStacks; i++) {
    if (i % 3 == 0)
      glColor3fv(vermelho);
    else if (i % 3 == 1)
      glColor3fv(cinza);
    else
      glColor3fv(azul);

    glBegin(GL_TRIANGLE_STRIP);
    for (GLuint j = 0; j <= nSectors; j++) {

      GLuint index_i = indices[i][j % nSectors];
      GLuint index_i1 = indices[i + 1][j % nSectors];

      GLfloat x_norm = pontos[index_i].x;
      GLfloat z_norm = pontos[index_i].z;
      GLfloat len = std::sqrt(x_norm * x_norm + z_norm * z_norm);

      if (len > 0.001) {

        GLfloat nx = (x_norm / len) * normal_horiz;
        GLfloat nz = (z_norm / len) * normal_horiz;
        glNormal3f(nx, normal_y, nz);
      } else {

        glNormal3f(0.0f, 1.0f, 0.0f);
      }

      glVertex3fv(&pontos[index_i].x);

      x_norm = pontos[index_i1].x;
      z_norm = pontos[index_i1].z;
      len = std::sqrt(x_norm * x_norm + z_norm * z_norm);

      if (len > 0.001) {
        GLfloat nx = (x_norm / len) * normal_horiz;
        GLfloat nz = (z_norm / len) * normal_horiz;
        glNormal3f(nx, normal_y, nz);
      } else {
        glNormal3f(0.0f, 1.0f, 0.0f);
      }

      glVertex3fv(&pontos[index_i1].x);
    }
    glEnd();
  }

  glColor3fv(laranja);
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0.0f, -1.0f, 0.0f);
  glVertex3f(0.0f, -altura / 2.0f, 0.0f);
  for (GLuint j = 0; j <= nSectors; j++) {
    GLfloat theta = j * deltaTheta;
    GLfloat x_base = raioBase * cos(theta);
    GLfloat z_base = raioBase * sin(theta);

    glVertex3f(x_base, -altura / 2.0f, z_base);
  }
  glEnd();
}
void desenharPiramide(float baseComprimento, float baseLargura, float altura) {
  glPushMatrix();

  float x = baseComprimento / 2.0f;
  float z = baseLargura / 2.0f;

  vec3 topo(0.0f, altura, 0.0f);

  // Vértices da base (no plano Y=0)
  vec3 v_fe(-x, 0.0f, z);  // frente esquerda
  vec3 v_fd(x, 0.0f, z);   // frente direita
  vec3 v_td(x, 0.0f, -z);  // trás direita
  vec3 v_te(-x, 0.0f, -z); // trás esquerda

  glBegin(GL_TRIANGLES);

  // Face frontal (Vertices: topo, v_fe, v_fd)
  vec3 normal_frente = normalize(cross(v_fe - topo, v_fd - topo));
  glNormal3f(normal_frente.x, normal_frente.y, normal_frente.z);
  glColor3fv(azul);
  glVertex3fv(&topo.x);
  glVertex3fv(&v_fe.x);
  glVertex3fv(&v_fd.x);

  // Face direita (Vertices: topo, v_fd, v_td)
  vec3 normal_direita = normalize(cross(v_fd - topo, v_td - topo));
  glNormal3f(normal_direita.x, normal_direita.y, normal_direita.z);
  glColor3fv(verde);
  glVertex3fv(&topo.x);
  glVertex3fv(&v_fd.x);
  glVertex3fv(&v_td.x);

  // Face traseira (Vertices: topo, v_td, v_te)
  vec3 normal_tras = normalize(cross(v_td - topo, v_te - topo));
  glNormal3f(normal_tras.x, normal_tras.y, normal_tras.z);
  glColor3fv(azul);
  glVertex3fv(&topo.x);
  glVertex3fv(&v_td.x);
  glVertex3fv(&v_te.x);

  // Face esquerda (Vertices: topo, v_te, v_fe)
  vec3 normal_esquerda = normalize(cross(v_te - topo, v_fe - topo));
  glNormal3f(normal_esquerda.x, normal_esquerda.y, normal_esquerda.z);
  glColor3fv(amarelo);
  glVertex3fv(&topo.x);
  glVertex3fv(&v_te.x);
  glVertex3fv(&v_fe.x);

  glEnd();

  glBegin(GL_QUADS);
  glNormal3f(0.0f, -1.0f, 0.0f); // Normal da base
  glColor3fv(amarelo);
  glVertex3fv(&v_fe.x); // frente esquerda
  glVertex3fv(&v_fd.x); // frente direita
  glVertex3fv(&v_td.x); // trás direita
  glVertex3fv(&v_te.x); // trás esquerda
  glEnd();

  glPopMatrix();
}
void desenhaParalelepipedo(float largura, float altura, float profundidade) {
  float dx = largura / 2.0f;
  float dy = altura / 2.0f;
  float dz = profundidade / 2.0f;

  // Define os 8 vértices
  vec3 v1(-dx, dy, dz);
  vec3 v2(-dx, -dy, dz);
  vec3 v3(dx, -dy, dz);
  vec3 v4(dx, dy, dz);

  vec3 v5(dx, dy, -dz);
  vec3 v6(dx, -dy, -dz);
  vec3 v7(-dx, -dy, -dz);
  vec3 v8(-dx, dy, -dz);

  // Frente
  rect(v1, v2, v3, v4, vermelho);

  // Direita
  rect(v4, v3, v6, v5, azul);

  // Traseira
  rect(v5, v8, v7, v6, amarelo);

  // Esquerda
  rect(v1, v8, v7, v2, verde);

  // Topo
  rect(v1, v4, v5, v8, laranja);

  // Base
  rect(v2, v7, v6, v3, vermelho);
}
void desenhaTerreno(unsigned int id) {
  float L = 500.0;
  float y = -0.5f;

  glNormal3f(0.0f, 1.0f, 0.0f);

  glColor3fv(verde);

  glBegin(GL_QUADS);

  glVertex3f(-L, y, -L);

  glVertex3f(L, y, -L);

  glVertex3f(L, y, L);

  glVertex3f(-L, y, L);
  glEnd();
}
void desenhaPassarro(float currentAnguloAsas) {
  // asa direita
  glPushMatrix();
  glRotatef(currentAnguloAsas / 200 * 100, 1.0, 0.0, 0);
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glRotatef(90, 0.0, 1.0, 0);
  glRotatef(90, 0.0, 0.0, 1.0);
  glRotatef(90, 1.0, 0.0, 0.0);
  glRotatef(currentAnguloAsas, 0.0, 0.0, 1.0);
  desenharPiramide(0.5, 1.5, 3);
  glPopMatrix();

  // asa esquerda
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glRotatef(90, 0.0, 1.0, 0);
  glRotatef(90, 0.0, 0.0, 1.0);
  glRotatef(-90, 1.0, 0.0, 0.0);
  glRotatef(currentAnguloAsas, 0.0, 0, 1);
  desenharPiramide(0.5, 1.5, 3);
  glPopMatrix();

  // bico
  glPushMatrix();
  glTranslatef(0, 0, 1);
  glRotatef(90, 0.0, 1.0, 0);
  glRotatef(90, 0.0, 0.0, 1.0);
  desenharPiramide(1.5, 1.5, 2);
  glPopMatrix();

  // rabo
  glPushMatrix();
  glTranslatef(0, 0, -2.5);
  glRotatef(90, 0.0, 1.0, 0);
  glRotatef(90, 0.0, 0.0, 1.0);
  desenharPiramide(0.5, 0.5, 2);
  glPopMatrix();

  // corpo
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glRotatef(90, 0.0, 1.0, 0.0);
  desenhaParalelepipedo(2, 1, 1);
  glPopMatrix();
  glPopMatrix();
}