#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <windows.h>
#include <malloc.h>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"

struct shader_program_sources
{
    std::string vertex_source;
    std::string fragment_source;
};

static shader_program_sources parse_shader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class shader_type
    {
        none = -1,
        vertex = 0,
        fragment = 1
    };

    std::string line;
    std::stringstream string_streams[2];
    auto type = shader_type::none;
    
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = shader_type::vertex;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = shader_type::fragment;
            }
        }
        else if (type != shader_type::none && !line.empty())
        {
            string_streams[(int)type] << line << '\n';
        }
    }

    return { string_streams[0].str(), string_streams[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GL_CALL(glShaderSource(id, 1, &src, nullptr))

    GL_CALL(glCompileShader(id))

    int result;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result))

    if (result == GL_FALSE)
    {
		int length;

		GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length))
        __try
        {
            const auto message = (char*)_malloca(length * sizeof(char));
            GL_CALL(glGetShaderInfoLog(id, length, &length, message))
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
            std::cout << message << std::endl;
            GL_CALL(glDeleteShader(id))
            _freea(message);
        }
        __except ((GetExceptionCode() == STATUS_STACK_OVERFLOW) ?
            EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
        {
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
            std::cout << "Stack overflow" << std::endl;
        }
		
		return 0;
	}

    return id;
}

static unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader) 
{
    const unsigned int program_id = glCreateProgram();
    const unsigned int compile_vertex_shader_id = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    const unsigned int compile_fragment_shader_id = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

    GL_CALL(glAttachShader(program_id, compile_vertex_shader_id))
    GL_CALL(glAttachShader(program_id, compile_fragment_shader_id))
    GL_CALL(glLinkProgram(program_id))
    GL_CALL(glValidateProgram(program_id))

    GL_CALL(glDeleteShader(compile_vertex_shader_id))
    GL_CALL(glDeleteShader(compile_fragment_shader_id))

    return program_id;
}

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        constexpr float square_vertices[8] = {
            -0.5f, -0.5f, 
             0.5f, -0.5f, 
             0.5f,  0.5f,
            -0.5f,  0.5f,
        };

        const unsigned int square_indices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        unsigned int vertex_array_object;
        GL_CALL(glGenVertexArrays(1, &vertex_array_object))
        GL_CALL(glBindVertexArray(vertex_array_object))

        vertex_array va;
        vertex_buffer vb(square_vertices, 4 * 2 * sizeof(float));

        vertex_buffer_layout layout;
        layout.push<float>(2);
        va.add_buffer(vb, layout);

        index_buffer ib(square_indices, 6);

        const shader_program_sources shader_sources = parse_shader("res/shaders/Basic.shader");
        const unsigned int shader_program_id = create_shader(shader_sources.vertex_source, shader_sources.fragment_source);
        GL_CALL(glUseProgram(shader_program_id))

        GL_CALL(int location = glGetUniformLocation(shader_program_id, "u_Color"))
        ASSERT(location != -1)

        GL_CALL(glBindVertexArray(0))
        GL_CALL(glUseProgram(0))
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0))
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GL_CALL(glClear(GL_COLOR_BUFFER_BIT))

            GL_CALL(glUseProgram(shader_program_id))
            GL_CALL(glUniform4f(location, r, 0.2f, 0.2f, 1.0f))

            va.bind();
            ib.bind();

            GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr))
        
            if (r > 1.0f)
                increment = -0.10f;
            else if (r < 0.0f)
                increment = 0.10f;

            r += increment;

            /* Swap front and back buffers */
            GL_CALL(glfwSwapBuffers(window))

            /* Poll for and process events */
            GL_CALL(glfwPollEvents())
        }

        GL_CALL(glDeleteProgram(shader_program_id))
    }

    glfwTerminate();
    return 0;
}