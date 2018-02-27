//
//  Item.cpp
//  OpenGL-Cpp-Mac
//
//  Created by Barsehv Ivan on 05/12/2017.
//

#include "Item.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "helper.hpp"

Item::Item() {
    _rsx = _rsy = _rsz = 0.0f;
    _rx = _ry = _rz = 0.0f;
    _px = _py = _pz = 0.0f;
    _sx = _sy = _sz = _sc = 1.0f;
    t1 = t2 = t3 = -1; cf = 1.0f;
}

glm::mat4 Item::apply() {
    glm::mat4 res;
    res = glm::translate(res, glm::vec3(_px, _py, _pz));
    res = glm::scale(res, glm::vec3(_sx * _sc, _sy * _sc, _sz * _sc));
    res = glm::rotate(res, (float)degreesToRadians(_rx), glm::vec3(1.0f, 0.0f, 0.0f));
    res = glm::rotate(res, (float)degreesToRadians(_ry), glm::vec3(0.0f, 1.0f, 0.0f));
    res = glm::rotate(res, (float)degreesToRadians(_rz), glm::vec3(0.0f, 0.0f, 1.0f));
    return res;
}

Item* Item::place(float x, float y, float z) {
    _px = x; _py = y; _pz = z;
    return this;
}

Item* Item::rotate(float rx, float ry, float rz) {
    _rx = rx; _ry = ry; _rz = rz;
    return this;
}

Item* Item::rotateY(float ry) {
    _ry = ry;
    return this;
}

Item* Item::kick(float rsx, float rsy, float rsz) {
    _rsx = rsx; _rsy = rsy; _rsz = rsz;
    return this;
}

Item* Item::scale(float sx, float sy, float sz) {
    _sx = sx; _sy = sy; _sz = sz;
    return this;
}

Item* Item::scaleAll(float sc) {
    _sc = sc;
    return this;
}

void Item::processStep() {
    _rx += _rsx;
    _ry += _rsy;
    _rz += _rsz;
    
    if (_rx > 360) {
        _rx -= 360;
    } else if (_rx < 0) {
        _rx += 360;
    }
    if (_ry > 360) {
        _ry -= 360;
    } else if (_ry < 0) {
        _ry += 360;
    }
    if (_ry > 360) {
        _ry -= 360;
    } else if (_ry < 0) {
        _ry += 360;
    }
}

