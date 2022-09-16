#pragma once

class DepthBuffer {
public:
    DepthBuffer();
    ~DepthBuffer();
    
    bool update(int x, int y, float depth);
    void clear();
    void resize(int width, int height);

    int width;
    int height;

private:
    float* buffer;
};