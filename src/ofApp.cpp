#include <chrono>

#include "GeoObject.h"
#include "Scene.h"
#include "Shader.h"
#include "ofApp.h"

using namespace std::chrono;

//--------------------------------------------------------------
void ofApp::setup()
{
    w = 640;
    h = 480;
    colorPixels.allocate(w, h, OF_PIXELS_RGB);

    scene.ca =
        std::make_unique<Camera>(Vector3f(0, 0, -1), Vector3f(0, 0, 1), 1.0f, w, h, 2.0f, 1.5f, 1);
    scene.pixels = &colorPixels;
    scene.shader = std::make_unique<LambertianShader>();
    // scene.shader = std::make_unique<NormShader>();

    scene.lights.push_back(std::make_shared<Light>(Vector3f(0, 5, 0), Vector3f(1, 1, 1)));
    scene.lights.push_back(std::make_shared<Light>(Vector3f(0, 3, -2), Vector3f(0.5, 0.5, 0.5)));

    auto sphere = std::make_shared<Sphere>(Vector3f(-0.8, 0.8, 1), 0.5, Vector3f(1, 0, 0));
    sphere->set_specular(0.8);
    scene.objects.push_back(sphere);

    auto elipsoid = std::make_shared<Elipsoid>(Vector3f(0.2, 0.4, 0.3), Vector3f(1.6, -0.5, 2),
                                               Vector3f(0, 1, 0));
    scene.objects.push_back(elipsoid);

    scene.objects.push_back(
        std::make_shared<Sphere>(Vector3f(-0.4, -0.3, -0.5), 0.2, Vector3f(0, 0, 1)));

    scene.objects.push_back(std::make_shared<Cylinder>(Vector3f(0, 0, 2), Vector3f(0.5, 1, -0.2),
                                                       0.5, 1, -1, Vector3f(1, 1, 0)));

    auto plane =
        std::make_shared<Plane>(Vector3f(0, -1.4, 0), Vector3f(0, 1, 0), Vector3f(0.5, 0.5, 0.5));
    plane->set_specular(0.5);
    scene.objects.push_back(plane);
}

//--------------------------------------------------------------
void ofApp::update()
{
    auto start = high_resolution_clock::now();
    scene.render();
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
        if (scene.ca->perspective)
        {
            scene.ca->setParallel();
        }
        else
        {
            scene.ca->setPerspective();
        }
        break;
    case 'q':
        scene.shader = std::move(std::make_unique<LambertianShader>());
        break;
    case 'e':
        scene.shader = std::move(std::make_unique<PhongShader>());
        break;
    case 'n':
        scene.ca->dolly(0.1f);
        break;
    case 'm':
        scene.ca->dolly(-0.1f);
        break;
    case 'a':
        scene.ca->pan(0.1f, 0);
        break;
    case 'w':
        scene.ca->pan(0.1f, 1);
        break;
    case 'd':
        scene.ca->pan(0.1f, 2);
        break;
    case 's':
        scene.ca->pan(0.1f, 3);
        break;

    case 'f':
        scene.ca->orbit(pi / 18, 3, 0);
        break;
    case 't':
        scene.ca->orbit(pi / 18, 3, 1);
        break;
    case 'h':
        scene.ca->orbit(pi / 18, 3, 2);
        break;
    case 'g':
        scene.ca->orbit(pi / 18, 3, 3);
        break;
    case ',':
        scene.ca->reset();
        break;

    // Left arrow
    case 57356:
        scene.objMoveInCaSpace(0.1, 0);
        break;

    // Up arrow
    case 57357:
        scene.objMoveInCaSpace(0.1, 1);
        break;

    // Right arrow
    case 57358:
        scene.objMoveInCaSpace(0.1, 2);
        break;

    // Down arrow
    case 57359:
        scene.objMoveInCaSpace(0.1, 3);
        break;

    case '+':
        scene.objScale(1.1f);
        break;
    case '-':
        scene.objScale(1.0f / 1.1f);
        break;

    case 'j':
        scene.objRotate(pi / 18, 0);
        break;
    case 'k':
        scene.objRotate(pi / 18, 1);
        break;
    case 'l':
        scene.objRotate(pi / 18, 2);
        break;

    case 'J':
        scene.objRotate(-pi / 18, 0);
        break;
    case 'K':
        scene.objRotate(-pi / 18, 1);
        break;
    case 'L':
        scene.objRotate(-pi / 18, 2);
        break;

    case '.':
        scene.objReset();
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    switch (button)
    {
    case 0:
        scene.select(x, y);
        break;

    default:
        std::cout << button << " " << std::endl;
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

void ofApp::mouseScrolled(ofMouseEventArgs &args)
{
    if (args.scrollY == 1)
    {
        scene.ca->zoom(0.05f);
    }
    else if (args.scrollY == -1)
    {
        scene.ca->zoom(-0.05f);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}