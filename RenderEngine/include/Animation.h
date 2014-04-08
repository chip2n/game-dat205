#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>

class Animation {
    public:
        Animation(std::string n, int startFrame, int endFrame, int totalFrames, float totalDuration);
        Animation(const Animation& other);
        Animation() {};
        float animDuration;
        std::string name;
        float timePerFrame;
        int startFrame;
    private:
};

#endif
