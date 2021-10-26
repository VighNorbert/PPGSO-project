
#ifndef PPGSO_OBJECTTYPE_HPP
#define PPGSO_OBJECTTYPE_HPP


#include <utility>
#include <vector>

struct Face {
    GLuint v0, v1, v2;
};

class ObjectType {
private:
    std::vector<glm::vec3> vertices;

    std::vector<Face> indices;

public:

    ObjectType(std::vector<glm::vec3> vertices, std::vector<Face> indices) {
        this->vertices = std::move(vertices);
        this->indices = std::move(indices);
    }

    const std::vector<glm::vec3> &getVertices() const {
        return vertices;
    }

    const std::vector<Face> &getIndices() const {
        return indices;
    }
};


#endif //PPGSO_OBJECTTYPE_HPP
