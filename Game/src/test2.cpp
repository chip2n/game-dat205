#include "RenderEngine.h"
#include "Player.h"
#include <sstream>
#include <cmath>
#include "rapidjson/document.h"
#include "Level.h"
#include "ResourceManager.h"

#define PICKUP_RADIUS 0.3

Camera camera(45.0f, 640, 480);
Player player;
ResourceManager resourceManager;

glm::vec3 movementDirection;
glm::vec3 playerMovementDirection;
glm::vec3 playerForward = glm::vec3(0,0,-1);
float playerRotation;
int numKeys = 0;
int width;
int height;
void key_callback(int key, int action) {
    if(key == GLFW_KEY_UP) {
        if(action == GLFW_PRESS) {
            numKeys++;
            playerMovementDirection.z = -1;
        } else if(action == GLFW_RELEASE) {
            if(playerMovementDirection.z != 1)
                playerMovementDirection.z = 0;
            numKeys--;
        }
    }
    if(key == GLFW_KEY_DOWN) {
        if(action == GLFW_PRESS) {
            numKeys++;
            playerMovementDirection.z = 1;
        } else if(action == GLFW_RELEASE) {
            if(playerMovementDirection.z != -1)
                playerMovementDirection.z = 0;
            numKeys--;
        }
    }
    if(key == GLFW_KEY_LEFT) {
        if(action == GLFW_PRESS) {
            numKeys++;
            playerMovementDirection.x = -1;
        } else if(action == GLFW_RELEASE) {
            if(playerMovementDirection.x != 1)
                playerMovementDirection.x = 0;
            numKeys--;
        }
    }
    if(key == GLFW_KEY_RIGHT) {
        if(action == GLFW_PRESS) {
            numKeys++;
            playerMovementDirection.x = 1;
        } else if(action == GLFW_RELEASE) {
            if(playerMovementDirection.x != -1)
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
    lastX = xpos;
    lastY = ypos;
}

static void resize_callback(GLFWwindow* window, int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, width, height);
}

void printMatrix(glm::mat4 m) {
    std::cout << "---------------------------------------" << std::endl;
    std::cout << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << std::endl;
    std::cout << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << std::endl;
    std::cout << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << std::endl;
    std::cout << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << std::endl;
}

float sign(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    return (p1.x - p3.x) * (p2.z - p3.z) * (p2.x - p3.x) * (p1.z - p3.z);
}

bool pointInsideTriangle(glm::vec3 p, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) {
    /*
    bool b1, b2, b3;

    b1 = sign(p, t1, t2) < 0.0f;
    b2 = sign(p, t2, t3) < 0.0f;
    b3 = sign(p, t3, t1) < 0.0f;

    return ((b1 == b2) && (b2 ==  b3));
    glm::vec3 pt1 = glm::normalize(t1-p);
    glm::vec3 pt2 = glm::normalize(t2-p);
    glm::vec3 pt3 = glm::normalize(t3-p);

    std::cout << "RESULT: " << glm::angle(pt1, pt2) + glm::angle(pt2, pt3) + glm::angle(pt3, pt1) << std::endl;
    */
    float s = p0.z * p2.x - p0.x * p2.z + (p2.z - p0.z) * p.x + (p0.x - p2.x) * p.z;
    float t = p0.x * p1.z - p0.z * p1.x + (p0.z - p1.z) * p.x + (p1.x - p0.x) * p.z;

    if ((s < 0) != (t < 0))
        return false;

    float a = -p1.z * p2.x + p0.z * (p2.x - p1.x) + p0.x * (p1.z - p2.z) + p1.x * p2.z;
    if (a < 0.0)
    {
        s = -s;
        t = -t;
        a = -a;
    }
    return s > 0 && t > 0 && (s + t) < a;
}

bool pointInsideMaze(Model &maze, glm::vec3 point) {
    for(unsigned int i = 0; i < maze.positions.size(); i+=3) {
        glm::vec3 p1 = maze.positions[i];
        glm::vec3 p2 = maze.positions[i+1];
        glm::vec3 p3 = maze.positions[i+2];

        if(pointInsideTriangle(point, p1, p2, p3)) {
//            std::cout << "Point (" << point.x << "," << point.y << "," << point.z << ") is inside (" << p1.x << "," << p1.y << "," << p1.z << "),(" << p2.x << "," << p2.y << "," << p2.z << "),(" << p3.x << "," << p3.y << "," << p3.z << ")" << std::endl;
            return true;
        }
    }

    return false;
}

bool isCollision(Model &levelCollision, Player &player) {
    if(!pointInsideMaze(levelCollision, player.getPosition())) {
        return true;
    }
    return false;
}

int coins = 0;
void handleItemCollisions(std::vector<GameObject> &objects, Player &player) {
    std::vector<GameObject>::iterator iter;
    for(iter = objects.begin(); iter != objects.end(); ) {
        float distance = glm::length(iter->getPosition() - player.getPosition());
        if(distance < PICKUP_RADIUS) {
            std::cout << "Picked up a coin" << std::endl;
            coins++;
            std::cout << "You now have " << coins << " coins." << std::endl;
            iter = objects.erase(iter);
        } else {
            iter++;
        }
    }
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
    ShaderProgram staticShader("assets/shaders/simpleshading.vert", "assets/shaders/simpleshading.frag");
    ShaderProgram shadowmapShaderProgram("assets/shaders/shadowmap.vert", "assets/shaders/shadowmap.frag");

    camera.setPosition(glm::vec3(0,5,4));
    camera.update();

    //Level l;
    //l.loadLevel("assets/data/levels/maze.json");
    Level *l = resourceManager.loadLevel("assets/data/levels/maze.json");
    l = resourceManager.loadLevel("assets/data/levels/maze.json");
    //Level l = resourceManager.levels["assets/data/levels/maze.json"];


    Mesh monkey;
    monkey.loadMesh("assets/unfinished/skull.dae");
    Animation restAnim = Animation("rest", 9, 249, 333, 12.1f);
    monkey.addAnimation(restAnim);
    Animation runAnim = Animation("run", 296, 320, 320, 9.0f);
    monkey.addAnimation(runAnim);

    Texture texture("assets/textures/bricks.png");

    Environment env;
    for(unsigned int i = 0; i < l->getLights().size(); i++) {
        std::cout << "YO" << std::endl;
        env.addLight(l->getLights()[i]);
    }
    env.addLight(Light(glm::vec3(1,1,0)));

    Model levelCollision;
    levelCollision.loadFromFile("assets/unfinished/maze_col.obj");
    ModelInstance levelCollisionInstance(&levelCollision);

    Mesh coin;
    coin.loadMesh("assets/unfinished/coin.obj");

    ShadowMap shadowMap(shadowmapShaderProgram);

    double lastTime = glfwGetTime();
    double deltaTime = lastTime;
    double runTime = 0;
    double restTime = 0;
    glm::vec3 currentCubeNormal = glm::vec3(0,1,0);
	while(!glfwWindowShouldClose(window.window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glCullFace(GL_BACK);

        std::cout << "Player z: " << player.getPosition().z << std::endl;

        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        if(player.isControllable()) {
            player.rotate(playerRotation, currentCubeNormal, 0.15f);

            if(playerMovementDirection != glm::vec3(0)) {
                glm::vec3 oldPosition = player.getPosition();
                player.move(glm::normalize(playerMovementDirection));
                player.update(deltaTime);
                if(isCollision(levelCollision, player)) {
                    player.setPosition(oldPosition);
                    player.move(glm::vec3(0));
                } else {
                camera.move((float)deltaTime * 2.5f * glm::normalize(playerMovementDirection));
                }
            } else {
                player.move(glm::vec3(0));
            }
        }

        handleItemCollisions(l->gameObjects, player);

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

        monkey.render(shaderProgram, camera, env, player.getPosition(), player.up, player.currentRotation);
        shaderProgram.end();

        l->levelMesh->render(staticShader, camera, env);
        //coin.render(staticShader, camera, env);

        for(unsigned int i = 0; i < l->gameObjects.size(); i++) {
            l->gameObjects[i].mesh->render(staticShader, camera, env, l->gameObjects[i].getPosition());
        }

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}
}

