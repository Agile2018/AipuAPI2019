#include "ShaderGL.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"

ShaderGL::ShaderGL(const std::string& filepath): rendererId(0)
{    
	ShaderProgramSource source = ParseShader(filepath);
	rendererId = CreateShader(source.VertexSource, source.FragmentSource);
}

ShaderGL::~ShaderGL()
{
	GLCall(glDeleteProgram(rendererId));
}

void ShaderGL::Delete() const
{
    GLCall(glDeleteProgram(rendererId));
}

void ShaderGL::Bind() const
{
	GLCall(glUseProgram(rendererId));
}

void ShaderGL::Unbind() const
{
	GLCall(glUseProgram(0));
}

void ShaderGL::SetUniform2f(const std::string& name, float v0, float v1)
{
    GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void ShaderGL::SetUniform1ui(const std::string& name, unsigned int value)
{
    GLCall(glUniform1ui(GetUniformLocation(name), value));
}

void ShaderGL::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void ShaderGL::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void ShaderGL::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void ShaderGL::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void ShaderGL::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

int ShaderGL::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(rendererId, name.c_str()));
	if (location == -1)
		std::cout << "Warning : uniform '" << name << "' doens't exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}

unsigned int ShaderGL::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

unsigned int ShaderGL::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

ShaderProgramSource ShaderGL::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {        
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;

        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    //std::cout << ss[0].str() << "  " << ss[1].str() << std::endl;
    return { ss[0].str(), ss[1].str() };
}
