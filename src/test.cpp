#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>
#include "ShaderProgram.h"
#include "Camera.h"
#include "Room.h"
#include "OBJLoader.h"
#include "Model.h"
#include "ModelInstance.h"
#include "Environment.h"
#include "Light.h"
#include "Billboard.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Camera camera(45.0f, 640, 480);

struct MyMesh {
    unsigned int numFaces;
    GLuint vao;
};
std::vector<MyMesh> meshes;

void processScene(const aiScene *sc) {
    MyMesh aMesh;
    GLuint buffer;

    for(unsigned int n = 0; n < sc->mNumMeshes; ++n) {
        const aiMesh* mesh = sc->mMeshes[n];

        unsigned int *faceArray;
        faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
        unsigned int faceIndex = 0;

        for(unsigned int t = 0; t < mesh->mNumFaces; ++t) {
            const aiFace* face = &mesh->mFaces[t];

            memcpy(&faceArray[faceIndex], face->mIndices, 3*sizeof(unsigned int));
            faceIndex+=3;
        }
        aMesh.numFaces = sc->mMeshes[n]->mNumFaces;

        glGenVertexArrays(1, &(aMesh.vao));
        glBindVertexArray(aMesh.vao);

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

        if(mesh->HasPositions()) {
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);  //TODO: Rätt?
            glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
        }

        if(mesh->HasNormals()) {
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
            glEnableVertexAttribArray(2);  //TODO: Rätt?
            glVertexAttribPointer(2, 3, GL_FLOAT, 0, 0, 0);
        }

        if(mesh->HasTextureCoords(0)) {
            float *texCoords = (float*)malloc(sizeof(float)*2*mesh->mNumVertices);
            for(unsigned int k = 0; k < mesh->mNumVertices; ++k) {
                texCoords[k*2] = mesh->mTextureCoords[0][k].x;
                texCoords[k*2+1] = mesh->mTextureCoords[0][k].y;
            }
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);  //TODO: Rätt?
            glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        meshes.push_back(aMesh);
    }
}

bool assimpImport(const std::string &pFile) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( pFile,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

    if(!scene) {
        std::cout << "WFEGFD" << std::endl;
        return false;
    }

    processScene(scene);

    return true;
}

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

glm::vec3 movementDirection;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

    if(key == GLFW_KEY_W) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.y = 1;
        } else {
            movementDirection.y = 0;
        }
    }
    if(key == GLFW_KEY_S) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.y = -1;
        } else {
            movementDirection.y = 0;
        }
    }
    if(key == GLFW_KEY_A) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.x = -1;
        } else {
            movementDirection.x = 0;
        }
    }
    if(key == GLFW_KEY_D) {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            movementDirection.x = 1;
        } else {
            movementDirection.x = 0;
        }
    }
}

double lastX, lastY;
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    double deltaX = xpos - lastX;
    double deltaY = ypos - lastY;

    camera.rotate(glm::vec3(0,1,0), -deltaX/10);
    camera.rotate(camera.getRight(), -deltaY/10);
    camera.update();

    lastX = xpos;
    lastY = ypos;
}

static void resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, const char *argv[]) {
    // Initiate GLFW or crash everything if failure, yo
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	if(!glfwInit()) {
		exit(EXIT_FAILURE);
    }

	 window = glfwCreateWindow(640, 480, "Title", NULL, NULL);

	if(!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

    // Set up all them callbacks, yo
	glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetWindowSizeCallback(window, resize_callback);

	glfwMakeContextCurrent(window);

    // Disable the cursor, yo
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initiate GLEW or crash everything if failure, yo
    if(glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }


    // Load shaders, yo
    ShaderProgram billboardShaderProgram("texture_noshading.vert", "texture_noshading.frag");
    ShaderProgram shaderProgram("simpleshading.vert", "simpleshading.frag");
    ShaderProgram shadowmapShaderProgram("shadowmap.vert", "shadowmap.frag");

    // Init camera at position (2,3,3) looking at origin, yo
    camera.setPosition(glm::vec3(2,3,3));
    camera.lookAt(glm::vec3(0,0,0));
    camera.update();


    Model model;
    model.loadFromFile("cube.obj");
    ModelInstance modelInstance(&model);

    Model monkey;
    monkey.loadFromFile("monkey.obj");
    ModelInstance monkeyInstance(&monkey); // TODO: REMOVEs BILLBOARD WTF?
    monkeyInstance.move(glm::vec3(3,0,0));

    ModelInstance monkeyInstance2(&monkey);
    monkeyInstance2.move(glm::vec3(0,2,0));

    Texture texture("bricks.png");
    Texture testTex("light.png");

    Environment env;
    Light light(glm::vec3(10, 4, 2));
    env.addLight(light);

    Billboard billboard;
    billboard.move(glm::vec3(10, 4, 2));














    // SHADOW MAP STUFF
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Failed to set up shadow map framebuffer." << std::endl;
    }

    glm::vec3 lightInvDir = glm::vec3(10, 4, 2);
    
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
        
    glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
            );
    glm::mat4 depthBiasMVP = biasMatrix * depthMVP;












    double lastTime = glfwGetTime();
    double deltaTime = lastTime;
	while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glCullFace(GL_BACK);

        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        camera.move(10*(float)deltaTime * (camera.getDirection() * movementDirection.y));
        camera.move(10*(float)deltaTime * (camera.getRight() * movementDirection.x));
        camera.update();


        // Render shadowmap
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, 1024, 1024);
        glDrawBuffer(GL_NONE);
        shadowmapShaderProgram.begin();
        shadowmapShaderProgram.setUniform("depthMVP", depthMVP);
        modelInstance.render(camera, env, shadowmapShaderProgram);
        monkeyInstance.render(camera, env, shadowmapShaderProgram);
        monkeyInstance2.render(camera, env, shadowmapShaderProgram);
        shadowmapShaderProgram.end();

        // End render shadowmap
        


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);
        glActiveTexture(GL_TEXTURE0);
        texture.bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
        shaderProgram.begin();
        shaderProgram.setUniform("texSampler", 0);
        shaderProgram.setUniform("shadowMap", 1);
        shaderProgram.setUniform("depthBiasMVP", depthBiasMVP);
        shaderProgram.end();
        modelInstance.render(camera, env, shaderProgram);
        monkeyInstance.render(camera, env, shaderProgram);
        monkeyInstance2.render(camera, env, shaderProgram);
        texture.unbind();
    glActiveTexture(GL_TEXTURE0);
        testTex.bind();
        billboard.render(camera, env, billboardShaderProgram);
        testTex.unbind();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
