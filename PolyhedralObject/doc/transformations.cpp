#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include <limits>
#include <iomanip>
#include <cstdlib> // for system()

// Vertex class
class Vertex {
public:
    float x, y, z;
    Vertex(float xCoord = 0, float yCoord = 0, float zCoord = 0)
        : x(xCoord), y(yCoord), z(zCoord) {}

    void print() const {
        std::cout << "Vertex(" << x << ", " << y << ", " << z << ")\n";
    }
};

// Utility function to clear the input stream
void clearInputStream() {
    std::cin.clear();  // Clear error flags
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore bad input
}

// Shape2D class
class Shape2D {
public:
    std::vector<Vertex> vertices;

    void addVertex(float x, float y) {
        vertices.emplace_back(x, y, 0.0f); // z is zero for 2D objects
    }

    void translate(float dx, float dy) {
        for (auto& vertex : vertices) {
            vertex.x += dx;
            vertex.y += dy;
        }
    }

    void scale(float factor) {
        for (auto& vertex : vertices) {
            vertex.x *= factor;
            vertex.y *= factor;
        }
    }

    void rotate(float angleDegrees) {
        float angleRadians = angleDegrees * M_PI / 180.0f;
        for (auto& vertex : vertices) {
            float newX = vertex.x * cos(angleRadians) - vertex.y * sin(angleRadians);
            float newY = vertex.x * sin(angleRadians) + vertex.y * cos(angleRadians);
            vertex.x = newX;
            vertex.y = newY;
        }
    }

    void printVertices() const {
        std::cout << "2D Shape Vertices:\n";
        for (const auto& vertex : vertices) {
            vertex.print();
        }
    }

    // Method to export vertices to a file
    void exportToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error opening file for writing!" << std::endl;
            return;
        }

        file << "2D\n"; // Indicate the type of shape
        for (const auto& vertex : vertices) {
            file << vertex.x << " " << vertex.y << "\n";
        }

        file.close();
        std::cout << "Vertices exported to " << filename << " successfully!\n";
    }
};

// Shape3D class
class Shape3D {
public:
    std::vector<Vertex> vertices;

    void addVertex(float x, float y, float z) {
        vertices.emplace_back(x, y, z);
    }

    void translate(float dx, float dy, float dz) {
        for (auto& vertex : vertices) {
            vertex.x += dx;
            vertex.y += dy;
            vertex.z += dz;
        }
    }

    void scale(float factor) {
        for (auto& vertex : vertices) {
            vertex.x *= factor;
            vertex.y *= factor;
            vertex.z *= factor;
        }
    }

    void rotateX(float angleDegrees) {
        float angleRadians = angleDegrees * M_PI / 180.0f;
        for (auto& vertex : vertices) {
            float newY = vertex.y * cos(angleRadians) - vertex.z * sin(angleRadians);
            float newZ = vertex.y * sin(angleRadians) + vertex.z * cos(angleRadians);
            vertex.y = newY;
            vertex.z = newZ;
        }
    }

    void rotateY(float angleDegrees) {
        float angleRadians = angleDegrees * M_PI / 180.0f;
        for (auto& vertex : vertices) {
            float newX = vertex.z * sin(angleRadians) + vertex.x * cos(angleRadians);
            float newZ = vertex.z * cos(angleRadians) - vertex.x * sin(angleRadians);
            vertex.x = newX;
            vertex.z = newZ;
        }
    }

    void rotateZ(float angleDegrees) {
        float angleRadians = angleDegrees * M_PI / 180.0f;
        for (auto& vertex : vertices) {
            float newX = vertex.x * cos(angleRadians) - vertex.y * sin(angleRadians);
            float newY = vertex.x * sin(angleRadians) + vertex.y * cos(angleRadians);
            vertex.x = newX;
            vertex.y = newY;
        }
    }

    void printVertices() const {
        std::cout << "3D Shape Vertices:\n";
        for (const auto& vertex : vertices) {
            vertex.print();
        }
    }

    // Method to export vertices to a file
    void exportToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error opening file for writing!" << std::endl;
            return;
        }

        file << "3D\n"; // Indicate the type of shape
        for (const auto& vertex : vertices) {
            file << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
        }

        file.close();
        std::cout << "Vertices exported to " << filename << " successfully!\n";
    }
};

// Function to read vertices from a file
bool readVerticesFromFile(const std::string& filename, bool& is2D, std::vector<Vertex>& vertices) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
    }

    std::string objectType;
    file >> objectType;
    is2D = (objectType == "2D");

    float x, y, z;
    while (file >> x >> y >> z) {
        vertices.emplace_back(x, y, z);
    }

    file.close();
    return true;
}

int main() {
    int loadChoice;
    bool is2D = false;
    std::vector<Vertex> vertices;

    // Loop until valid choice is entered
    while (true) {
        std::cout << "Do you want to load an object from a file?\n";
        std::cout << "1. Yes\n";
        std::cout << "2. No (create a new object)\n";
        std::cin >> loadChoice;
        if (std::cin.fail() || (loadChoice != 1 && loadChoice != 2)) {
            clearInputStream();
            std::cout << "Invalid choice. Please enter 1 or 2.\n";
        } else {
            break;
        }
    }

    // Load object from file
    if (loadChoice == 1) {
        std::string filename;
        std::cout << "Enter the filename to load (e.g., vertices.txt): ";
        std::cin >> filename;

        if (!readVerticesFromFile(filename, is2D, vertices)) {
            std::cerr << "Error loading file. Exiting." << std::endl;
            return 1;
        }
    } else {
        // Ask if the user wants to create a 2D or 3D object
        int objectTypeChoice;
        while (true) {
            std::cout << "What type of object would you like to create?\n";
            std::cout << "1. 2D\n";
            std::cout << "2. 3D\n";
            std::cin >> objectTypeChoice;
            if (std::cin.fail() || (objectTypeChoice != 1 && objectTypeChoice != 2)) {
                clearInputStream();
                std::cout << "Invalid choice. Please enter 1 or 2.\n";
            } else {
                break;
            }
        }

        // Vertex input for new object
        if (objectTypeChoice == 1) {
            is2D = true;
            Shape2D shape2D;
            int numVertices;
            std::cout << "Enter the number of vertices: ";
            std::cin >> numVertices;

            for (int i = 0; i < numVertices; ++i) {
                float x, y;
                std::cout << "Enter coordinates (x y) for vertex " << (i + 1) << ": ";
                std::cin >> x >> y;
                shape2D.addVertex(x, y);
            }
            shape2D.printVertices();
            vertices = shape2D.vertices; // Store vertices for further operations
        } else {
            is2D = false;
            Shape3D shape3D;
            int numVertices;
            std::cout << "Enter the number of vertices: ";
            std::cin >> numVertices;

            for (int i = 0; i < numVertices; ++i) {
                float x, y, z;
                std::cout << "Enter coordinates (x y z) for vertex " << (i + 1) << ": ";
                std::cin >> x >> y >> z;
                shape3D.addVertex(x, y, z);
            }
            shape3D.printVertices();
            vertices = shape3D.vertices; // Store vertices for further operations
        }
    }

    // Perform operations
    char continueChoice;
    do {
        int operationChoice;
        std::cout << "Choose an operation:\n";
        std::cout << "1. Translate\n";
        std::cout << "2. Scale\n";
        std::cout << "3. Rotate\n";
        std::cout << "Enter your choice (1-3): ";
        std::cin >> operationChoice;

        if (std::cin.fail() || operationChoice < 1 || operationChoice > 3) {
            clearInputStream();
            std::cout << "Invalid operation choice. Please select a valid option.\n";
            continue; // Ask for the operation again
        }

        if (is2D) {
            Shape2D shape2D;
            shape2D.vertices = vertices; // Load vertices into shape2D

            if (operationChoice == 1) {
                float dx, dy;
                std::cout << "Enter translation values (dx dy): ";
                std::cin >> dx >> dy;
                shape2D.translate(dx, dy);
            } else if (operationChoice == 2) {
                float factor;
                std::cout << "Enter scaling factor: ";
                std::cin >> factor;
                shape2D.scale(factor);
            } else if (operationChoice == 3) {
                float angle;
                std::cout << "Enter rotation angle (degrees): ";
                std::cin >> angle;
                shape2D.rotate(angle);
            }

            shape2D.printVertices(); // Print updated vertices
            shape2D.exportToFile("shape2D_vertices.txt");

            // Call Python script for further visualization
            system("python3 visualize2d.py shape2D_vertices.txt");

            vertices = shape2D.vertices; // Update main vertices after operation
        } else {
            Shape3D shape3D;
            shape3D.vertices = vertices; // Load vertices into shape3D

            if (operationChoice == 1) {
                float dx, dy, dz;
                std::cout << "Enter translation values (dx dy dz): ";
                std::cin >> dx >> dy >> dz;
                shape3D.translate(dx, dy, dz);
            } else if (operationChoice == 2) {
                float factor;
                std::cout << "Enter scaling factor: ";
                std::cin >> factor;
                shape3D.scale(factor);
            } else if (operationChoice == 3) {
                float angle;
                std::cout << "Enter rotation angle (degrees) around axis (X/Y/Z): ";
                char axis;
                std::cin >> angle >> axis;
                if (axis == 'X' || axis == 'x') {
                    shape3D.rotateX(angle);
                } else if (axis == 'Y' || axis == 'y') {
                    shape3D.rotateY(angle);
                } else if (axis == 'Z' || axis == 'z') {
                    shape3D.rotateZ(angle);
                } else {
                    std::cout << "Invalid axis choice. Skipping rotation.\n";
                }
            }

            shape3D.printVertices(); // Print updated vertices
            shape3D.exportToFile("shape3D_vertices.txt");

            // Call Python script for further visualization
            system("python3 visualize_3d.py shape3D_vertices.txt");

            vertices = shape3D.vertices; // Update main vertices after operation
        }

        std::cout << "Do you want to perform another operation? (Y/N): ";
        std::cin >> continueChoice;
    } while (continueChoice == 'Y' || continueChoice == 'y');

    std::cout << "Exiting the program. Goodbye!\n";
    return 0;
}
