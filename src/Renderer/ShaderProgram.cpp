#include "ShaderProgram.h"
#include <iostream>

namespace Renderer {
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
		GLuint vsid;
		if (!CreateShader(vertexShader, GL_VERTEX_SHADER, vsid)) {
			std::cerr << "VERTEX::SHADER: compile-time-error" << std::endl;
			return;
		}

		GLuint fsid;
		if (!CreateShader(fragmentShader, GL_FRAGMENT_SHADER, fsid)) {
			std::cerr << "FRAGMENT::SHADER: compile-time-error" << std::endl;
			glDeleteShader(vsid);
			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vsid);
		glAttachShader(m_ID, fsid);
		glLinkProgram(m_ID);
		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar log[1024];
			glGetShaderInfoLog(m_ID, 1024, nullptr, log);
			std::cerr << "ERROR::SHADER: Link-time-error:\n" << log;
		}
		else {
			m_isCompiled = true;
		}

		glDeleteShader(vsid);
		glDeleteShader(fsid);
	}

	bool ShaderProgram::CreateShader(const std::string& source, const GLenum shaderType, GLuint& shaderID) {
		shaderID = glCreateShader(shaderType);
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar log[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, log);
			std::cerr << "ERROR::SHADER: Compile-time-error:\n" << log;
			return false;
		}
		return true;
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_ID);
	}

	void ShaderProgram::use() const {
		glUseProgram(m_ID);
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept {
		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;

		return *this;
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept {
		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
	}
}
