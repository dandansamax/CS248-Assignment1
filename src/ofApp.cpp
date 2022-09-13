#include "ofApp.h"
#include "GeoObject.h"
#include "Scene.h"
#include "Shader.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    w = 640;
    h = 480;
    colorPixels.allocate(w, h, OF_PIXELS_RGB);

    scene.ca = std::move(std::make_unique<Camera>(Vector3f(0, 0, -1), Vector3f(0, 0, 1), w, h, 2.0f, 1.5f));
    scene.pixels = &colorPixels;
    scene.shader = std::move(std::make_unique<NaiveShader>());

    scene.lights.push_back(std::make_shared<Light>(Vector3f(0, 5, 0), Vector3f(1, 1, 1)));
    scene.objects.push_back(std::make_shared<Sphere>(Vector3f(0, 0, 0), 0.5, Vector3f(0, 0, 1)));
    scene.objects.push_back(std::make_shared<Sphere>(Vector3f(0, 0, 2), 1, Vector3f(0, 1, 0)));

    update();
}

//--------------------------------------------------------------
void ofApp::update()
{
    scene.render();
    texColor.allocate(colorPixels);
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
    if (key == 'p')
    {
        if (scene.ca->perspective)
        {
            scene.ca->setParallel();
        }
        else
        {
            scene.ca->setPerspective(1.0f);
        }
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