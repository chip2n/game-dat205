#include "RenderEngine.h"

Camera camera(45.0f, 640, 480);

glm::vec3 movementDirection;
void key_callback(int key, int action) {
    /*
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
        */

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
     Window window;

     window.setKeyCallback(key_callback);

    // Set up all them callbacks, yo
	// glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, mouse_callback);
    glfwSetWindowSizeCallback(window.window, resize_callback);

    // Initiate GLEW or crash everything if failure, yo
    if(glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }

    // Load shaders, yo
    ShaderProgram billboardShaderProgram("assets/shaders/texture_noshading.vert", "assets/shaders/texture_noshading.frag");
    ShaderProgram shaderProgram("assets/shaders/simpleshading.vert", "assets/shaders/simpleshading.frag");
    ShaderProgram shadowmapShaderProgram("assets/shaders/shadowmap.vert", "assets/shaders/shadowmap.frag");

    // Init camera at position (2,3,3) looking at origin, yo
    camera.setPosition(glm::vec3(2,3,3));
    camera.lookAt(glm::vec3(0,0,0));
    camera.update();


    Model model;
    model.loadFromFile("assets/models/cube.obj");
    ModelInstance modelInstance(&model);

    Model monkey;
    monkey.loadFromFile("assets/models/monkey.obj");
    ModelInstance monkeyInstance(&monkey);
    monkeyInstance.move(glm::vec3(10,0,0));

    ModelInstance monkeyInstance2(&monkey);
    monkeyInstance2.move(glm::vec3(0,2,0));

    Texture texture("assets/textures/bricks.png");
    Texture testTex("assets/textures/light.png");

    Environment env;
    Light light(glm::vec3(10, 4, 2));
    env.addLight(light);

    Billboard billboard;
    billboard.move(glm::vec3(10, 4, 2));

    ShadowMap shadowMap(shadowmapShaderProgram);

    double lastTime = glfwGetTime();
    double deltaTime = lastTime;
	while(!glfwWindowShouldClose(window.window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glCullFace(GL_BACK);

        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        camera.move(10*(float)deltaTime * (camera.getDirection() * movementDirection.y));
        camera.move(10*(float)deltaTime * (camera.getRight() * movementDirection.x));
        camera.update();

        monkeyInstance.rotate(-deltaTime*30, glm::vec3(0,1,0));
        monkeyInstance2.rotate(deltaTime*10, glm::vec3(0,1,0));

        shadowMap.begin();
        shadowMap.render(modelInstance);
        shadowMap.render(monkeyInstance);
        shadowMap.render(monkeyInstance2);
        shadowMap.end();


        glActiveTexture(GL_TEXTURE0);
        texture.bind();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadowMap.depthTexture);
        shaderProgram.begin();
        shaderProgram.setUniform("texSampler", 0);
        shaderProgram.setUniform("shadowMap", 1);
        glm::mat4 depthBiasMVP = shadowMap.getBiasMVP(glm::vec3(0,0,0));
        shaderProgram.setUniform("depthBiasMVP", depthBiasMVP);
        shaderProgram.end();
        modelInstance.render(camera, env, shaderProgram);


        shaderProgram.begin();
        //depthBiasMVP = shadowMap.getBiasMVP(monkeyInstance.position);
        depthBiasMVP = shadowMap.getBiasMVP(glm::vec3(0,0,0)) * monkeyInstance.getModelMatrix();
        shaderProgram.setUniform("depthBiasMVP", depthBiasMVP);
        shaderProgram.end();
        monkeyInstance.render(camera, env, shaderProgram);

        shaderProgram.begin();
        //depthBiasMVP = shadowMap.getBiasMVP(monkeyInstance2.position);
        depthBiasMVP = shadowMap.getBiasMVP(glm::vec3(0,0,0)) * monkeyInstance2.getModelMatrix();
        shaderProgram.setUniform("depthBiasMVP", depthBiasMVP);
        shaderProgram.end();
        monkeyInstance2.render(camera, env, shaderProgram);



        texture.unbind();
        glActiveTexture(GL_TEXTURE0);
        testTex.bind();
        billboard.render(camera, env, billboardShaderProgram);
        testTex.unbind();


		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}
}
