#pragma once

#include "Camera.h"

class PixelShader
{
public:
    virtual void shade(std::unique_ptr<Camera> &ca) = 0;
};

class NormalShader : public PixelShader
{
    void shade(std::unique_ptr<Camera> &ca)
    {
        for (int i = 0; i < ca->width; i++)
            for (int j = 0; j < ca->height; j++)
            {
                ca->setColor(i, j, ca->getNormal(i, j));
            }
    }
};