Lesson 0: Setup + setting pixel colours on the window

We will provide the students with a default project setup - it will be an Arduino style development process. Students will write in C and will only have 2 functions to implement:
Initialize()
Update()
The application will call initialize() at startup, and update() every frame (60 times a second). 

Initialize() will be used to create/allocate any state the program may need during runtime. This could be a counter which increments each frame, another image itself, a colour (if we are blending between colours we need to have a variable which slowly changes over time, etc…).

Update() will be used to set the colours of the window we are drawing on. The function will take a pointer to a buffer of pixels which we have full freedom over. After the update function is called, the window gets updated with the new colour values we wrote into that buffer.


The assignment of this lesson will be to fill the window with a single color and then to only color in a rectangle of the window.


Lesson 1: Moving rectangle

For this assignment, we build off of the previous with the rectangle which takes over part of the screen and make it move over time. This will involve creating a global variable (initialized to 0 in the initialize() function).


