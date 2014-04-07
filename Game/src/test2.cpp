#include "RenderEngine.h"
#include <sstream>

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

void printMatrix(glm::mat4 m) {
    std::cout << "---------------------------------------" << std::endl;
    std::cout << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << std::endl;
    std::cout << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << std::endl;
    std::cout << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << std::endl;
    std::cout << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << std::endl;
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
    ShaderProgram shaderProgram("assets/shaders/simple_shading_texture_skinning_bones.vert", "assets/shaders/simple_shading_texture_skinning_bones.frag");
    ShaderProgram staticShader("assets/shaders/simple_shading_texture_skinning.vert", "assets/shaders/simple_shading_texture_skinning.frag");

    // Init camera at position (2,3,3) looking at origin, yo
    camera.setPosition(glm::vec3(2,3,3));
    camera.lookAt(glm::vec3(0,0,0));
    camera.update();


    Mesh monkey;
    monkey.loadMesh("assets/unfinished/skull.dae");
    //ModelInstance monkeyInstance(&monkey);
    //monkeyInstance.move(glm::vec3(10,0,0));

    Texture texture("assets/textures/bricks.png");

    Environment env;
    Light light(glm::vec3(10, 4, 2));
    env.addLight(light);

    Model level;
    level.loadFromFile("assets/unfinished/tiles.obj");
    ModelInstance levelInstance(&level);

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





        std::vector<glm::mat4> transforms;
        transforms.resize(4);
        shaderProgram.begin();
        monkey.boneTransform((float)lastTime, transforms);

        for(uint i = 0; i < transforms.size(); i++) {
            std::stringstream sstm;
            sstm << "gBones[" << i << "]";
            shaderProgram.setUniform(sstm.str().c_str(), transforms[i]);
        }

        shaderProgram.setUniform("modelViewProjectionMatrix", camera.getCombinedMatrix());
        glm::mat4 modelM = glm::mat4(1.0f);
        shaderProgram.setUniform("modelMatrix", modelM);
        shaderProgram.setUniform("lightPos", env.getLights()[0].getPosition());
        monkey.render();
        shaderProgram.end();


        //levelInstance.render(camera, env, staticShader);

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}
}

