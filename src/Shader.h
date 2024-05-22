#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

// Function to read a text file and return its contents as a string
std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw(errno);
}

class Shader
{
public:
    // Reference ID of the Shader Program
    GLuint ID;

    // Constructor that builds the Shader Program from two different shaders
    Shader(const char* vertexFile, const char* fragmentFile)
    {
        // Read vertexFile and fragmentFile and store the strings
        std::string vertexCode = get_file_contents(vertexFile);
        std::string fragmentCode = get_file_contents(fragmentFile);

        // Convert the shader source strings into character arrays
        const char* vertexSource = vertexCode.c_str();
        const char* fragmentSource = fragmentCode.c_str();

        // Create Vertex Shader Object and get its reference
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // Attach Vertex Shader source to the Vertex Shader Object
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        // Compile the Vertex Shader into machine code
        glCompileShader(vertexShader);
        // Check if Shader compiled successfully
        compileErrors(vertexShader, "VERTEX");

        // Create Fragment Shader Object and get its reference
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // Attach Fragment Shader source to the Fragment Shader Object
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        // Compile the Fragment Shader into machine code
        glCompileShader(fragmentShader);
        // Check if Shader compiled successfully
        compileErrors(fragmentShader, "FRAGMENT");

        // Create Shader Program Object and get its reference
        ID = glCreateProgram();
        // Attach the Vertex and Fragment Shaders to the Shader Program
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        // Wrap-up/Link all the shaders together into the Shader Program
        glLinkProgram(ID);
        // Check if Shaders linked successfully
        compileErrors(ID, "PROGRAM");

        // Delete the now useless Vertex and Fragment Shader objects
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // Activates the Shader Program
    void Activate()
    {
        glUseProgram(ID);
    }

    // Deletes the Shader Program
    void Delete()
    {
        glDeleteProgram(ID);
    }

private:
    // Checks if the different Shaders have compiled properly
    void compileErrors(unsigned int shader, const char* type)
    {
        // Stores status of compilation
        GLint hasCompiled;
        // Character array to store error message in
        char infoLog[1024];
        if (type != std::string("PROGRAM"))
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if (hasCompiled == GL_FALSE)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << infoLog << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
            if (hasCompiled == GL_FALSE)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << infoLog << std::endl;
            }
        }
    }
};

#endif
