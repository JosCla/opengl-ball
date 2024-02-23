# OpenGL Ball
A simple demonstration of ball physics in OpenGL, in an interactive environment. Move around and push an ellipsoid that collides with the environment!

I originally built this project in Summer of 2020 to learn and practice C++, Linux, and a bit of Blender, too.

If you look in the screenshots folder, you can see screenshots I took while developing the program!

## Building the Project
Navigate to the root of the project, and run `make`.

This project uses OpenGL, GLFW, GLAD, STB_IMAGE, and GLM.

## Usage
After building the project, run the project with `./main`.
- Use the mouse to rotate the camera
- Use WASD to move around
- Use Escape to quit
- Use T to toggle your flashlight
- Use N to toggle noclip (allows you to fly up and down)
- Hold F to render in wireframe mode
- Use F11 to take a screenshot
- Use F10 to unlock/lock the cursor

## Resources
- The base OpenGL graphical renderer was built following the Learn OpenGL website's tutorial: [https://learnopengl.com/](https://learnopengl.com/)
- The ellipsoid-to-triangle collision detection was built following "Improved Collision detection and Response" by Kasper Fauerby: [https://www.peroxide.dk/papers/collision/collision.pdf](https://www.peroxide.dk/papers/collision/collision.pdf)
