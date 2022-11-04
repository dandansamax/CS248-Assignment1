#include <chrono>

#include "GeoObject.h"
#include "Mesh.h"
#include "RasterScene.h"
#include "Shader.h"
#include "ofApp.h"

using namespace std::chrono;

//--------------------------------------------------------------
void ofApp::setup()
{
    w = 640;
    h = 480;
    colorPixels.allocate(w, h, OF_PIXELS_RGB);

    scene = std::unique_ptr<BaseScene>(new RasterScene());

    auto mesh = make_shared<Mesh>("mesh/teapot.obj", Vector3f(0.0f, 0.0f, 0.0f));
    mesh->calVectexNormal();
    scene->ca = std::make_unique<Camera>(Vector3f(0, 3.0f, 6.0f), Vector3f(0, 0, -1), 3.0f, w, h,
                                         4.0f, 3.0f, 1);
    scene->ca->perspective = true;
    scene->pixels = &colorPixels;

    dynamic_cast<RasterScene &>(*scene).meshes.push_back(mesh);
    dynamic_cast<RasterScene &>(*scene).shader =
        std::unique_ptr<PhongBaseShader>(new PhongBaseShader());

    scene->lights.push_back(std::make_shared<Light>(Vector3f(0, 5, 0), Vector3f(1, 1, 1)));
}

//--------------------------------------------------------------
void ofApp::update()
{
    auto start = high_resolution_clock::now();
    scene->render();
    auto stop = high_resolution_clock::now();
    texColor.allocate(colorPixels);
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Takes " << duration.count() / 1000.0 << "ms to render"
         << "\t\r" << std::flush;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetHexColor(0xffffff);
    texColor.draw(0, 0, w, h);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key)
    {
    case 'p':
        if (scene->ca->perspective)
        {
            scene->ca->setParallel();
        }
        else
        {
            scene->ca->setPerspective();
        }
        break;

    case 'r':
        dynamic_cast<RasterScene &>(*scene).gouraud =
            1 - dynamic_cast<RasterScene &>(*scene).gouraud;
        break;
    /*
    case 'q':
        scene->shader = std::move(std::make_unique<LambertianShader>());
        break;
    case 'e':
        scene->shader = std::move(std::make_unique<PhongShader>());
        break;
    */
    case 'n':
        scene->moveLight(0.1f, 4);
        break;
    case 'm':
        scene->moveLight(0.1f, 5);
        break;
    case 'a':
        scene->moveLight(0.1f, 0);
        break;
    case 'w':
        scene->moveLight(0.1f, 1);
        break;
    case 'd':
        scene->moveLight(0.1f, 2);
        break;
    case 's':
        scene->moveLight(0.1f, 3);
        break;

    case '/':
        scene->switchLight();
        break;

    case 'f':
        scene->ca->orbit(pi / 18, 3, 0);
        break;
    case 't':
        scene->ca->orbit(pi / 18, 3, 1);
        break;
    case 'h':
        scene->ca->orbit(pi / 18, 3, 2);
        break;
    case 'g':
        scene->ca->orbit(pi / 18, 3, 3);
        break;
    case ',':
        scene->ca->reset();
        break;

    // Left arrow
    case 57356:
        scene->objMoveInCaSpace(0.1, 0);
        break;

    // Up arrow
    case 57357:
        scene->objMoveInCaSpace(0.1, 1);
        break;

    // Right arrow
    case 57358:
        scene->objMoveInCaSpace(0.1, 2);
        break;

    // Down arrow
    case 57359:
        scene->objMoveInCaSpace(0.1, 3);
        break;

    case '+':
        scene->objScale(1.1f);
        break;
    case '-':
        scene->objScale(1.0f / 1.1f);
        break;

    case 'j':
        scene->objRotate(pi / 18, 0);
        break;
    case 'k':
        scene->objRotate(pi / 18, 1);
        break;
    case 'l':
        scene->objRotate(pi / 18, 2);
        break;

    case 'J':
        scene->objRotate(-pi / 18, 0);
        break;
    case 'K':
        scene->objRotate(-pi / 18, 1);
        break;
    case 'L':
        scene->objRotate(-pi / 18, 2);
        break;

    case '.':
        scene->objReset();
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    switch (button)
    {
    case 0:
        scene->drag(x, y);
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    switch (button)
    {
    case 0:
        scene->click(x, y);
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    switch (button)
    {
    case 0:
        scene->release(x, y);
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

void ofApp::mouseScrolled(ofMouseEventArgs &args)
{
    if (args.scrollY == 1)
    {
        scene->ca->zoom(0.05f);
    }
    else if (args.scrollY == -1)
    {
        scene->ca->zoom(-0.05f);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}
