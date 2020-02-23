# University Public Examples
 Most of my university coursework are within private repositories. However, I have placed examples of my work within this repository for public review.



## CSC8501 - Advanced Programming For Games

A simple **C++** program that can generate 15-tile style puzzles (writing them to a file), and given a set of puzzles, calculate the number of continuous rows and columns for all reachable states of each puzzle. This is then written to a file.



- **Configuration.h**: The declaration of a class for storing the state of a puzzle. These can be of arbitrary width and height.
- **Configuration.cpp**: The definition of a class for storing the state of a puzzle.
- **Solver.h**: The declaration of a class for solving a set of Configuration objects. 
- **Solver.cpp**: The definition of a the Solver class. After it is created, you can pass it Configuration objects and get it to solve them, storing the solutions. The solver uses mathematical equations to calculate the number of continuations (inc. reverse) as well as partial continuations (inc. reverse).
- **15-File.txt**: Example output from generating a set of puzzles.
- **Solution.txt**: Example output from solving a set of puzzles.



## CSC8502 - Advanced Graphics for Games

A scene of a landscape that rises out of the water, rendered using **C++** and **OpenGL**. To see a video of the project, you can visit [my portfolio](https://charsleyj.github.io).



- **SceneNode.h**: Declaration of a SceneNode class. These structure the scene by forming a tree. Each node can store a mesh, an optional shader, a relative transform (from the parent) and a vector of child nodes. This can also be inherited from and can add additional uniforms for the attached shader.
- **SceneNode.cpp**: Definition of the SceneNode class.
- **DepthOfFieldFragment.glsl**: A OpenGL fragment shader that creates a depth of field effect by applying a gaussian blur over fragments with a depth value within a given range.
- **sobelDepthFragment.glsl**: An OpenGL fragment shader that applies Sobel Edge Detection using the depth buffer to create an outline post processing effect.
- **sobelFilterFragment.glsl**: An OpenGL fragment shader that applies Sobel Edge Detection to the scene. This allows for either the traditional monochrome version, or an RGB variant but taking in a boolean uniform.