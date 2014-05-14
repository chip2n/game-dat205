#include <iostream>
#include <fstream>
#include <sstream>
#include "Level.h"
#include "rapidjson/document.h"

std::vector<Light> Level::getLights() {
    //return levelMesh->lights;
    return lights;
}
