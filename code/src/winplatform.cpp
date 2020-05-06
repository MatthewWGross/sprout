#include "winplatform.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
// Include GLEW
#include <glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "objloader.hpp"
#include "vboindexer.hpp"

#include "input.h"

// Dumping ground for temporary globals
namespace
{
    std::vector<unsigned short> indices;

    // This will identify our vertex buffer
    GLuint uvbuffer;
    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;
    GLuint VertexArrayID;
    GLuint MatrixID;
    GLuint ModelMatrixID;
    GLuint ViewMatrixID;
    GLuint programID;
    GLuint textureID;
    GLuint LightID;
    glm::mat4 MVP;
    GLFWwindow *window;
} // namespace

namespace Platform
{
void InitPlatformAndWindow()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE; // Needed for core profile
    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("./data/shaders/simpleVertexShader.vertexShader", "./data/shaders/simpleFragmentShader.fragmentShader");

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    ModelMatrixID = glGetUniformLocation(programID, "M");
    ViewMatrixID = glGetUniformLocation(programID, "V");

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    printf("Loading Image\n");
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("./data/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("Image Loaded successfully!, width %i, height %i\n", width, height);
    }
    else
    {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

    // Read our .obj file
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals; // Won't be used at the moment.
    bool res = loadOBJ("./data/models/suzanne.obj", vertices, uvs, normals);

    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);


    // Bind our VBO Data
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);


	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

}

int getGLFWKeyForInput(e_sproutInputKey key)
{
    switch (key)
    {
        case KEY_DOWN: return GLFW_KEY_S;
        case KEY_UP: return GLFW_KEY_W;
        case KEY_LEFT: return GLFW_KEY_A;
        case KEY_RIGHT: return GLFW_KEY_D;
        case KEY_ENTER: return GLFW_KEY_ENTER;
        case KEY_ESCAPE: return GLFW_KEY_ESCAPE;
        case KEY_SPACE: return GLFW_KEY_SPACE;
    }

}

void UpdateInput(sproutInputState& input)
{
    glfwPollEvents();
    auto tmp = input.currentInput;
    input.currentInput = input.previousInput;
    input.previousInput = tmp;

    for(int i = 0; i < INPUT_COUNT; i++)
    {
        input.currentInput[i] = glfwGetKey(window, getGLFWKeyForInput(e_sproutInputKey(i))) == GLFW_PRESS;
    }
}

static float gModelRot = 0.0f;
void UpdateRender()
{
    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::rotate(Model, gModelRot, glm::vec3(0, 1, 0));

    gModelRot += .01f;
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
    

    glUseProgram(programID);
    glUniform1i(glGetUniformLocation(programID, "ourTexture"), 0);

    glm::vec3 lightPos = glm::vec3(4,4,4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        1,        // attribute. No particular reason for 1, but must match the layout in the shader.
        2,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		static_cast<GLsizei>(indices.size()),    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // Swap buffers
    glfwSwapBuffers(window);
}

void Shutdown()
{
    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

void SetShouldShutdown()
{
    glfwSetWindowShouldClose(window, true);
}

bool ShouldShutdown()
{
    return glfwWindowShouldClose(window) !=0;
}

} // namespace Platform