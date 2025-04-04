
// Includes
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "ShaderLoader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Load Image data
int ImageWidth;
int ImageHeight;
int ImageComponents;

unsigned char* ImageData = stbi_load("Resources/Textures/Run.png",
	&ImageWidth, &ImageHeight, &ImageComponents, 0);

unsigned char* ImageMixData = stbi_load("Resources/Textures/Jump.png",
	&ImageWidth, &ImageHeight, &ImageComponents, 0);

// Globals
GLFWwindow* Window = nullptr;

GLuint Texture_Run;
GLuint Texture_Jump;

// check how many components the loaded image has (RGBA or RGB?)
GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

GLuint VBO_Quad;
GLuint Program_Texture;
GLuint Program_Mix_Texture;

glm::vec3 QuadPosition = glm::vec3(0.5f, 0.5f, 0.5f);
glm::mat4 TranslationMat;

float QuadRotationAngle = 45.0f;
glm::mat4 RotationMat;

glm::vec3 QuadScale = glm::vec3(0.5f, 0.5f, 1.0f);
glm::mat4 ScaleMat;

glm::mat4 ModelMat;

GLuint EBO_Quad;

GLfloat CurrentTime;

GLuint VAO_Quad;

GLuint Indices_Quad[] = { 0, 1, 2, 2, 3, 0 };


GLfloat Vertices_Quad[] = {
	// Position               // Color            // Texture Coords (flipped)
	-0.5f,  0.5f, 0.0f,       1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Top-left 
	-0.5f, -0.5f, 0.0f,       0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Bottom-left 
	 0.5f, -0.5f, 0.0f,       0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Bottom-right 
	 0.5f,  0.5f, 0.0f,       1.0f, 1.0f, 0.0f,  1.0f, 0.0f  // Top-right 
};


// Functions
void InitialSetup();
void Update();
void Render();



// Main 
int main()
{
	stbi_set_flip_vertically_on_load(true);

	// GLFW
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// Create GLFW Wiundow
	Window = glfwCreateWindow(800, 800, "First OpenGL Window", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "GLFW failed to initialize properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	if (!ImageData) {
		std::cout << "Failed to load texture!" << std::endl;
	}
	else {
		std::cout << "Texture loaded successfully: " << ImageWidth << "x" << ImageHeight << std::endl;
	}

	
	if (!ImageMixData) {
		std::cout << "Failed to load texture!" << std::endl;
	}
	else {
		std::cout << "Texture loaded successfully: " << ImageWidth << "x" << ImageHeight << std::endl;

	}

	// Setup Initial
	InitialSetup();

	// Loop
	while (glfwWindowShouldClose(Window) == false)
	{
		// Update all objects 
		Update();

		// Render all the objects
		Render();
	}
	

	glfwTerminate();
	return 0;
}

void InitialSetup()
{


		// Set the background color
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

		// Set viewport size
		glViewport(0, 0, 800, 800);


		// Create shader program
		Program_Texture = ShaderLoader::CreateProgram("Resources/Texture.vert",
			"Resources/Texture.frag");

		// Create Second Program
		Program_Mix_Texture = ShaderLoader::CreateProgram("Resources/Texture.vert", "Resources/TextureMix.frag");

		// Generate and bind VAO
		glGenVertexArrays(1, &VAO_Quad);
		glBindVertexArray(VAO_Quad);

		// Generate and bind VBO
		glGenBuffers(1, &VBO_Quad);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Quad);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad), Vertices_Quad, GL_STATIC_DRAW);

		// Generate and bind EBO
		glGenBuffers(1, &EBO_Quad);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Quad);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Quad), Indices_Quad, GL_STATIC_DRAW);


		//    create and bind Run texture    //
		glGenTextures(1, &Texture_Run);
		glBindTexture(GL_TEXTURE_2D, Texture_Run);		

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Populate the texture with the image data
		glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);
		glGenerateMipmap(GL_TEXTURE_2D);


		//   create texutre 2 Jump   //
		glGenTextures(1, &Texture_Jump);
		glBindTexture(GL_TEXTURE_2D, Texture_Jump);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageMixData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		// Define vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// Unbind VAO and VBO to prevent accidental modifications
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		stbi_image_free(ImageMixData);
		stbi_image_free(ImageData);

}

void Update()
{
	glfwPollEvents();

	CurrentTime = (float)glfwGetTime();

	// Set only translation matrix 
	glm::mat4 TranslationMat = glm::translate(glm::mat4(1.0f), QuadPosition);

	// Keep rotation fixed at 0 degrees
	glm::mat4 RotationMat = glm::mat4(1.0f); 

	// Keep scale fixed
	glm::mat4 ScaleMat = glm::mat4(1.0f); 

	// Combine transformations
	ModelMat = TranslationMat * RotationMat * ScaleMat;
}

// Render 
void Render()
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_COLOR_BUFFER_BIT);

	// Use shader program
	glUseProgram(Program_Mix_Texture);

	// Get the uniform location for ModelMat
	GLuint modelLoc = glGetUniformLocation(Program_Mix_Texture, "ModelMat");

	// Pass ModelMat to the shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

	// Bind the texture to texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture_Run);
	glUniform1i(glGetUniformLocation(Program_Mix_Texture, "Texture0"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture_Jump);
	glUniform1i(glGetUniformLocation(Program_Mix_Texture, "Texture1"), 1);

	// Bind VAO and draw quad
	glBindVertexArray(VAO_Quad);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);

	// Swap buffers
	glfwSwapBuffers(Window);
}


