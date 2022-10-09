#include<fmt/core.h>
#include<memory>
#include"imgui.h"
#include"utils/handle.cpp"
#include"utils/scene.hpp"

struct Test
{
    void Show() {
        ImGui::Begin("test");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
};


int main() {
    fmt::print("start\n");

    auto test_ptr = std::make_unique<Scene>();
    handle(std::move(test_ptr));

    return 0;
}