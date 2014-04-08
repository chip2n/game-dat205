#include <iostream>
#include "Animation.h"

Animation::Animation(std::string n, int startF, int endFrame, int totalFrames, float totalDuration) {
    startFrame = startF;
    timePerFrame = totalDuration / totalFrames;
    float animP = (endFrame - startFrame) / (float)totalFrames;
    animDuration = animP * totalDuration;
    name = n;
}

Animation::Animation(const Animation& other) : name(other.name), animDuration(other.animDuration), timePerFrame(other.timePerFrame), startFrame(other.startFrame) {

}
