#ifndef DEBUG_H
#define DEBUG_H

void printVec3(glm::vec3 v) {
  std::cout << "(" << v.x << "," << v.y << "," << v.z << ")" << std::endl;
}

void printVec2(glm::vec2 v) {
  std::cout << "(" << v.x << "," << v.y << ")" << std::endl;
}

void printMatrix(glm::mat4 m) {
    std::cout << "---------------------------------------" << std::endl;
    std::cout << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << std::endl;
    std::cout << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << std::endl;
    std::cout << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << std::endl;
    std::cout << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << std::endl;
}

#endif
