#include "RenderEngine.h"
#include "Player.h"
#include <sstream>
#include <cmath>

Camera camera(45.0f, 640, 480);
Player player;

glm::vec3 movementDirection;
glm::vec3 playerMovementDirection;
glm::vec3 playerForward = glm::vec3(0,0,-1);
float playerRotation;
int numKeys = 0;
void key_callback(int key, int action) {
    /*
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
        */

    float oldRotation = playerRotation;
    glm::vec2 oldP = glm::vec2(playerMovementDirection.x, playerMovementDirection.z);
    if(key == GLFW_KEY_UP) {
        if(action == GLFW_PRESS) {
            numKeys++;
            playerMovementDirection.z = -1;
        } else if(action == GLFW_RELEASE) {
            playerMovementDirection.z = 0;
            numKeys--;
        }
    }
    if(key == GLFW_KEY_DOWN) {
        if(action == GLFW_PRESS) {
            numKeys++;
            playerMovementDirection.z = 1;
        } else if(action == GLFW_RELEASE) {
            playerMovementDirection.z = 0;
            numKeys--;
        }
    }
    if(key == GLFW_KEY_LEFT) {
        if(action == GLFW_PRESS) {
            numKeys++;
            playerMovementDirection.x = -1;
        } else if(action == GLFW_RELEASE) {
            playerMovementDirection.x = 0;
            numKeys--;
        }
    }
    if(key == GLFW_KEY_RIGHT) {
        if(action == GLFW_PRESS) {
            numKeys++;
            playerMovementDirection.x = 1;
        } else if(action == GLFW_RELEASE) {
            playerMovementDirection.x = 0;
            numKeys--;
        }
    }

    if(numKeys > 0) {
        glm::vec2 p = glm::vec2(playerMovementDirection.x, playerMovementDirection.z);
        float angle = 0.0f;
        if(p.y == 0) {
            angle = -90.0f * p.x;
        } else if(p.y < 0) {
            angle = (float) atan((double)p.x / p.y) * 180.0f / M_PI;
        } else {
            angle = (float) atan((double)p.x / p.y) * 180.0f / M_PI - 180.0f;
        }
        playerRotation = angle;
    }

    if(playerMovementDirection != glm::vec3(0)) {
        playerForward = playerMovementDirection;
        playerForward.x = -playerForward.x;
    }
}

double lastX, lastY;
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    double deltaX = xpos - lastX;
    double deltaY = ypos - lastY;

    //camera.rotate(glm::vec3(0,1,0), -deltaX/10);
    //camera.rotate(camera.getRight(), -deltaY/10);
    //camera.update();

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

    camera.setPosition(glm::vec3(0,5,4));
    camera.update();


    Mesh monkey;
    monkey.loadMesh("assets/unfinished/skull.dae");
    Animation restAnim = Animation("rest", 9, 249, 333, 12.1f);
    monkey.addAnimation(restAnim);
    Animation runAnim = Animation("run", 296, 328, 328, 9.0f);
    monkey.addAnimation(runAnim);
    //ModelInstance monkeyInstance(&monkey);
    //monkeyInstance.move(glm::vec3(10,0,0));

    Texture texture("assets/textures/bricks.png");

    Environment env;
    Light light(glm::vec3(10, 4, 2));
    env.addLight(light);

    Model level;
    level.loadFromFile("assets/unfinished/cube.obj");
    ModelInstance levelInstance(&level);
    levelInstance.move(glm::vec3(0,-5,0));

    Model levelCollision;
    levelCollision.loadFromFile("assets/unfinished/cube_collision.obj");




    double lastTime = glfwGetTime();
    double deltaTime = lastTime;
    double runTime = 0;
    double restTime = 0;
    int playerSide = 0;
    float sideChangeTime;
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

        if(player.isControllable()) {
            player.rotate(playerRotation, glm::vec3(0,1,0), 0.15f);

            if(playerMovementDirection != glm::vec3(0)) {
                player.move(glm::normalize(playerMovementDirection));
                camera.move((float)deltaTime * 3.0f * glm::normalize(playerMovementDirection));
            } else {
                player.move(glm::vec3(0));
            }
        }

        player.update(deltaTime);

        sideChangeTime += deltaTime;

        if(playerSide == 0 && player.getPosition().x > 5) {
            std::cout << "ABOVE" << std::endl;
            playerSide = 1;
            player.setControllable(false);
            player.stop();
            sideChangeTime = 0.0f;

            player.rotate(-90.0f, glm::vec3(0,0,1), 0.3f);
            player.up = glm::vec3(1,0,0);
            playerSide = 2;
        }


        if(sideChangeTime > 0.3f) {
            player.setControllable(true);
        }

        camera.lookAt(player.getPosition());
        camera.update();

        std::vector<glm::mat4> transforms;
        transforms.resize(4);
        shaderProgram.begin();

        if(playerMovementDirection != glm::vec3(0)) {
            runTime += deltaTime;
            restTime = 0;
            monkey.boneTransform("run", (float)runTime, transforms);
        } else {
            runTime = 0;
            restTime += deltaTime;
            monkey.boneTransform("rest", (float)restTime, transforms);
        }

        for(uint i = 0; i < transforms.size(); i++) {
            std::stringstream sstm;
            sstm << "gBones[" << i << "]";
            shaderProgram.setUniform(sstm.str().c_str(), transforms[i]);
        }

        monkey.render(shaderProgram, camera, env, player.getPosition(), glm::vec3(0,1,0), player.currentRotation);
        shaderProgram.end();


        levelInstance.render(camera, env, staticShader);

        //std::cout << "Player position: (" << player.getPosition().x << "," << player.getPosition().y << "," << player.getPosition().z << ")" << std::endl;


		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}
}

bool pointInsideMaze(Model &maze, glm::vec3 point) {
    for(int i = 0; i < maze.positions.size(); i++) {
        glm::vec3 p = maze.positions[i];
        std::cout << "Position: (" << p.x << "," << p.y << "," << p.z << ")" << std::endl;


    }
}
