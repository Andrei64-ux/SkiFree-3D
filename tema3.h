#pragma once

#include <string>
#include <unordered_map>
#include "components/simple_scene.h"
#include "components/transform.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        bool check_sphere_collision(glm::vec3 my_player, glm::vec3 obstacle);
        void RenderSimpleMeshLight(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));
        std::unordered_map<std::string, Texture2D*> mapTextures;
        GLuint randomTextureID;
        int morethanonetexture;
        float translateXTerrain, translateYTerrain, translateZTerrain;
        float translateXPlayer, translateYPlayer, translateZPlayer;
        float translateXTree = 0, translateYTree = 0, translateZTree = 0;
        float translateXLamp = 0, translateYLamp = 0, translateZLamp = 0;
        float translateXPresent = 0, translateYPresent = 0, translateZPresent = 0;
        float translateXStone = 0, translateYStone = 0, translateZStone = 0;
        float player_radius = 0.1;

        glm::vec3 lightPosition = glm::vec3(0, 1, 1);
        glm::vec3 lightDirection = glm::vec3(0, -1, 0);
        unsigned int materialShininess = 30;
        float materialKd = 0.5;
        float materialKs = 0.5;
        int is_spot_light = 1;
        float angle = 30;
        int numberObj = 50;

        int present_collected = 0;
        bool new_game = false;
        int print = 1;

        int terrain = 0;
        float time;
    };
}   // namespace m1
