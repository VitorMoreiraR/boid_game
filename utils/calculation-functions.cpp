#ifndef BOID_CPP
#define BOID_CPP

#include <random>
#include <limits>

inline int randomInt(int min, int max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

inline float randomFloat(float min_val, float max_val) {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<float> distrib(min_val, max_val);
    return distrib(gen);
}

#endif