# Computer Graphics assignments
This is a collection of all the assignments of the course Computer Graphics held by Politecnico di Milano during my Computer Science and Engineering Master degree in 2023.
# Structure
Each folder correspond to one assignment.
We were provided a .pdf with the requirements and all the files necessary to correctly display the solutions of the assignments.
# Template Generator 
It was used to generate the project folders without the tedious setup phase 
It is not my work and was developed by a fellow student whose work can be found [here](https://github.com/Lorenzo978/computer-graphics-empty-project.git).
### Prerequisites
- This project template ONLY works Visual Studio on Windows.
- You must have already installed Vulkan SDK.
### Create a new empty project
1. If your Visual Studio version is different from 2022, open the file `GenerateProjects.bat` in the `template` folder with a text editor and change `vs2022` with your version.
2. Execute the python script `generate.py`, which, by inputting a name, will create a new project folder and generate the solution files.
3. Open the created folder and launch the `.sln` file.

The project has this structure:
- `src` contains your code. By default, it contains only `main.cpp`. Put here the file `A##.cpp` and rename it to `main.cpp`.
- `vendor` contains all the libraries. If you need to add other includes, such as `stb_image.h` given by the professor in the `headers` folder, put them inside `vendor/other` folder.

For any other folders, such as `shaders`, `textures` etc. copy them outside the `src` folder.