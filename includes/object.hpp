#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object {
public:
  vec3 pos;
  Object() : pos(vec3(0.0, 0.0, 0.0)) {}
  Object(vec3 pos_) : pos(pos_) {}
};

#endif