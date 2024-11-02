#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include <limits>

// Vertex class
class Vertex {
public:
    float x, y;
    Vertex(float xCoord = 0, float yCoord = 0)
        : x(xCoord), y(yCoord) {}

    void print() const {
        std::cout << "Vertex(" << x << ", " << y << ")\n";
    }
};

// Shape2D class
class Shape2D {
public:
    std::vector<Vertex> vertices;
    std::vector<float> holeDiameters; // Store diameters for circular holes

    void addVertex(float x, float y) {
        vertices.emplace_back(x, y);
    }

    void addCircularHole(float diameter) {
        holeDiameters.push_back(diameter);
    }

    void printVertices() const {
        std::cout << "2D Shape Vertices:\n";
        for (const auto& vertex : vertices) {
            vertex.print();
        }
    }

    float calculateArea() const {
        if (vertices.size() < 3) {
            std::cout << "Not enough vertices to form a closed shape.\n";
            return 0.0f;
        }
        float area = 0.0f;
        int n = vertices.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            area += (vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y);
        }
        area = std::abs(area) / 2.0f;

        // Subtract areas of holes
        for (const auto& diameter : holeDiameters) {
            float radius = diameter / 2.0f;
            area -= M_PI * radius * radius; // Area of the circular hole
        }

        return area;
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

        // Add perimeters of circular holes
        for (const auto& diameter : holeDiameters) {
            float radius = diameter / 2.0f;
            perimeter += 2 * M_PI * radius; // Circumference of the circular hole
        }

        return perimeter;
    }

    Vertex calculateCentroid() const {
        if (vertices.size() < 3) {
            std::cout << "Not enough vertices to form a closed shape.\n";
            return Vertex(0, 0);
        }
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
        signedArea *= 0.5f;
        cx = (1.0f / (6.0f * signedArea)) * cx;
        cy = (1.0f / (6.0f * signedArea)) * cy;

        return Vertex(cx / signedArea, cy / signedArea);
    }

    void exportToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error opening file for writing!" << std::endl;
            return;
        }
        file << "2D\n";
        for (const auto& vertex : vertices) {
            file << vertex.x << " " << vertex.y << "\n";
        }

        // Export holes
        for (const auto& diameter : holeDiameters) {
            file << "Hole Diameter: " << diameter << "\n"; // Indicate the diameter of the hole
        }

        file.close();
        std::cout << "Vertices and holes exported to " << filename << " successfully!\n";
    }

    // Create a circular hole based on position and diameter
    void createCircularHole(const std::string& position, float diameter) {
        addCircularHole(diameter); // Just store the diameter for circular holes
    }
};

// Function to read vertices from a file
bool readVerticesFromFile(const std::string& filename, std::vector<Vertex>& vertices) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
    }

    std::string objectType;
    file >> objectType;
    if (objectType != "2D") {
        std::cerr << "Invalid file format! Expected '2D' at the beginning." << std::endl;
        return false;
    }

    float x, y;
    while (file >> x >> y) {
        vertices.emplace_back(x, y);
    }

    file.close();
    return true;
}

int main() {
    int loadChoice;
    Shape2D shape2D;

    std::cout << "Load an object from file (1) or create a new object (2)? ";
    std::cin >> loadChoice;

    if (loadChoice == 1) {
        std::string filename;
        std::cout << "Enter filename: ";
        std::cin >> filename;

        std::vector<Vertex> vertices;
        if (!readVerticesFromFile(filename, vertices)) {
            std::cerr << "Error loading file. Exiting." << std::endl;
            return 1;
        }
        shape2D.vertices = vertices;
    } else {
        int numVertices;
        std::cout << "Enter the number of vertices: ";
        std::cin >> numVertices;

        for (int i = 0; i < numVertices; ++i) {
            float x, y;
            std::cout << "Enter coordinates (x y) for vertex " << (i + 1) << " (in meters): ";
            std::cin >> x >> y;
            shape2D.addVertex(x, y);
        }
        shape2D.exportToFile("new_2d_object.txt");
    }

    // Ask if there are holes
    char hasHoles;
    std::cout << "Does the shape have holes? (y/n): ";
    std::cin >> hasHoles;

    if (hasHoles == 'y' || hasHoles == 'Y') {
        int numHoles;
        std::cout << "Enter the number of holes: ";
        std::cin >> numHoles;

        for (int i = 0; i < numHoles; ++i) {
            std::string position;
            float diameter;
            std::cout << "Enter position for hole " << (i + 1) << " (left, center, right): ";
            std::cin >> position;
            std::cout << "Enter diameter for hole " << (i + 1) << " (in meters): ";
            std::cin >> diameter;

            shape2D.createCircularHole(position, diameter);
        }
    }

    // Export after holes are specified
    shape2D.exportToFile("shape_with_holes.txt");

    int propertyChoice;
    std::cout << "Select property to calculate:\n";
    std::cout << "1. Area\n";
    std::cout << "2. Perimeter\n";
    std::cout << "3. Centroid\n";
    std::cout << "Enter choice: ";
    std::cin >> propertyChoice;

    if (propertyChoice == 1) {
        float area = shape2D.calculateArea();
        std::cout << "Area: " << area << " square meters\n";
    } else if (propertyChoice == 2) {
        float perimeter = shape2D.calculatePerimeter();
        std::cout << "Perimeter: " << perimeter << " meters\n";
    } else if (propertyChoice == 3) {
        Vertex centroid = shape2D.calculateCentroid();
        std::cout << "Centroid: (" << centroid.x << " m, " << centroid.y << " m)\n";
    } else {
        std::cout << "Invalid choice!" << std::endl;
    }

    return 0;
}
