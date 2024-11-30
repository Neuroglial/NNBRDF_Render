#include "core/platform/renderAPI/Shader.hpp"
#include "scene/ShaderManager.hpp"


#include <glad/glad.h>
#include <glm/glm.hpp>

void shader_error_check(unsigned int shader, Shader::Shader_Type type);
void pipeline_error_check(unsigned int pipeline);

namespace Shader
{

    class Shader_GL : public Shader
    {
    public:
        Shader_GL(const std::string &path, Shader_Type type) : Shader(path, type), m_compiled(false), m_id(0) {}

        void read_file();

        void compiled();

        virtual ~Shader_GL()
        {
            if (m_compiled)
                glDeleteShader(m_id);
        }

        unsigned int get_id();

        bool m_compiled;

    private:
        std::string m_code;
        unsigned int m_id;
    };

    class Pipline_GL : public Pipline
    {
    public:
        Pipline_GL() : m_id(0)
        {
        }

        virtual void bind() override;

        virtual void set_params(ParamListBase& params) override;

    private:
        unsigned int m_id;
    };

}

#define DECLARE_SHADER(TypeName)                                                            \
    TypeName (const std::string &path, Shader_Type type):Shader_GL(path,type){              \
        ShaderManager::instance.register_shader(m_path,&instance);                          \
    }                                                                                       \
    Parameters pms;                                                                         \
    static TypeName instance;                                                               \
    virtual ParamListBase& get_params() override {return pms;}                              \
    

void shader_error_check(unsigned int shader, Shader::Shader_Type type);

void pipeline_error_check(unsigned int pipeline);