#ifndef GL_MODEL_H
#define GL_MODEL_H

#include <vector>
#include <string>
#include <GL/glew.h>

struct ModelVertex {
    float position[3];
    float normal[3];
    float texCoord[2];
};

struct ModelMesh {
    std::vector<ModelVertex> vertices;
    std::vector<unsigned int> indices;
    GLuint VAO, VBO, EBO;
};

class Model {
public:
    Model(const std::string& filepath);
    ~Model();
    
    bool loadModel();
    void render(const float* view, const float* projection, 
                const float* modelMatrix = nullptr,
                const float* color = nullptr);
    void cleanup();
    
    GLuint getShaderProgram() const { return shader_program; }
    
private:
    std::string filepath;
    std::vector<ModelMesh> meshes;
    GLuint shader_program;
    
    bool initShader();
    void processNode(void* node, const void* scene);
    ModelMesh processMesh(void* mesh, const void* scene);
    void setupMesh(ModelMesh& mesh);
};

#endif
