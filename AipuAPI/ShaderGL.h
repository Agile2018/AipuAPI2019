#ifndef SHADERGL_H
#define SHADERGL_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class ShaderGL
{
public:
	ShaderGL(const std::string& filepath);
	~ShaderGL();
	void Bind() const;
	void Unbind() const;
	void Delete() const;
	// Set uniforms
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform1ui(const std::string& name, unsigned int value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	unsigned int rendererId;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	//caching for uniforms
	
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name) const;
	ShaderProgramSource ParseShader(const std::string& filepath);
};


#endif // !SHADERGL_H

