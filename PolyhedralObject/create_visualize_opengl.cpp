#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// Struct to hold vertex data
struct Vertex {
    float x, y, z;
};

// Function to read object data from a file
void readObjectFile(const std::string& filename, std::vector<Vertex>& vertices, std::vector<std::vector<int>>& faces) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string line;
    // Read vertices
    while (std::getline(file, line)) {
        if (line == "3D") continue; // Skip the first line indicating 3D object
        std::istringstream iss(line);
        Vertex v;
        iss >> v.x >> v.y >> v.z;
        vertices.push_back(v);
    }

    // Read faces
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> face;
        int index;
        while (iss >> index) {
            face.push_back(index);
        }
        faces.push_back(face);
    }
}

// OpenGL initialization and rendering function
void render(const std::vector<Vertex>& vertices, const std::vector<std::vector<int>>& faces) {
    glBegin(GL_TRIANGLES); // You can change this to GL_LINES or GL_QUADS depending on your faces

    for (const auto& face : faces) {
        for (int index : face) {
            const Vertex& v = vertices[index];
            glVertex3f(v.x, v.y, v.z);
        }
    }

    glEnd();
}

int main() {
    // Initialize GLFW and create a window
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Object Visualization", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    std::vector<Vertex> vertices;
    std::vector<std::vector<int>> faces;
    readObjectFile("your_exported_file.txt", vertices, faces); // Replace with your actual filename

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set camera position
        glTranslatef(0.0f, 0.0f, -5.0f);

        render(vertices, faces);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
