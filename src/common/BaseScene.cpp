#include "BaseScene.h"

// 0: Left, 1: Up, 2: Right, 3: Down
void BaseScene::objMoveInCaSpace(float distance, int direction)
{
    if (!selectedObj)
    {
        return;
    }
    selectedObj->translate(ca->getDirection(direction) * distance);
    return;
}

void BaseScene::objScale(float factor)
{
    if (!selectedObj)
    {
        return;
    }
    selectedObj->scale(factor);
    return;
}

void BaseScene::objReset()
{
    if (!selectedObj)
    {
        return;
    }
    selectedObj->reset();
    return;
}

void BaseScene::objRotate(float angle, int axis)
{
    if (!selectedObj)
    {
        return;
    }
    selectedObj->rotate(angle, axis);
    return;
}

void BaseScene::switchLight()
{
    int totNum = lights.size() + 1;
    selectLight += 1;
    if (selectLight >= totNum)
    {
        selectLight = 0;
        std::cout << "Current selection: Camera" << std::endl;
    }
    else
    {
        std::cout << "Current selection: Light " << selectLight - 1 << std::endl;
    }
}

void BaseScene::moveLight(float distance, int direction)
{
    if (selectLight == 0)
    {
        switch (direction)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            ca->pan(distance, direction);
            break;
        case 4:
            ca->dolly(distance);
            break;
        case 5:
            ca->dolly(-distance);
            break;
        }
    }
    else
    {
        lights[selectLight - 1]->move(distance, direction);
    }
}