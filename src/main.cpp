#include "Framer/Framer.h"
#include <array>
#include <vector>
#include <Framer/ComputeShader.h>

constexpr int width = 800;
constexpr int height = 800;

constexpr glm::vec4 RED = {1.0f, 0.0f, 0.0f, 1.0f};
constexpr glm::vec4 BLUE = {0.0f, 0.0f, 1.0f, 1.0f};

int main() {
    std::vector<glm::vec4> image{width * height, glm::vec4(0)};



    std::array<float, 8> vertices = {
        -1.0f, 1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
    };


    fr::Window win{width, height, "Super Sand"};
    glViewport(0, 0, width, height);
    win.SetVSync(false);


    fr::Shader shader{RESOURCES_PATH"/shaders/basic.vert", RESOURCES_PATH"/shaders/basic.frag"};
    fr::VBO VBO{vertices.data(), vertices.size() * sizeof(float)};
    fr::VAO VAO{};
    VAO.SpecifyData(0, 2, 0);
    VAO.BindVertexBuffer(VBO, 0, 0, 2 * sizeof(float));

    GLuint tex0;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex0);
    glTextureStorage2D(tex0, 1, GL_RGBA32F, 2 * width, height);
    glTextureParameteri(tex0, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(tex0, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(tex0, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(tex0, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureSubImage2D(tex0, 0, 0, 0,  width, height, GL_RGBA, GL_FLOAT, image.data()); // TODO: Add predefined texture
    glBindImageTexture(0, tex0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);


    fr::ComputeShader compute_shader{RESOURCES_PATH"/shaders/test.comp"};



    while (win.IsRunning()) {
        // Use the compute shader program
        glBindTextureUnit(0, tex0);
        glUseProgram(compute_shader.ID());

        bool mousePressed = glfwGetMouseButton(win.Ptr(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        glm::ivec2 mousePos = {static_cast<int>(win.MouseX()), static_cast<int>(win.MouseY())};

        glUniform1i(glGetUniformLocation(compute_shader.ID(), "mousePressed"), mousePressed);
        glUniform2i(glGetUniformLocation(compute_shader.ID(), "mousePos"),  mousePos.x, static_cast<int>(win.Height()) - mousePos.y);
        glUniform2i(glGetUniformLocation(compute_shader.ID(), "winSize"), static_cast<int>(win.Width()), static_cast<int>(win.Height()));

        // Dispatch the compute shader
        glDispatchCompute(width, height, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


        shader.Bind();
        VAO.Bind();
        glBindTextureUnit(0, tex0);
        shader.uni1i("imgTexture", 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //std::swap(texture1, texture2);

        win.ShowFPS();
        win.OnUpdate();
    }

    glDeleteTextures(1, &tex0);
}