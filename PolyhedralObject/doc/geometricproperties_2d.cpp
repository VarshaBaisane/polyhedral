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

    void addVertex(float x, float y) {
        vertices.emplace_back(x, y);
    }

    void printVertices() const {
        std::cout << "2D Shape Vertices:\n";
        for (const auto& vertex : vertices) {
            vertex.print();
        }
    }

    // Calculate area using the Shoelace formula
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
        return std::abs(area) / 2.0f;
    }

    // Calculate centroid
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
        return Vertex(cx, cy);
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
        file.close();
        std::cout << "Vertices exported to " << filename << " successfully!\n";
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
    std::vector<Vertex> vertices;

    std::cout << "Load an object from file (1) or create a new object (2)? ";
    std::cin >> loadChoice;

    if (loadChoice == 1) {
        std::string filename;
        std::cout << "Enter filename: ";
        std::cin >> filename;

        if (!readVerticesFromFile(filename, vertices)) {
            std::cerr << "Error loading file. Exiting." << std::endl;
            return 1;
        }
    } else {
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
        vertices = shape2D.vertices;
        shape2D.exportToFile("new_2d_object.txt");
    }

    Shape2D shape;
    shape.vertices = vertices;

    int propertyChoice;
    std::cout << "Select property to calculate:\n";
    std::cout << "1. Area\n";
    std::cout << "2. Centroid\n";
    std::cout << "Enter choice: ";
    std::cin >> propertyChoice;

    if (propertyChoice == 1) {
        float area = shape.calculateArea();
        std::cout << "Area: " << area << "\n";
    } else if (propertyChoice == 2) {
        Vertex centroid = shape.calculateCentroid();
        std::cout << "Centroid: (" << centroid.x << ", " << centroid.y << ")\n";
    } else {
        std::cout << "Invalid choice!" << std::endl;
    }

    return 0;
}

// Area example: Vertex A: (1, 2)
// Vertex B: (4, 5)
// sVertex C: (7, 2)

//centroid calculation


//perimeter calculation

//file:
//2D
//1 2
//4 5
//7 2
