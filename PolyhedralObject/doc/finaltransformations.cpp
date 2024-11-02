//transformations with geometric properties added
//use 3dcreated.txt


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

    // Translation, Scaling, and Rotation operations
    // (same as previous versions)
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

    // Property calculations
    float calculateArea() const {
        // Assuming a polygon, we use the Shoelace formula
        float area = 0.0f;
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            area += (vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y);
        }
        return std::abs(area) / 2.0f;
    }

    float calculatePerimeter() const {
        float perimeter = 0.0f;
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            float dx = vertices[j].x - vertices[i].x;
            float dy = vertices[j].y - vertices[i].y;
            perimeter += std::sqrt(dx * dx + dy * dy);
        }
        return perimeter;
    }

    Vertex calculateCentroid() const {
        // Assuming a simple polygon for the centroid calculation
        float cx = 0.0f, cy = 0.0f;
        float signedArea = 0.0f;
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            float a = vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
            signedArea += a;
            cx += (vertices[i].x + vertices[j].x) * a;
            cy += (vertices[i].y + vertices[j].y) * a;
        }
        signedArea *= 0.5;
        cx = (1.0 / (6.0 * signedArea)) * cx;
        cy = (1.0 / (6.0 * signedArea)) * cy;
        return Vertex(cx, cy, 0);
    }

    // Method to export vertices to a file (same as previous version)
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

    // Translation, Scaling, and Rotation operations
    // (same as previous versions)
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

    // Property calculations
    float calculateVolume() const {
        // Assuming a simple tetrahedron for demonstration
        if (vertices.size() < 4) return 0.0f;
        Vertex v0 = vertices[0], v1 = vertices[1], v2 = vertices[2], v3 = vertices[3];
        float volume = std::abs((v1.x - v0.x) * (v2.y - v0.y) * (v3.z - v0.z) -
                                (v1.x - v0.x) * (v2.z - v0.z) * (v3.y - v0.y) +
                                (v1.y - v0.y) * (v2.z - v0.z) * (v3.x - v0.x) -
                                (v1.y - v0.y) * (v2.x - v0.x) * (v3.z - v0.z) +
                                (v1.z - v0.z) * (v2.x - v0.x) * (v3.y - v0.y) -
                                (v1.z - v0.z) * (v2.y - v0.y) * (v3.x - v0.x)) / 6.0f;
        return volume;
    }

    float calculateSurfaceArea() const {
        // Placeholder: Assuming a simple shape (like tetrahedron) for the demonstration
        // In practice, we'd compute the sum of all triangle face areas.
        return 0.0f; // Needs actual calculation logic based on shape
    }

    Vertex calculateCentroid() const {
        float cx = 0.0f, cy = 0.0f, cz = 0.0f;
        int n = vertices.size();
        for (const auto& vertex : vertices) {
            cx += vertex.x;
            cy += vertex.y;
            cz += vertex.z;
        }
        return Vertex(cx / n, cy / n, cz / n);
    }

    // Method to export vertices to a file (same as previous version)
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


// Function to read vertices from a file (same as previous version)

// Main function (similar to previous versions but with additional property calculation options)
int main() {
    int loadChoice;
    bool is2D = false;
    std::vector<Vertex> vertices;

    // Prompt user for loading or creating an object
    // (same as previous version)
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

    // User-selected operations
    char continueChoice;
    do {
        int operationChoice;
        std::cout << "Choose an operation:\n";
        std::cout << "1. Translate\n";
        std::cout << "2. Scale\n";
        std::cout << "3. Rotate\n";
        std::cout << "4. Calculate Geometric Properties\n";
        std::cout << "Enter your choice (1-4): ";
        std::cin >> operationChoice;

        if (std::cin.fail() || operationChoice < 1 || operationChoice > 4) {
            clearInputStream();
            std::cout << "Invalid operation choice. Please select a valid option.\n";
            continue; // Ask for the operation again
        }

        // Handling transformation operations and property calculations
        if (operationChoice == 4) {
            if (is2D) {
                Shape2D shape2D;
                shape2D.vertices = vertices;
                float area = shape2D.calculateArea();
                float perimeter = shape2D.calculatePerimeter();
                Vertex centroid = shape2D.calculateCentroid();

                std::cout << "2D Shape Properties:\n";
                std::cout << "Area: " << area << "\n";
                std::cout << "Perimeter: " << perimeter << "\n";
                std::cout << "Centroid: (" << centroid.x << ", " << centroid.y << ")\n";

                // Export properties to file
                shape2D.exportToFile("shape2D_properties.txt");

                // Call Python script for further visualization
                system("python3 visualize_2d.py shape2D_properties.txt");

            } else {
                Shape3D shape3D;
                shape3D.vertices = vertices;
                float volume = shape3D.calculateVolume();
                float surfaceArea = shape3D.calculateSurfaceArea();
                Vertex centroid = shape3D.calculateCentroid();

                std::cout << "3D Shape Properties:\n";
                std::cout << "Volume: " << volume << "\n";
                std::cout << "Surface Area: " << surfaceArea << "\n";
                std::cout << "Centroid: (" << centroid.x << ", " << centroid.y << ", " << centroid.z << ")\n";

                // Export properties to file
                shape3D.exportToFile("shape3D_properties.txt");

                // Call Python script for further visualization
                system("python3 visualize_3d.py shape3D_properties.txt");
            }
        } else {
            // Handle translation, scaling, and rotation (similar to previous versions)
        }

        std::cout << "Do you want to perform another operation? (Y/N): ";
        std::cin >> continueChoice;
    } while (continueChoice == 'Y' || continueChoice == 'y');

    std::cout << "Exiting the program. Goodbye!\n";
    return 0;
}
