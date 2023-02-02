#include "lab_m1/tema3/tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <assert.h>
using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


vector<glm::vec3> coord;
vector<glm::vec3> tree_coord;
vector<glm::vec3> present_coord;
vector<glm::vec3> lamp_coord;
vector<glm::vec3> stone_coord;

Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "textures");

    // Load textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "grass_bilboard.png").c_str(), GL_REPEAT);
        mapTextures["grass"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "lamp.jpg").c_str(), GL_REPEAT);
        mapTextures["lamp"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "christmas.jpg").c_str(), GL_REPEAT);
        mapTextures["christmas"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
        mapTextures["crate"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "earth.png").c_str(), GL_REPEAT);
        mapTextures["earth"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo", "bamboo.png").c_str(), GL_REPEAT);
        mapTextures["bamboo"] = texture;
    }

    {
        mapTextures["random"] = CreateRandomTexture(25, 25);
    }

    // Load meshes

    {
        Mesh* mesh = new Mesh("present");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("ski");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("player");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tree");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo"), "lowpolytree.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("StreetLamp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props"), "StreetLamp.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("Stone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "stone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a simple quad
    {
        vector<glm::vec3> vertices
        {
            glm::vec3(1.5f,  0.0f, -1.5f),    // top right
            glm::vec3(1.5f,  0.0f, 1.5f),    // bottom right
            glm::vec3(-1.5f, 0.0f, 1.5f),    // bottom left
            glm::vec3(-1.5f, 0.0f, -1.5f),    // top left
        };

        vector<glm::vec3> normals
        {
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0)
        };

        vector<glm::vec2> textureCoords
        {
            // TODO(student): Complete texture coordinates for the square
            //glm::vec2(0.0f, 0.0f)
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f)

        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        Mesh* mesh = new Mesh("square");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("LabShaderLight");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShaderLight.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShaderLight.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    translateXPlayer = 0.0f;
    translateYPlayer = 3.f;
    translateZPlayer = 0.0f;
}

bool Tema3::check_sphere_collision(glm::vec3 my_player, glm::vec3 obstacle) {
    float dx = my_player.x - obstacle.x;
    float dy = my_player.y - obstacle.y;
    float dz = my_player.z - obstacle.z;

    float distance = sqrt(dx * dx + dz * dz);

    if (distance <= 2 * player_radius) {
        return true;
    }
    return false;
}

inline glm::mat4 RotateOX(float radians)
{
    // TODO(student): Implement the rotation matrix
    return glm::transpose(
        glm::mat4(1, 0, 0, 0,
            0, cos(radians), -sin(radians), 0,
            0, sin(radians), cos(radians), 0,
            0, 0, 0, 1)
    );

}

inline glm::mat4 RotateOY(float radians)
{
    // TODO(student): Implement the rotation matrix
    return glm::transpose(
        glm::mat4(cos(radians), 0, sin(radians), 0,
            0, 1, 0, 0,
            -sin(radians), 0, cos(radians), 0,
            0, 0, 0, 1)
    );

}

void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

float RandomFloat(float min, float max)
{
    // this  function assumes max > min, you may want 
    // more robust error checking for a non-debug build
    float random = ((float)rand()) / (float)RAND_MAX;

    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    float range = max - min;
    return (random * range) + min;
}

void Tema3::Update(float deltaTimeSeconds)
{   
    translateXTerrain = translateXPlayer;
    translateYTerrain = translateYPlayer;
    translateZTerrain = translateZPlayer;

    translateYPlayer -= 0.1 * deltaTimeSeconds;
    translateZPlayer += 0.1 * deltaTimeSeconds;

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXPlayer, translateYPlayer - 1.6, translateZPlayer + 1.7));
        modelMatrix *= RotateOX(M_PI / 4);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.15f, 0.1f));
        RenderSimpleMeshLight(meshes["player"], shaders["LabShaderLight"], modelMatrix, glm::vec3(0.8, 0.5, 0.1));

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXPlayer - 0.075, translateYPlayer - 1.7, translateZPlayer + 1.7));
        modelMatrix *= RotateOX(M_PI / 4);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.01f, 0.3f));
        RenderSimpleMeshLight(meshes["ski"], shaders["LabShaderLight"], modelMatrix, glm::vec3(0.2, 0.1, 0.8));

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXPlayer + 0.075, translateYPlayer - 1.7, translateZPlayer + 1.7));
        modelMatrix *= RotateOX(M_PI / 4);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.01f, 0.3f));
        RenderSimpleMeshLight(meshes["ski"], shaders["LabShaderLight"], modelMatrix, glm::vec3(0.2, 0.1, 0.8));
    }

    terrain = 1;
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXTerrain, translateYTerrain, translateZTerrain));
        modelMatrix *= RotateOX(M_PI / 4);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(3.f));
        RenderSimpleMesh(meshes["square"], shaders["LabShader"], modelMatrix, mapTextures["snow"]);
    }
    terrain = 0;

    if (coord.size() != numberObj) {
        for (int i = 0; i < numberObj; i++) {
            float x = RandomFloat(translateXPlayer - 1.9, translateXPlayer + 1.9);
            float y = 1.5f;
            float z = RandomFloat(translateZPlayer, translateZPlayer + 2);
            coord.push_back(glm::vec3(x, y, z));
        }
    }

    for (int i = 0; i < coord.size(); i++) {
        //cout << "Desenez obiectul " << i << endl;
        //cout << "Desenez obiectul " << i << " cu coordonatele " << coord[i].x << " " << coord[i].y << " " << coord[i].z << endl;
        if (i % 4 == 0) {
            translateXTree = coord[i].x;
            translateYTree = coord[i].y + 0.75;
            translateZTree = coord[i].z;
            tree_coord.push_back(glm::vec3(translateXTree, translateYTree, translateZTree));

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= RotateOX(M_PI / 4);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXTree, translateYTree, translateZTree));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
            RenderSimpleMesh(meshes["tree"], shaders["LabShader"], modelMatrix, mapTextures["christmas"]); 
        }

        if (i % 4 == 1) {
            translateXPresent = coord[i].x + 0.6;
            translateYPresent = coord[i].y + 0.7;
            translateZPresent = coord[i].z - 0.2;
            present_coord.push_back(glm::vec3(translateXPresent, translateYPresent, translateZPresent));

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= RotateOX(M_PI / 4);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXPresent, translateYPresent, translateZPresent));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
            RenderSimpleMesh(meshes["present"], shaders["LabShader"], modelMatrix, mapTextures["random"]);
        }

        if (i % 4 == 2) {
            translateXLamp = coord[i].x - 0.6;
            translateYLamp = coord[i].y + 0.7;
            translateZLamp = coord[i].z - 0.9;
            lamp_coord.push_back(glm::vec3(translateXLamp, translateYLamp, translateZLamp));

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= RotateOX(M_PI / 4);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXLamp, translateYLamp, translateZLamp));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.03f));
            RenderSimpleMesh(meshes["StreetLamp"], shaders["LabShader"], modelMatrix, mapTextures["lamp"]);

            modelMatrix = glm::mat4(1);
            modelMatrix *= RotateOX(M_PI / 4);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXLamp, translateYLamp, translateZLamp));
            modelMatrix *= RotateOY(M_PI);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.03f));
            RenderSimpleMesh(meshes["StreetLamp"], shaders["LabShader"], modelMatrix, mapTextures["lamp"]);
        }

        if (i % 4 == 3) {
            translateXStone = coord[i].x + 2.5;
            translateYStone = coord[i].y + 0.6;
            translateZStone = coord[i].z - 0.9;
            stone_coord.push_back(glm::vec3(translateXStone, translateYStone, translateZStone));

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= RotateOX(M_PI / 4);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXStone, translateYStone, translateZStone));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.003f));
            RenderSimpleMesh(meshes["Stone"], shaders["LabShader"], modelMatrix, mapTextures["lamp"]);

            modelMatrix = glm::mat4(1);
            modelMatrix *= RotateOX(M_PI / 4);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXStone + 0.1, translateYStone, translateZStone));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.003f));
            RenderSimpleMesh(meshes["Stone"], shaders["LabShader"], modelMatrix, mapTextures["lamp"]);

            modelMatrix = glm::mat4(1);
            modelMatrix *= RotateOX(M_PI / 4);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXStone + 0.2, translateYStone, translateZStone));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.003f));
            RenderSimpleMesh(meshes["Stone"], shaders["LabShader"], modelMatrix, mapTextures["lamp"]);
        }
    }

    for (int i = 0; i < coord.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= RotateOX(M_PI / 4);
        modelMatrix = glm::translate(modelMatrix, coord[i]);
        glm::vec3 new_pos = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
        if (print == 1) {
            cout << coord[i].x << " " << coord[i].y << " " << coord[i].z << endl;
            cout << new_pos.x << " " << new_pos.y << " " << new_pos.z << endl;
        }
        if (i % 4 == 0) {
            if (check_sphere_collision(glm::vec3(translateXPlayer, translateYPlayer, translateZPlayer), new_pos)) {
                cout << "Coliziune copac" << endl;
                new_game = true;
                coord.erase(coord.begin() + i);
            }
        }

        if (i % 4 == 1) {
            if (check_sphere_collision(glm::vec3(translateXPlayer, translateYPlayer, translateZPlayer), new_pos)) {
                cout << "Coliziune cadou" << endl;
                coord.erase(coord.begin() + i);
                present_collected++;
            }
        }

        if (i % 4 == 2) {
            if (check_sphere_collision(glm::vec3(translateXPlayer, translateYPlayer, translateZPlayer), new_pos)) {
                cout << "Coliziune stalp" << endl;
                new_game = true;
                coord.erase(coord.begin() + i);
            }
        }

        if (i % 4 == 3) {
            if (check_sphere_collision(glm::vec3(translateXPlayer, translateYPlayer, translateZPlayer), new_pos)) {
                cout << "Coliziune piatra" << endl;
                new_game = true;
                coord.erase(coord.begin() + i);
            }
        }
    }
}


void Tema3::FrameEnd()
{
    DrawCoordinateSystem();
}

void Tema3::RenderSimpleMeshLight(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // TODO(student): Set any other shader uniforms that you need
    int is_spot_light_location = glGetUniformLocation(shader->program, "is_spot_light");
    glUniform1i(is_spot_light_location, is_spot_light);

    int angle_location = glGetUniformLocation(shader->program, "angle");
    glUniform1f(angle_location, angle);


    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    time = Engine::GetElapsedTime();
    glUniform1f(glGetUniformLocation(shader->program, "time"), time / 300);
    glUniform1i(glGetUniformLocation(shader->program, "terrain"), terrain);

    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    }

    if (texture2)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);

    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // TODO(student): Generate random texture data
    for (int i = 0; i < size; i++) {
        data[i] = rand() % 9999999;
    }

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &randomTextureID);
    glBindTexture(GL_TEXTURE_2D, randomTextureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(randomTextureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    //translateXPlayer = deltaX / 20;
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
