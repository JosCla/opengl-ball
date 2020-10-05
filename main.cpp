#include <glad/glad.h> // loads opengl flags
#include <GLFW/glfw3.h> // lets us create a window and take inputs
#include <glm/glm.hpp> // gl mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stdlib
#include <iostream> // for cin, cout, endl
#include <cmath> // for sin
#include <string> // for string, to_string

// our headers
#include "src/shader.h" // defines the Shader class
#include "src/utils.h" // defines some utility functions
#include "src/camera.h" // defines the Camera class
#include "src/light.h" // defines the Light class
#include "src/mesh.h" // defines the Mesh class
#include "src/model.h" // defines the Model class
#include "src/text.h" // defines the Text class
#include "src/shapes.h" // defines Shape classes
#include "src/collision.h" // defines some collision functions

// declaring some global variables before main
Camera camera(glm::vec3(-2.0, 1.0, -2.0), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float prevX = SCR_WIDTH / 2.0f;
float prevY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool stayXZ = true;
bool flashLightOn = false;
bool cursorLocked = true;

int main()
{
  // getting everything initialized
  ///////////////////////////////////////////////////////////////////////////////////////
  // initializing glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // creating window
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Camera", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // also telling glfw to capture mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // use glad to load opengl function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // getting and building shaders, models, etc. from other files
  ///////////////////////////////////////////////////////////////////////////////////////
	// shaders
  Shader objectShader("shaders/object.vert", "shaders/object.frag");
  Shader lightShader("shaders/light.vert", "shaders/light.frag");
	Shader textShader("shaders/text.vert", "shaders/text.frag");
  
	// the model
	std::string filepath = "resources/box-scene/box-scene.obj";
	Model ourModel(filepath.c_str());

	std::vector<Triangle> tris = ourModel.ToTriangles();

	// another model
	std::string filepath2 = "resources/boxsphere/boxsphere.obj";
	Model sphereModel(filepath2.c_str());
	Ellipsoid sphere(1.0f, glm::vec3(0.0f, 2.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// the text
	Text sampleText("hello there, world!", SCR_WIDTH, SCR_HEIGHT, 30, 30, 400, 20, 5);
	sampleText.SetText("Basic 3D Environment");

  // getting some last things done before entering main rendering loop
  ///////////////////////////////////////////////////////////////////////////////////////

  // getting lighting stuff set up
  glm::vec3 lightColor = glm::vec3(1.0f);

	PointLight ptLights[] = {
    PointLight(glm::vec3(0.0f, 1.0f, 2.0f), lightColor * 0.0f, lightColor * 0.5f, lightColor)
	};

	DirectionalLight dirLights[] = {
    DirectionalLight(glm::vec3(-1.0f, -1.0f, 0.3f), lightColor * 0.1f, lightColor * 0.0f, lightColor * 0.0f)
	};

	SpotLight spotLights[] = {
		SpotLight(glm::vec3(0.0f), glm::vec3(0.0f), glm::cos(glm::radians(15.0f)), glm::cos(glm::radians(20.0f)), lightColor * 0.0f, lightColor * 0.8f, lightColor * 1.0f, 1.0f, 0.09f, 0.032f),
		SpotLight(glm::vec3(1.0f), glm::vec3(1.0f), glm::cos(glm::radians(180.0f)), glm::cos(glm::radians(180.0f)), glm::vec3(0.0f), glm::vec3(0.5f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f),
    SpotLight(glm::vec3(5.2f, -6.0f, -3.0f), glm::vec3(-0.2f, 0.0f, 1.0f), glm::cos(glm::radians(18.0f)), glm::cos(glm::radians(25.0f)), lightColor * 0.0f, lightColor * 0.5f, lightColor * 1.0f)
	};

  // also enabling depth test to make sure that stuff in the front is drawn
  glEnable(GL_DEPTH_TEST);

  // main rendering loop
  ////////////////////////////////////////////////////////////////////////////////////////
  while (!glfwWindowShouldClose(window))
  {
    // calculating deltaTime (time between frames)
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

		// getting continuous inputs
		processInput(window);

		objectShader.use();

    // move stuff like lights as needed
    ptLights[0].Position.x = 2.0f * std::cos(glfwGetTime() / 2.0f);
    ptLights[0].Position.z = 2.0f * std::sin(glfwGetTime() / 2.0f);
		spotLights[0].Position = camera.CameraPosition;
		spotLights[0].Direction = camera.CameraDirection;
		if (spotLights[0].IsOn != flashLightOn)
			spotLights[0].IsOn = flashLightOn;

		// configuring uniforms
		//ptLights[0].AddLight(objectShader, 0, "ptlight");
    dirLights[0].AddLight(objectShader, 0, "dirlight");
		spotLights[0].AddLight(objectShader, 0, "spotlight");
		spotLights[1].AddLight(objectShader, 1, "spotlight");
    objectShader.setVec3("material.specular", glm::vec3(0.1f));
    objectShader.setFloat("material.shininess", 31.0f);
		objectShader.setVec3("viewPos", camera.CameraPosition);
    objectShader.setFloat("time", glfwGetTime() / 3);
    objectShader.setBool("doEmission", false);

    // remaking matrices
    glm::mat4 modelObject = glm::mat4(1.0f);
    modelObject = glm::translate(modelObject, glm::vec3(0.0f));

    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); 

		objectShader.use();
		objectShader.setMat4("model", modelObject);
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);

    // background
    glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// drawing environment
		ourModel.Draw(objectShader);

		// drawing sphere
		if (glm::length(sphere.Position - camera.CameraPosition) <= 1.2f)
		{
			sphere.Velocity += glm::normalize(sphere.Position - camera.CameraPosition) * 0.005f;
		}
		handleIntersection(sphere, tris, sphere.Position, sphere.Velocity);
		
		glm::mat4 modelSphere = glm::mat4(1.0f);

		modelSphere = glm::translate(modelSphere, sphere.Position);
		modelSphere = glm::scale(modelSphere, glm::vec3(2.5f));
		objectShader.setMat4("model", modelSphere);

		sphereModel.Draw(objectShader);

		// drawing text
		sampleText.DrawText(textShader);

    // swap buffers, poll I/O events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // letting glfw clean up
  glfwTerminate();
  return 0;
}

// process (continuous) inputs
// wasd - move around
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessMovement(FORWARD, deltaTime, stayXZ);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessMovement(LEFT, deltaTime, stayXZ);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessMovement(BACKWARD, deltaTime, stayXZ);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessMovement(RIGHT, deltaTime, stayXZ);
}
// process (discontinuous) inputs
// esc - exit window
// f - activate wireframe mode
// n - activate/deactivate noclip
// t - activate/deactivate flashlight
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  
    glfwSetWindowShouldClose(window, true);
  if (key == GLFW_KEY_F && action == GLFW_PRESS)  
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (key == GLFW_KEY_F && action == GLFW_RELEASE)  
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (key == GLFW_KEY_N && action == GLFW_PRESS)
    stayXZ = !stayXZ;
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		flashLightOn = !flashLightOn;
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
		std::string filename = "screenshots/scrshot_" + std::to_string(time(NULL)) + ".png";
		saveScreenshot(filename.c_str());
	}
	if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
	{
		if (cursorLocked)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstMouse = true;
		}
		if (!cursorLocked)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		cursorLocked = !cursorLocked;
	}
}

// done to make the viewport resize if the window does too
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// rotate the camera whenever the mouse moves
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
  {
    prevX = (float)xpos;
    prevY = (float)ypos;
    firstMouse = false;
  }

  float xoffset = xpos - prevX;
  float yoffset = prevY - ypos; // reversed since y-coordinates go bottom to top
  prevX = xpos;
  prevY = ypos;

  if (cursorLocked)
		camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessScroll(yoffset);
}
