#include "ResourceManager.h"
#include "rapidjson/document.h"
#include <iostream>
#include <fstream>
#include <sstream>

Mesh* ResourceManager::loadMesh(std::string path) {
    if(meshes.find(path) == meshes.end()) {
        Mesh *mesh = new Mesh();
        mesh->loadMesh(path);
        meshes.insert(std::pair<std::string, Mesh*>(path, mesh));
        return mesh;
    } else {
        return meshes[path];
    }
}

Level* ResourceManager::loadLevel(std::string path) {
    if(levels.find(path) == levels.end()) {
        Level level;
        std::string json;
        std::ifstream shaderStream(path, std::ios::in);
        if(shaderStream.is_open()) {
            std::string line = "";
            while(getline(shaderStream, line)) {
                json += "\n" + line;
            }
            shaderStream.close();
        } else {
            std::cout << "ERROR: Could not open " + path << std::endl;
            return NULL;
        }

        rapidjson::Document d;
        d.Parse<0>(json.c_str());

        // Loading level mesh
        level.levelMesh = loadMesh(d["level"]["mesh"].GetString());

        // Loading game objects
        const rapidjson::Value& objects = d["objects"];
        assert(objects.IsArray());
        for(rapidjson::SizeType i = 0; i < objects.Size(); i++) {
            std::stringstream ss;
            ss << "assets/data/gameobjects/" << objects[i]["id"].GetString() << ".json";
            GameObject gameObject = loadGameObject(ss.str());
            glm::vec3 position = glm::vec3(objects[i]["pos"]["x"].GetInt(), objects[i]["pos"]["y"].GetInt(), objects[i]["pos"]["z"].GetInt());
            gameObject.setPosition(position);
            level.gameObjects.push_back(gameObject);
        }

        // Loading lights
        const rapidjson::Value& l = d["lights"];
        assert(objects.IsArray());
        for(rapidjson::SizeType i = 0; i < l.Size(); i++) {
            glm::vec3 position = glm::vec3(l[i]["position"]["x"].GetDouble(), l[i]["position"]["y"].GetDouble(), l[i]["position"]["z"].GetDouble());
            Light light(position);
            light.intensity = l[i]["intensity"].GetDouble();
            level.environment.addLight(light);
        }
        levels.insert(std::pair<std::string, Level>(path, level));


        return &levels[path];
    } else {
        return &levels[path];
    }
}

GameObject ResourceManager::loadGameObject(std::string path) {
    GameObject gameObject;

    std::string json;
    std::ifstream shaderStream(path, std::ios::in);
    if(shaderStream.is_open()) {
        std::string line = "";
        while(getline(shaderStream, line)) {
            json += "\n" + line;
        }
        shaderStream.close();
    } else {
        std::cout << "ERROR: Could not open " + path << std::endl;
    }

    rapidjson::Document d;
    d.Parse<0>(json.c_str());

    gameObject.collectable = d["collectable"].GetBool();    
    gameObject.passable = d["passable"].GetBool();
    gameObject.collectAnimation = d["collectAnimation"].GetString();

    gameObject.mesh = loadMesh(d["mesh"].GetString());
    
    return gameObject;
}
