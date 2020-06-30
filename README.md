# University Public Examples
 Most of my university coursework are within private repositories. However, I have placed examples of my work within this repository for public review.

## CSC8498 - Project and Dissertation for MCOMP

Final project of my degree. During this, I investigated different ways of visualising 4D scenes in real time. This used **C++** and **OpenGL**. To see a video of the results, you can visit [my portfolio](https://charsleyj.github.io/4DVisualisation.html).



- **csVertex.glsl, csGeometry.glsl, csFragment.glsl:** Vertex, geometry and fragment shaders for rendering a 3D cross-section of a 4D object. This involved calculating new vertices for the intersection with the 3D plane w=0.
- **wireVertex, wireFragment.glsl:** Vertex and Fragment shaders for rendering a wireframe projection of the 4D object. This takes in a 5x5 projection matrix for projecting the points into 3D space.

## CSC8503 - Advanced Game Technologies

My third coursework for my final year. A simple **C++** game where you control a goose, stealing items and taking them back to your island. We were tasked with adding physics, collision detection, AI and Networking to the game. To see a video of the project, you can visit [my portfolio](https://charsleyj.github.io/UnoriginalGooseGame.html).



- **CollisionLayer**: A class for defining unity style collisions layers. Objects on a layer will only collide with objects on certain user defined layers. Layers are given an ID and a mask for which layers to collide with using bitwise logic.
- **GooseObject**: The player object. Player can move, pick up items and jump. Contains a finite state machine. When the goose collides with the park keeper, will go to a temporary state, moving in a random direction.
- **ParkKeeperObject**: An enemy which chases the goose object using A\* pathfinding.
- **VillagerObject**: An enemy which has a finite state machine. If the goose gets close with an item, it will chase using A\* pathfinding. If the goose gets away, it will go back to its spawn.

## CSC8502 - Advanced Graphics for Games

My second coursework for my final year. A scene of a landscape that rises out of the water, rendered using **C++** and **OpenGL**. To see a video of the project, you can visit [my portfolio](https://charsleyj.github.io).



- **SceneNode.h**: Declaration of a SceneNode class. These structure the scene by forming a tree. Each node can store a mesh, an optional shader, a relative transform (from the parent) and a vector of child nodes. This can also be inherited from and can add additional uniforms for the attached shader.
- **SceneNode.cpp**: Definition of the SceneNode class.
- **DepthOfFieldFragment.glsl**: A OpenGL fragment shader that creates a depth of field effect by applying a gaussian blur over fragments with a depth value within a given range.
- **sobelDepthFragment.glsl**: An OpenGL fragment shader that applies Sobel Edge Detection using the depth buffer to create an outline post processing effect.
- **sobelFilterFragment.glsl**: An OpenGL fragment shader that applies Sobel Edge Detection to the scene. This allows for either the traditional monochrome version, or an RGB variant but taking in a boolean uniform.

## CSC8501 - Advanced Programming For Games

My first coursework for my final year. A simple **C++** program that can generate 15-tile style puzzles (writing them to a file), and given a set of puzzles, calculate the number of continuous rows and columns for all reachable states of each puzzle. This is then written to a file.



- **CSC8501-Coursework.cpp**: Contains the main function, controls the flow of the menus.
- **Configuration.h**: The declaration of a class for storing the state of a puzzle. These can be of arbitrary width and height.
- **Configuration.cpp**: The definition of a class for storing the state of a puzzle.
- **Solver.h**: The declaration of a class for solving a set of Configuration objects. 
- **Solver.cpp**: The definition of a the Solver class. After it is created, you can pass it Configuration objects and get it to solve them, storing the solutions. The solver uses mathematical equations to calculate the number of continuations (inc. reverse) as well as partial continuations (inc. reverse).
- **15-File.txt**: Example output from generating a set of puzzles.
- **Solution.txt**: Example output from solving a set of puzzles.