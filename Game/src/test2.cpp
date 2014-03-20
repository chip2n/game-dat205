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
    glfwSetCursorPosCallback(window.window, mouse_callback);
    glfwSetWindowSizeCallback(window.window, resize_callback);

    // Initiate GLEW or crash everything if failure, yo
    if(glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
    }

    // Load shaders, yo
    ShaderProgram shaderProgram("assets/shaders/simple_shading_texture_skinning.vert", "assets/shaders/simple_shading_texture_skinning.frag");

    // Init camera at position (2,3,3) looking at origin, yo
    camera.setPosition(glm::vec3(2,3,3));
    camera.lookAt(glm::vec3(0,0,0));
    camera.update();


    SkinnedModel monkey;
    monkey.loadFromFile("assets/unfinished/editable-person.dae");
    ModelInstance monkeyInstance(&monkey);
    //monkeyInstance.move(glm::vec3(10,0,0));

    Texture texture("assets/textures/bricks.png");

    Environment env;
    Light light(glm::vec3(10, 4, 2));
    env.addLight(light);

    double lastTime = glfwGetTime();
    double deltaTime = lastTime;
	while(!glfwWindowShouldClose(window.window)) {
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


        shaderProgram.begin();
        monkeyInstance.render(camera, env, shaderProgram);
        shaderProgram.end();

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}
}
