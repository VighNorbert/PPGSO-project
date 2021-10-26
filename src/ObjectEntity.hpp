#include <utility>

#ifndef PPGSO_OBJECTENTITY_HPP
#define PPGSO_OBJECTENTITY_HPP


class ObjectEntity {
private:
    std::vector<glm::vec3> vertices;

    std::vector<Face> indices;

    // Program to associate with the object
    ppgso::Shader program = {color_vert_glsl, color_frag_glsl};

    // These will hold the data and object buffers
    GLuint vao{}, vbo{}, cbo{}, ibo{};
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;

    void setup() {
        // Copy data to OpenGL
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Copy positions to gpu
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        // Set vertex program inputs
        auto position_attrib = program.getAttribLocation("Position");
        glEnableVertexAttribArray(position_attrib);
        glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Copy indices to gpu
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Face), indices.data(), GL_STATIC_DRAW);

        // Set projection matrices to identity
        program.setUniform("ProjectionMatrix", glm::perspective((ppgso::PI / 180.f) * 60.0f, 1.0f, 0.1f, 100.0f));

        program.setUniform("ViewMatrix", viewMatrix);
    }

public:
    // Public attributes that define position, color ..
    glm::vec3 position{0,0,0};
    glm::vec3 rotation{0,0,0};
    glm::vec3 scale{1,1,1};
    glm::vec3 color{1,0,0};


    // Initialize object data buffers
    ObjectEntity(std::vector<glm::vec3> vertices, std::vector<Face> indices) {
        this->vertices = std::move(vertices);
        this->indices = std::move(indices);
        setup();
    };

    explicit ObjectEntity(const ObjectType& objectType) {
        vertices = objectType.getVertices();
        indices = objectType.getIndices();
        setup();
    }



    // Clean up
    ~ObjectEntity() {
        // Delete data from OpenGL
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &cbo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    // Set the object transformation matrix
    void updateModelMatrix() {
        // Compute transformation by scaling, rotating and then translating the shape
        modelMatrix = glm::mat4{1.0f};
        modelMatrix = glm::scale(modelMatrix, scale);
        modelMatrix = glm::rotate(modelMatrix, rotation.x, {1, 0, 0});
        modelMatrix = glm::rotate(modelMatrix, rotation.y, {0, 1, 0});
        modelMatrix = glm::rotate(modelMatrix, rotation.z, {0, 0, 1});
        modelMatrix = glm::translate(modelMatrix, position);
    }

    void updateViewMatrix(glm::vec3 viewRotation, glm::vec3 viewTranslation) {
        // Compute transformation by scaling, rotating and then translating the shape
        viewMatrix = glm::mat4{1.0f};
        viewMatrix = glm::translate(viewMatrix, viewTranslation);
        viewMatrix = glm::rotate(viewMatrix, viewRotation.x, {1, 0, 0});
        viewMatrix = glm::rotate(viewMatrix, viewRotation.y, {0, 1, 0});
        viewMatrix = glm::rotate(viewMatrix, viewRotation.z, {0, 0, 1});
    }

    // Draw polygons
    void render(){
        // Update transformation and color uniforms in the shader
        program.use();
        program.setUniform("OverallColor", color);
        program.setUniform("ModelMatrix", modelMatrix);
        program.setUniform("ViewMatrix", viewMatrix);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, (GLsizei) indices.size() * 3, GL_UNSIGNED_INT, nullptr);
    };
};


#endif //PPGSO_OBJECTENTITY_HPP
