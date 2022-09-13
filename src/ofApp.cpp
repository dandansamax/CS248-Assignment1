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

    scene.ca = std::move(std::make_unique<Camera>(Vector3f(0, 0, 0), Vector3f(0, 0, 1), w, h, 2.0f, 1.5f));
    scene.pixels = &colorPixels;
    scene.shader = std::move(std::make_unique<LambertianShader>());

    scene.lights.push_back(std::make_shared<Light>(Vector3f(0, 5, 0), Vector3f(1, 1, 1)));
    scene.lights.push_back(std::make_shared<Light>(Vector3f(0, 3, -2), Vector3f(0.5, 0, 0)));
    scene.objects.push_back(std::make_shared<Sphere>(Vector3f(-0.8, 0.8, 1), 0.5, Vector3f(1, 0, 0)));
    scene.objects.push_back(
        std::make_shared<Elipsoid>(Vector3f(0.2, 0.4, 0.3), Vector3f(1.6, -0.5, 2), Vector3f(0, 1, 0)));
    scene.objects.push_back(std::make_shared<Sphere>(Vector3f(-0.4, -0.3, -0.5), 0.2, Vector3f(0, 0, 1)));
    scene.objects.push_back(std::make_shared<Plane>(0, 1, 0, 1.4, Vector3f(0.5, 0.5, 0.5)));
}

//--------------------------------------------------------------
void ofApp::update()
{
    auto start = high_resolution_clock::now();
    scene.render();
    auto stop = high_resolution_clock::now();
    texColor.allocate(colorPixels);
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Takes " << duration.count()/1000.0 << "ms to render" << endl;
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
            scene.ca->setPerspective(1.0f);
        }
        break;
    case 'a':
        scene.shader = std::move(std::make_unique<LambertianShader>());
        break;
    case 'b':
        scene.shader = std::move(std::make_unique<PhongShader>());
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
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}