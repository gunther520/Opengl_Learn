
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main() {
    glfwInit();
    // Set the OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set the major version of OpenGL to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set the minor version of OpenGL to 3
    // Set the version of OpenGL to 3.3 Finished

    /*-------------------Set the profile to core profile-------------------
    * 
    * We also tell GLFW we want to explicitly use the core-profile. 
    * Telling GLFW we want to use the core-profile means we’ll get 
    * access to a smaller subset of OpenGL features without 
    * backwards-compatible features we no longer need.
    */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);// Set the viewport size
    // Set the callback function for resizing the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    //-------------------Create a vertex buffer object store in GPU-------------------
    //gl is a state machine, so we can set the state of OpenGL by calling appropriate functions

    float vertices[] = {
         0.75f,  0.75f, 0.0f, // top right
         0.15f, -0.70f, 0.0f, // bottom right
        -0.05f, -0.75f, 0.0f, // bottom left
    };

    float vertices2[] = {
        // positions        // colors         // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };

    float texCoords[] = {
            0.0f, 0.0f, // lower-left corner
            1.0f, 0.0f, // lower-right corner
            0.5f, 1.0f  // top-center corner
    };

    unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


// !!!! The sequence of the objects is important!!!!
// !!!! The sequence of the objects is important!!!!
// !!!! The sequence of the objects is important!!!!




    unsigned int VAO[2]; // Usage: determine how to retrieve the vertex data
    glGenVertexArrays(2, VAO);

    unsigned int VBO[2]; // Vertex Buffer Object (VBO) ID  
    glGenBuffers(2, VBO); // Generate 1 buffer object and store its ID in VBO

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
        (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		(void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    


//Why Binding is important? So that openGL can use the state to draw the object


    Shader ourShader1("res/vertexShader1.txt", "res/fragmentShader1.txt");
    Shader ourShader2("res/vertexShader2.txt", "res/fragmentShader2.txt");


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("res/container.jpg", &width, &height,
        &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // Tell OpenGL how to interpret the vertex data in this case 3 floats per positional vertex
    //This will update VAO we defined earlier's state, it also stores the VBO Target's state in VAO


    /*The following code is for the case where we have 6 floats per vertex, 3 for position and 3 for color
    * -------------------Tell OpenGL how to interpret the vertex data-------------------
    * two attributes are defined in VAO, one for position and one for color
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)( 3 * sizeof(float) ));
    */

    

    // Render loop one iteration is called a frame
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window); // Check if the user has pressed the escape key (if so, set the windowShouldClose property to true)


        
        //// Generate random color
        //std::random_device rd;
        //std::mt19937 gen(rd());
        //std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        //float r = dis(gen);
        //float g = dis(gen);
        //float b = dis(gen);
        

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        ourShader2.use();
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

     
        glBindVertexArray(0);
        glBindVertexArray(VAO[0]);
        ourShader1.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        

        


        // Render commands here
        glfwPollEvents(); // Check if any events are triggered (like keyboard input or mouse movement events)
        glfwSwapBuffers(window); // Swap the color buffer 
        
    }

    glfwTerminate(); // Terminate GLFW and clean up any resources it used

    return 0;
}