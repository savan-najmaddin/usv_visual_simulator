#include "Object.hpp"
#include "threepp/threepp.hpp"

Object::Object() {
    createMesh();
}

void Object::createMesh() {
    auto geometry = threepp::BoxGeometry::create();
    auto material = threepp::MeshBasicMaterial::create();
    m_objectMesh = threepp::Mesh::create(geometry, material);
}
