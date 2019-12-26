#include "Shader.hpp"

#include "Utils.hpp"
#include "gl_utils.h"

#include "glad/glad.h"

#include <cassert>
#include <iostream>
#include <sstream>

namespace glFractals {

Shader::Shader(const std::vector<Shader::Source>& sources)
    : program_([&]() {
          std::vector<Shader::ObjectWrapper> objs;
          for (auto& src : sources) {
              objs.emplace_back(src);
          }
          return objs;
      }())
{
}

Shader::Shader(Shader&& other) : program_(std::move(other.program_)) {}

Shader& Shader::operator=(Shader&& other)
{
    program_ = std::move(other.program_);
    return *this;
}

Shader::~Shader() {}

void Shader::setUniform_impl(const GLint location, const bool value)
{
    GL(glUniform1i(location, (int)value));
}

void Shader::setUniform_impl(const GLint location, const int value)
{
    GL(glUniform1i(location, value));
}

void Shader::setUniform_impl(const GLint location, const float value)
{
    GL(glUniform1f(location, value));
}

Shader::Program Shader::get() const { return program_.program; }

void Shader::use() { GL(glUseProgram(program_.program)); }

Shader::ObjectWrapper::ObjectWrapper(const Shader::Source& source)
{
    auto shaderSrc = fileToString(source.path);

    GL(object = glCreateShader(static_cast<std::uint32_t>(source.type)));
    assert(object != 0);

    auto c_shaderSrc = shaderSrc.c_str();
    GL(glShaderSource(object, 1, &c_shaderSrc, nullptr));
    GL(glCompileShader(object));
    int success;
    GL(glGetShaderiv(object, GL_COMPILE_STATUS, &success));
    if (!success) {
        int len;
        GL(glGetShaderiv(object, GL_INFO_LOG_LENGTH, &len));
        auto info_log = std::string(len, '\0');
        GL(glGetShaderInfoLog(
            object, len, nullptr, const_cast<GLchar*>(info_log.c_str())));
        GL(glDeleteShader(object));
        std::stringstream ss;
        ss << __func__ << "\n"
           << "compiling error " << info_log << std::endl;
        throw std::runtime_error(ss.str());
    }
}

Shader::ObjectWrapper::ObjectWrapper(Shader::ObjectWrapper&& other)
{
    object = other.object;
    other.object = 0;
}

Shader::ObjectWrapper& Shader::ObjectWrapper::
operator=(Shader::ObjectWrapper&& other)
{
    GL(glDeleteShader(object));
    object = other.object;
    other.object = 0;
    return *this;
}

Shader::ObjectWrapper::~ObjectWrapper() { GL(glDeleteShader(object)); }

Shader::ProgramWrapper::ProgramWrapper(
    const std::vector<Shader::ObjectWrapper>& objects)
{
    program = glCreateProgram();
    for (auto& obj : objects) {
        GL(glAttachShader(program, obj.object));
    }
    GL(glLinkProgram(program));

    int success;
    GL(glGetProgramiv(program, GL_LINK_STATUS, &success));
    if (!success) {
        int len;
        GL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len));
        auto info_log = std::string(len, '\0');
        GL(glGetProgramInfoLog(
            program, len, nullptr, const_cast<GLchar*>(info_log.c_str())));
        for (auto& obj : objects) {
            GL(glDetachShader(program, obj.object));
        }
        GL(glDeleteProgram(program));
        std::stringstream ss;
        ss << __func__ << "\n"
           << "linking error " << info_log << std::endl;
        throw std::runtime_error(ss.str());
    }
    for (auto& obj : objects) {
        GL(glDetachShader(program, obj.object));
    }
}

Shader::ProgramWrapper::ProgramWrapper(Shader::ProgramWrapper&& other)
{
    program = other.program;
    other.program = 0;
}

Shader::ProgramWrapper& Shader::ProgramWrapper::
operator=(Shader::ProgramWrapper&& other)
{
    GL(glDeleteProgram(program));
    program = other.program;
    other.program = 0;
    return *this;
}

Shader::ProgramWrapper::~ProgramWrapper() { GL(glDeleteProgram(program)); }

} // namespace glFractals
