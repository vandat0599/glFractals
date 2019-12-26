#pragma once

#include "glad/glad.h"

#include <cstdint>
#include <string>
#include <vector>

namespace glFractals {
class Shader {
public:
    using Program = std::uint32_t;
    struct Source {
        enum class Type {
            VERTEX_SHADER = GL_VERTEX_SHADER,
            FRAGMENT_SHADER = GL_FRAGMENT_SHADER
        };
        Type type;
        std::string path;
    };

    Shader(const std::vector<Source>& sources);
    Shader(Shader&&);
    Shader& operator=(Shader&&);
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    virtual ~Shader();

    auto get() const -> Program;
    void use();

    // For error checking. Returns false if the uniform name is not found.
    template <typename T>
    auto setUniform(const std::string& name, const T& value)
    {
        auto uniform_location =
            glGetUniformLocation(program_.program, name.c_str());
        if (uniform_location < 0)
            return false;
        setUniform_impl(uniform_location, value);
        return true;
    }

private:
    using Object = std::uint32_t;

    // Helper openGL intermediate class for RAII
    struct ObjectWrapper {
        ObjectWrapper() = default;
        ObjectWrapper(const Shader::Source& source);
        ObjectWrapper(ObjectWrapper&&);
        ObjectWrapper& operator=(ObjectWrapper&&);
        ~ObjectWrapper();

        Object object = 0;
    };
    // Helper openGL intermediate class for RAII
    struct ProgramWrapper {
        ProgramWrapper() = default;
        ProgramWrapper(const std::vector<ObjectWrapper>& objects);
        ProgramWrapper(ProgramWrapper&&);
        ProgramWrapper& operator=(ProgramWrapper&&);
        ~ProgramWrapper();

        Program program = 0;
    };

    // These do the actual OpenGL calls.
    void setUniform_impl(const GLint location, const bool value);
    void setUniform_impl(const GLint location, const int value);
    void setUniform_impl(const GLint location, const float value);
    // Double requires #version 400.
    // void SetUniform_core(const GLint location, const double value) const;

    ProgramWrapper program_;
};

} // namespace glFractals