#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include "Types.h"
#include "Engine.h"

class CCamera
{
public:
    CCamera() = default;
    ~CCamera() = default;
    Vec3 GetForward();
    Vec3 GetRight();
    void Rotate(Vec3 Amt);
    void Translate(Vec3 Amt);
    void Update(CEngineDevice* Dev);
private:
    Vec3 Position;
    float Yaw, Pitch;
};

