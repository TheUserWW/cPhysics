#include "graphics/OpenGL/gl_model.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

static const char* modelVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
}
)";

static const char* modelFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    
    float diff = max(dot(norm, lightDir), 0.0);
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    vec3 specular = 0.5 * spec * vec3(1.0, 1.0, 1.0);
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";

static GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Model shader compilation error: %s\n", infoLog);
        return 0;
    }
    return shader;
}

static GLuint createModelShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, modelVertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, modelFragmentShaderSource);
    
    if (!vertexShader || !fragmentShader) {
        return 0;
    }
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("Model shader program linking error: %s\n", infoLog);
        return 0;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

Model::Model(const std::string& filepath) : filepath(filepath), shader_program(0) {}

Model::~Model() {
    cleanup();
}

bool Model::initShader() {
    shader_program = createModelShaderProgram();
    return shader_program != 0;
}

bool Model::loadModel() {
    if (!initShader()) {
        std::cout << "Failed to initialize model shader" << std::endl;
        return false;
    }
    
    std::cout << "Warning: Model loading from file is not supported" << std::endl;
    return false;
}

void Model::processNode(void* nodePtr, const void* scenePtr) {
    std::cout << "Warning: Model processing is not supported" << std::endl;
}

ModelMesh Model::processMesh(void* meshPtr, const void* scenePtr) {
    ModelMesh result;
    return result;
}

void Model::setupMesh(ModelMesh& mesh) {
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);
    
    glBindVertexArray(mesh.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(ModelVertex), 
                 &mesh.vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int),
                 &mesh.indices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 
                         (void*)(3 * sizeof(float)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex),
                         (void*)(6 * sizeof(float)));
    
    glBindVertexArray(0);
}

void Model::render(const float* view, const float* projection, 
                   const float* modelMatrix, const float* color) {
    if (!shader_program || meshes.empty()) return;
    
    glUseProgram(shader_program);
    
    float defaultModel[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    float defaultColor[3] = {1.0f, 0.8f, 0.2f};
    
    const float* model = modelMatrix ? modelMatrix : defaultModel;
    const float* objColor = color ? color : defaultColor;
    
    GLuint modelLoc = glGetUniformLocation(shader_program, "model");
    GLuint viewLoc = glGetUniformLocation(shader_program, "view");
    GLuint projLoc = glGetUniformLocation(shader_program, "projection");
    GLuint colorLoc = glGetUniformLocation(shader_program, "objectColor");
    GLuint lightPosLoc = glGetUniformLocation(shader_program, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(shader_program, "viewPos");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
    glUniform3fv(colorLoc, 1, objColor);
    glUniform3f(lightPosLoc, 50.0f, 50.0f, 50.0f);
    glUniform3f(viewPosLoc, 0.0f, 0.0f, 20.0f);
    
    for (ModelMesh& mesh : meshes) {
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void Model::cleanup() {
    for (ModelMesh& mesh : meshes) {
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
    }
    meshes.clear();
    
    if (shader_program) {
        glDeleteProgram(shader_program);
        shader_program = 0;
    }
}
