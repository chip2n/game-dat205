#include <iostream>
#include "Animation.h"

Animation::Animation(std::string n, int startF, int endF, int totalF, float totalDuration) {
    startFrame = startF;
    endFrame = endF;
    totalFrames = totalF;
    timePerFrame = totalDuration / totalFrames;
    float animP = (endFrame - startFrame) / (float)totalFrames;
    animDuration = animP * totalDuration;
    name = n;
}

Animation::Animation(const Animation& other) : name(other.name), animDuration(other.animDuration), timePerFrame(other.timePerFrame), startFrame(other.startFrame), endFrame(other.endFrame), totalFrames(other.totalFrames) {

}
