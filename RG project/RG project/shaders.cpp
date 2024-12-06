#include "Shaders.h"

GLuint compileShader(const char* shaderSource, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader Compilation Failed\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint loadShaders(const char* vertex_file_path, const char* fragment_file_path) {
    std::ifstream vertexFile(vertex_file_path);
    std::ifstream fragmentFile(fragment_file_path);

    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    std::string vertexCode = vertexStream.str();
    std::string fragmentCode = fragmentStream.str();

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader Program Linking Failed\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
