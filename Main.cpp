#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "PBRMaterial.h"
#include "Model.h"

// Ka�da cz�� pokoju jako osobny element - zmniejszony rozmiar dla lepszych proporcji z lampami
GLfloat floorVertices[] = {
    // posX, posY, posZ,  u, v,      nx, ny, nz,       tx, ty, tz,        bx, by, bz
    -15, -7.5, -30,  0.0f, 0.0f,     0, 1, 0,           1, 0, 0,           0, 0, 1,
     15, -7.5, -30, 10.0f, 0.0f,     0, 1, 0,           1, 0, 0,           0, 0, 1,
     15, -7.5,  30, 10.0f,10.0f,     0, 1, 0,           1, 0, 0,           0, 0, 1,
    -15, -7.5,  30,  0.0f,10.0f,     0, 1, 0,           1, 0, 0,           0, 0, 1,
};

GLfloat ceilingVertices[] = {
    -15, 7.5, -30, 0.0f, 0.0f,     0, -1, 0,     1, 0, 0,     0, 0, -1,
     15, 7.5, -30, 3.0f, 0.0f,     0, -1, 0,     1, 0, 0,     0, 0, -1,
     15, 7.5,  30, 3.0f, 6.0f,     0, -1, 0,     1, 0, 0,     0, 0, -1,
    -15, 7.5,  30, 0.0f, 6.0f,     0, -1, 0,     1, 0, 0,     0, 0, -1,
};

GLfloat wallVertices[] = {
    // back wall (z = -30)
    -15, -7.5, -30, 0.0f, 0.0f,     0, 0, 1,     1, 0, 0,     0, 1, 0,
     15, -7.5, -30, 6.0f, 0.0f,     0, 0, 1,     1, 0, 0,     0, 1, 0,
     15,  7.5, -30, 6.0f, 3.0f,     0, 0, 1,     1, 0, 0,     0, 1, 0,
    -15,  7.5, -30, 0.0f, 3.0f,     0, 0, 1,     1, 0, 0,     0, 1, 0,

    // front wall (z = +30)
    -15, -7.5, 30, 0.0f, 0.0f,      0, 0, -1,    1, 0, 0,     0, 1, 0,
     15, -7.5, 30, 6.0f, 0.0f,      0, 0, -1,    1, 0, 0,     0, 1, 0,
     15,  7.5, 30, 6.0f, 3.0f,      0, 0, -1,    1, 0, 0,     0, 1, 0,
    -15,  7.5, 30, 0.0f, 3.0f,      0, 0, -1,    1, 0, 0,     0, 1, 0,

    // left wall (x = -15)
    -15, -7.5, -30, 0.0f, 0.0f,     1, 0, 0,     0, 0, 1,     0, 1, 0,
    -15, -7.5,  30, 12.0f, 0.0f,    1, 0, 0,     0, 0, 1,     0, 1, 0,    -15,  7.5,  30, 12.0f, 3.0f,    1, 0, 0,     0, 0, 1,     0, 1, 0,
    -15,  7.5, -30, 0.0f, 3.0f,     1, 0, 0,     0, 0, 1,     0, 1, 0,

    // right wall (x = +15)
     15, -7.5, -30, 0.0f, 0.0f,    -1, 0, 0,     0, 0, -1,    0, 1, 0,
     15, -7.5,  30, 12.0f, 0.0f,   -1, 0, 0,     0, 0, -1,    0, 1, 0,
     15,  7.5,  30, 12.0f, 3.0f,   -1, 0, 0,     0, 0, -1,    0, 1, 0,
     15,  7.5, -30, 0.0f, 3.0f,    -1, 0, 0,     0, 0, -1,    0, 1, 0,

};

GLuint quadIndices[] = { 0, 1, 2, 2, 3, 0 };
GLuint wallIndices[] = {
    0, 1, 2, 2, 3, 0,      // back
    4, 5, 6, 6, 7, 4,      // front
    8, 9,10,10,11, 8,      // left
   12,13,14,14,15,12       // right
};

// Pozycje lamp na suficie (będą też pozycjami świateł)
glm::vec3 lampPositions[] = {
    {-10.0f, 2.8f, -25.0f},   // Lampy zwisają z sufitu na wysokości 2.8f
    {-10.0f, 2.8f, -12.5f},
    {-10.0f, 2.8f,   0.0f},
    {-10.0f, 2.8f,  12.5f},
    {-10.0f, 2.8f,  25.0f},

    {0.0f, 2.8f, -25.0f},
    {0.0f, 2.8f, -12.5f},
    {0.0f, 2.8f,   0.0f},
    {0.0f, 2.8f,  12.5f},
    {0.0f, 2.8f,  25.0f},

    {10.0f, 2.8f, -25.0f},
    {10.0f, 2.8f, -12.5f},
    {10.0f, 2.8f,   0.0f},
    {10.0f, 2.8f,  12.5f},
    {10.0f, 2.8f,  25.0f},
};

unsigned int loadTexture(const char* path) {
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int w, h, c;
    unsigned char* data = stbi_load(path, &w, &h, &c, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
    return tex;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 800, "Room", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glEnable(GL_DEPTH_TEST);

    Shader shader("default.vert", "default.frag");
    
    // Sprawdź, czy shader został poprawnie utworzony
    GLint success;
    glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shader.ID, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader linking failed\n" << infoLog << std::endl;
        return -1;
    }
    
    Camera camera(800, 800, glm::vec3(0.0f, 0.0f, 20.0f)); // Dostosowana pozycja kamery do nowego rozmiaru pokoju

    // Floor
    VAO vaoFloor; 
    vaoFloor.Bind();
    VBO vboFloor(floorVertices, sizeof(floorVertices));
    EBO eboFloor(quadIndices, sizeof(quadIndices));
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);                      // aPos
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));     // aTex
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(5 * sizeof(float)));     // aNormal
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));     // aTangent
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));    // aBitangent
    glEnableVertexAttribArray(4);
    vaoFloor.Unbind();


    // Ceiling
    VAO vaoCeil; vaoCeil.Bind();
    VBO vboCeil(ceilingVertices, sizeof(ceilingVertices));
    EBO eboCeil(quadIndices, sizeof(quadIndices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);                      // aPos
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));     // aTex
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(5 * sizeof(float)));     // aNormal
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));     // aTangent
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));    // aBitangent
    glEnableVertexAttribArray(4);
    vaoCeil.Unbind();

    // Walls
    VAO vaoWalls; vaoWalls.Bind();
    VBO vboWalls(wallVertices, sizeof(wallVertices));
    EBO eboWalls(wallIndices, sizeof(wallIndices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);                      // aPos
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));     // aTex
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(5 * sizeof(float)));     // aNormal
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));     // aTangent
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));    // aBitangent
    glEnableVertexAttribArray(4);
    vaoWalls.Unbind();

    // Tekstury
    PBRMaterial floorMat = loadPBRMaterial("res/texture/floor/", "wood");
    std::cout << "FLOOR albedo: " << floorMat.albedo
        << " normal: " << floorMat.normal
        << " rough: " << floorMat.roughness << std::endl;
        
    if (floorMat.albedo == 0 || floorMat.normal == 0 || floorMat.roughness == 0) {
        std::cerr << "ERROR: Failed to load floor textures!" << std::endl;
    }

	PBRMaterial ceilingMat = loadPBRMaterial("res/texture/ceiling/", "wall");
	std::cout << "CEILING albedo: " << ceilingMat.albedo
		<< " normal: " << ceilingMat.normal
		<< " rough: " << ceilingMat.roughness << std::endl;
		
    if (ceilingMat.albedo == 0 || ceilingMat.normal == 0 || ceilingMat.roughness == 0) {
        std::cerr << "ERROR: Failed to load ceiling textures!" << std::endl;
    }
    
	PBRMaterial wallMat = loadPBRMaterial("res/texture/wall/", "plastered_wall");
	std::cout << "WALL albedo: " << wallMat.albedo
		<< " normal: " << wallMat.normal
		<< " rough: " << wallMat.roughness << std::endl;
		
    if (wallMat.albedo == 0 || wallMat.normal == 0 || wallMat.roughness == 0) {
        std::cerr << "ERROR: Failed to load wall textures!" << std::endl;
    }    // Sprawdź błędy OpenGL po załadowaniu tekstur
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "GL ERROR after texture loading: " << err << std::endl;
    }    // Załaduj model lampy
    Model* lampModel = nullptr;
    try {
        lampModel = new Model("res/models/modern_ceiling_lamp_01.obj");
        std::cout << "Lamp model loaded successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load lamp model: " << e.what() << std::endl;
        // Kontynuuj bez modelu lampy
    }

    // Załaduj tekstury dla lamp
    PBRMaterial lampMat = loadPBRMaterial("res/texture/other/", "modern_ceiling_lamp_01");
    std::cout << "LAMP albedo: " << lampMat.albedo
        << " normal: " << lampMat.normal
        << " rough: " << lampMat.roughness << std::endl;
        
    if (lampMat.albedo == 0 || lampMat.normal == 0 || lampMat.roughness == 0) {
        std::cerr << "ERROR: Failed to load lamp textures!" << std::endl;
    }

    Model* frameModel = nullptr;
    Model* canvasModel = nullptr;
    Model* maxwellModel = nullptr;
    try {
        frameModel = new Model("res/models/picture_frame.obj");
        canvasModel = new Model("res/models/picture_frame_canvas.obj");
        maxwellModel = new Model("res/models/maxwell.obj");
        std::cout << "All models loaded successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load one or more models: " << e.what() << std::endl;
    }

    PBRMaterial frameMat;
    frameMat.albedo = loadTexturePBR("res/texture/other/fancy_picture_frame_01_diff_2k.jpg");
    frameMat.normal = loadTexturePBR("res/texture/other/fancy_picture_frame_01_nor_gl_2k.jpg");
    frameMat.roughness = loadTexturePBR("res/texture/other/fancy_picture_frame_01_rough_2k.jpg");

    PBRMaterial canvasMat;
    canvasMat.albedo = loadTexturePBR("res/texture/other/fancy_picture_frame_01_canvas_diff_2k.png", GL_RGB);
    canvasMat.normal = loadTexturePBR("res/texture/other/fancy_picture_frame_01_canvas_nor_gl_2k.png");
    canvasMat.roughness = loadTexturePBR("res/texture/other/fancy_picture_frame_01_canvas_rough_2k.png");

    GLuint maxwellTex = loadTexturePBR("res/texture/other/maxwell_basecolor.jpeg");

    bool sepiaEnabled = false;
    while (!glfwWindowShouldClose(window)) {
        static float maxwellAngle = 0.0f;
        maxwellAngle += 0.01f;
        if (maxwellAngle > 360.0f) maxwellAngle -= 360.0f;
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            sepiaEnabled = true;
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
            sepiaEnabled = false;
        
        glUniform1i(glGetUniformLocation(shader.ID, "useSepia"), sepiaEnabled);

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shader, "camMatrix");
        shader.Activate();
          // Pozycje świateł (teraz są w środku żarówek lamp)
        for (int i = 0; i < 15; ++i) {
            std::string name = "lightPositions[" + std::to_string(i) + "]";
            // Światło umieszczone w środku żarówki lampy (nieco niżej niż sama lampa dla lepszego efektu)
            glm::vec3 lightPos = lampPositions[i];
            lightPos.y -= 1.0f; // Przesunięcie światła nieco w dół do środka żarówki
            glUniform3fv(glGetUniformLocation(shader.ID, name.c_str()), 1, glm::value_ptr(lightPos));
        }
        // Pozycja kamery (dynamiczna)
        glUniform3fv(glGetUniformLocation(shader.ID, "viewPos"), 1, glm::value_ptr(camera.Position));

        glUniform1i(glGetUniformLocation(shader.ID, "albedoMap"), 0);
        glUniform1i(glGetUniformLocation(shader.ID, "normalMap"), 1);
        glUniform1i(glGetUniformLocation(shader.ID, "roughnessMap"), 2);
        
        // Ustaw macierz model (jednostkowa dla podstawowych obiektów)
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        
        // Floor
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, floorMat.albedo);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, floorMat.normal);
        glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, floorMat.roughness);
        vaoFloor.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Ceiling
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, ceilingMat.albedo);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, ceilingMat.normal);
        glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, ceilingMat.roughness);
        vaoCeil.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);        // Walls
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, wallMat.albedo);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, wallMat.normal);
        glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, wallMat.roughness);
        vaoWalls.Bind();
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);        // Renderuj lampy zwisające z sufitu
        if (lampModel) {
            // Ustaw tekstury dla lampy
            glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, lampMat.albedo);
            glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, lampMat.normal);
            glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, lampMat.roughness);
            
            for (int i = 0; i < 15; ++i) {
                glm::mat4 lampMatrix = glm::mat4(1.0f);
                
                // Umieść lampę w pozycji lampPositions (już zawiera prawidłową wysokość)
                lampMatrix = glm::translate(lampMatrix, lampPositions[i]);
                
                // Powiększ lampę do lepszego rozmiaru
                lampMatrix = glm::scale(lampMatrix, glm::vec3(4.0f, 4.0f, 4.0f)); 
                
                glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lampMatrix));
                
                lampModel->Draw(shader);
            }

            if (frameModel && canvasModel) {
                for (int i = 0; i < 4; ++i) {
                    float offsetZ = -14.9f + (i % 2) * 30.0f; // prawa i lewa ściana
                    float offsetY = 0.0f;
                    float offsetX = (i < 2) ? -14.9f : 14.9f;
                    float rotation = (i < 2) ? 90.0f : -90.0f;

                    glm::mat4 frameMatrix = glm::mat4(1.0f);
                    frameMatrix = glm::translate(frameMatrix, glm::vec3(offsetX, offsetY, offsetZ));
                    frameMatrix = glm::rotate(frameMatrix, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
                    frameMatrix = glm::scale(frameMatrix, glm::vec3(30.0f));
                    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(frameMatrix));

                    // Rysuj ramę
                    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, frameMat.albedo);
                    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, frameMat.normal);
                    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, frameMat.roughness);
                    frameModel->Draw(shader);

                    // Rysuj płótno
                    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, canvasMat.albedo);
                    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, canvasMat.normal);
                    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, canvasMat.roughness);
                    canvasModel->Draw(shader);
                }
            }

            if (maxwellModel) {
                glm::mat4 maxwellMatrix = glm::mat4(1.0f);
                maxwellMatrix = glm::translate(maxwellMatrix, glm::vec3(0.0f, -7.2f, 0.0f));
                maxwellMatrix = glm::rotate(maxwellMatrix, maxwellAngle, glm::vec3(0.0f, 1.0f, 0.0f));
                maxwellMatrix = glm::scale(maxwellMatrix, glm::vec3(0.15f));
                glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(maxwellMatrix));

                glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, maxwellTex);
                glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
                glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
                maxwellModel->Draw(shader);
            }

            
            // Przywróć macierz modelu jednostkową
            glm::mat4 identityMatrix = glm::mat4(1.0f);
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(identityMatrix));
        }
        
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
            std::cerr << "GL ERROR: " << err << std::endl;        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Wyczyść pamięć
    if (lampModel) {
        delete lampModel;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
