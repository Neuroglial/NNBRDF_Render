#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/platform/system/Window.hpp"
#include "core/platform/system/EventManager.hpp"

#include "core/platform/renderAPI/RenderAPI.hpp"

#include "core/render/Material.hpp"

#include "scene/ImageManager.hpp"
#include "scene/ShaderManager.hpp"
#include "scene/PipelineManager.hpp"

#include "scene/Camera.hpp"

struct ShaderBase
{
    alignas(4) float iTime;
    alignas(4) float iTimeDelta;
    alignas(4) int iFrame;
    alignas(4) float iFrameRate;
    alignas(16) glm::vec4 iMouse;
    alignas(16) glm::vec3 iResolution;
};

struct Light
{
    alignas(16) glm::vec3 light_pos;
    alignas(16) glm::vec3 light_color;
};

struct Lights
{
    int num;
    alignas(16) Light lg[10];
};

struct Camera_Uniform_Struct
{
    glm::vec3 view_pos;
    alignas(16) glm::mat4 projection;
    alignas(16) glm::mat4 view;
};

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main()
{
    EventManager event_mgr;
    Windows window;
    window.init();
    window.creat_window("NNBRDF_Render", SCR_WIDTH, SCR_HEIGHT, event_mgr);
    RenderAPI::init(GraphicsAPI::OpenGL);

    // 初始化Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // 初始化ImGui的GLFW和OpenGL3绑定
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    auto p_blackhole_p1 = RenderAPI::creator<Pipeline>::crt();
    p_blackhole_p1->attach_shader(ShaderManager::get("b_post_vs"));
    p_blackhole_p1->attach_shader(ShaderManager::get("b_blackhole_p1_fs"));

    auto p_blackhole_p2 = RenderAPI::creator<Pipeline>::crt();
    p_blackhole_p2->attach_shader(ShaderManager::get("b_post_vs"));
    p_blackhole_p2->attach_shader(ShaderManager::get("b_blackhole_p2_fs"));

    auto p_blackhole_p3 = RenderAPI::creator<Pipeline>::crt();
    p_blackhole_p3->attach_shader(ShaderManager::get("b_post_vs"));
    p_blackhole_p3->attach_shader(ShaderManager::get("b_blackhole_p3_fs"));

    auto p_blackhole_p4 = RenderAPI::creator<Pipeline>::crt();
    p_blackhole_p4->attach_shader(ShaderManager::get("b_post_vs"));
    p_blackhole_p4->attach_shader(ShaderManager::get("b_blackhole_p4_fs"));

     auto p_blackhole_p5 = RenderAPI::creator<Pipeline>::crt();
    p_blackhole_p5->attach_shader(ShaderManager::get("b_post_vs"));
    p_blackhole_p5->attach_shader(ShaderManager::get("b_blackhole_p5_fs"));

    auto p_through = RenderAPI::creator<Pipeline>::crt();
    p_through->attach_shader(ShaderManager::get("b_post_vs"));
    p_through->attach_shader(ShaderManager::get("b_through_fs"));

    Material mt_bk_p1(p_blackhole_p1);
    Material mt_bk_p2(p_blackhole_p2);
    Material mt_bk_p3(p_blackhole_p3);
    Material mt_bk_p4(p_blackhole_p4);
    Material mt_bk_p5(p_blackhole_p5);
    Material mt_thr(p_through);

    auto cube = RenderAPI::creator<Mesh>::crt();
    auto quad = RenderAPI::creator<Mesh>::crt();

    cube->as_base_shape(Mesh::Cube);
    quad->as_base_shape(Mesh::Quad);

    auto ub_base = RenderAPI::creator<UniformBuffer>::crt();
    ub_base->reset(48, 2);

    auto bufferA_img = RenderAPI::creator<Texture2D>::crt();
    bufferA_img->set_channels(Tex_Channels::RGB32F);
    bufferA_img->set_sample(Tex_WarppingMode::CLAMP, Tex_FilteringMode::Mipmap);

    auto bufferB_img = RenderAPI::creator<Texture2D>::crt();
    bufferB_img->set_channels(Tex_Channels::RGB32F);
    bufferB_img->set_sample(Tex_WarppingMode::CLAMP, Tex_FilteringMode::LINEAR);

    auto bufferC_img = RenderAPI::creator<Texture2D>::crt();
    bufferC_img->set_channels(Tex_Channels::RGB32F);
    bufferC_img->set_sample(Tex_WarppingMode::CLAMP, Tex_FilteringMode::LINEAR);

    auto bufferD_img = RenderAPI::creator<Texture2D>::crt();
    bufferD_img->set_channels(Tex_Channels::RGB32F);
    bufferD_img->set_sample(Tex_WarppingMode::CLAMP, Tex_FilteringMode::LINEAR);

    Ref<FrameBuffer> PassA;
    PassA = RenderAPI::creator<FrameBuffer>::crt();
    PassA->resize(SCR_WIDTH, SCR_HEIGHT);
    PassA->attach(bufferA_img, FrameBuffer::Color, 0);

    Ref<FrameBuffer> PassB;
    PassB = RenderAPI::creator<FrameBuffer>::crt();
    PassB->resize(SCR_WIDTH, SCR_HEIGHT);
    PassB->attach(bufferB_img, FrameBuffer::Color, 0);

    Ref<FrameBuffer> PassC;
    PassC = RenderAPI::creator<FrameBuffer>::crt();
    PassC->resize(SCR_WIDTH, SCR_HEIGHT);
    PassC->attach(bufferC_img, FrameBuffer::Color, 0);

    Ref<FrameBuffer> PassD;
    PassD = RenderAPI::creator<FrameBuffer>::crt();
    PassD->resize(SCR_WIDTH, SCR_HEIGHT);
    PassD->attach(bufferD_img, FrameBuffer::Color, 0);

    auto noise_img = RenderAPI::creator<Texture2D>::crt();
    noise_img->set_image(ImageManager::get(Root_Path + "source/image/RGBA_Noise.png"));
    noise_img->set_sample(Tex_WarppingMode::REPEAT, Tex_FilteringMode::Mipmap);
    auto org_img = RenderAPI::creator<Texture2D>::crt();
    org_img->set_image(ImageManager::get(Root_Path + "source/image/Organic_1.png"));
    org_img->set_sample(Tex_WarppingMode::REPEAT, Tex_FilteringMode::Mipmap);

    mt_bk_p1.set_param("iChannel0", &noise_img);
    mt_bk_p1.set_param("iChannel1", &org_img);

    mt_bk_p1.set_param("iChannel2", &bufferA_img);
    mt_bk_p2.set_param("iChannel0", &bufferA_img);
    mt_bk_p3.set_param("iChannel0", &bufferB_img);
    mt_bk_p4.set_param("iChannel0", &bufferC_img);

    mt_bk_p5.set_param("iChannel0", &bufferA_img);
    mt_bk_p5.set_param("iChannel1", &bufferB_img);
    mt_bk_p5.set_param("iChannel2", &bufferC_img);
    mt_bk_p5.set_param("iChannel3", &bufferD_img);

    mt_thr.set_param("iChannel0", &bufferA_img);

    ShaderBase base;
    base.iTime = 0;
    base.iTimeDelta = 0.02;
    base.iFrame = 0;
    base.iFrameRate = 50;
    base.iMouse = {0, 0, 0, 0};
    base.iResolution = {SCR_WIDTH, SCR_HEIGHT, 1};

    auto fun = [&](Event::Event &_event)
    {
        if (auto fr = dynamic_cast<Event::Event_Frame_Resize *>(&_event))
        {
            glViewport(0, 0, fr->m_width, fr->m_height);
            if (PassA != nullptr)
                PassA->resize(fr->m_width, fr->m_height);
            if (PassB != nullptr)
                PassB->resize(fr->m_width, fr->m_height);
            if (PassC != nullptr)
                PassC->resize(fr->m_width, fr->m_height);
            if (PassD != nullptr)
                PassD->resize(fr->m_width, fr->m_height);

            base.iResolution = glm::vec3(fr->m_width, fr->m_height, 1);
        }
        else if (auto kb = dynamic_cast<Event::Event_Keyboard *>(&_event))
        {
            if (kb->m_code == KeyCode::MouseLeft)
            {
                if (kb->m_type == PressType::Press)
                    base.iMouse = glm::vec4(base.iMouse.x, base.iMouse.y, 2, base.iMouse.w);
                else
                    base.iMouse = glm::vec4(base.iMouse.x, base.iMouse.y, 0, base.iMouse.w);
            }
            else if (kb->m_code == KeyCode::MouseRight)
            {
                if (kb->m_type == PressType::Press)
                    base.iMouse = glm::vec4(base.iMouse.x, base.iMouse.y, base.iMouse.z, 2);
                else
                    base.iMouse = glm::vec4(base.iMouse.x, base.iMouse.y, base.iMouse.z, 0);
            }
        }
        else if (auto mm = dynamic_cast<Event::Event_Mouse_Move *>(&_event))
        {
            base.iMouse = glm::vec4(
                base.iMouse.z || base.iMouse.w ? mm->m_xpos : base.iMouse.x,
                base.iMouse.z || base.iMouse.w ? mm->m_ypos : base.iMouse.y,
                base.iMouse.z, base.iMouse.w);
        }
    };

    event_mgr.registerCallback(fun);

    glDisable(GL_DEPTH_TEST);

    while (!window.shouldClose())
    {
        /*
        // imgui
        {
            // 开始新的ImGui帧
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            static bool win = true;
            if (ImGui::Begin("Controller", &win))
            {

                ImGui::Bullet();
                ImGui::Text("Light:");
                ImGui::BeginChild(106, ImVec2(20, 94), false);
                ImGui::EndChild();
                ImGui::SameLine();
                ImGui::BeginChild(108, ImVec2(205, 150), false);

                static glm::vec3 color(1.0f);
                ImGui::Text("Color:");
                ImGui::PushItemWidth(200);
                ImGui::ColorEdit3("##1", &color.x);
                ImGui::PopItemWidth();

                ImGui::EndChild();
            }
            ImGui::End();
            ImGui::Render();
        }
        */

        base.iTime += 0.02;
        ++base.iFrame;
        ub_base->set_data(0, 44, &base.iTime);

        PassA->bind();
        quad->draw(mt_bk_p1);
        PassA->unbind();
        bufferA_img->gen_mipmap();

        PassB->bind();
        quad->draw(mt_bk_p2);
        PassB->unbind();

        PassC->bind();
        quad->draw(mt_bk_p3);
        PassC->unbind();

        PassD->bind();
        quad->draw(mt_bk_p4);
        PassD->unbind();

        quad->draw(mt_bk_p5);
        // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.swapBuffer();
    }

    return 0;
}