#include <iostream>
#include <vector>
#include <fstream>
#include <limits> // For input validation

// Define Vertex class
class Vertex {
public:
    float x, y, z;
    Vertex(float xCoord, float yCoord, float zCoord = 0.0f)
        : x(xCoord), y(yCoord), z(zCoord) {}

    void print() const {
        std::cout << "Vertex(" << x << ", " << y << ", " << z << ")\n";
    }
};

// Define Edge class
class Edge {
public:
    int startVertex, endVertex;
    Edge(int start, int end) : startVertex(start), endVertex(end) {}

    void print() const {
        std::cout << "Edge(" << startVertex << ", " << endVertex << ")\n";
    }
};

// Define Face class
class Face {
public:
    std::vector<int> vertices;
    Face(const std::vector<int>& vertices) : vertices(vertices) {}

    void print() const {
        std::cout << "Face(";
        for (size_t i = 0; i < vertices.size(); ++i) {
            std::cout << vertices[i];
            if (i < vertices.size() - 1) std::cout << ", ";
        }
        std::cout << ")\n";
    }
};

// Define Shape2D class
class Shape2D {
public:
    std::vector<Vertex> vertices;

    void addVertex(float x, float y) {
        vertices.emplace_back(x, y, 0.0f); // z is zero for 2D objects
    }

    void printVertices() const {
        std::cout << "2D Shape Vertices:\n";
        for (const auto& vertex : vertices) {
            vertex.print();
        }
    }

    const std::vector<Vertex>& getVertices() const {
        return vertices;
    }
};

// Define Shape3D class
class Shape3D {
public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;

    void addVertex(float x, float y, float z) {
        vertices.emplace_back(x, y, z);
    }

    void addEdge(int start, int end) {
        edges.emplace_back(start, end);
    }

    void addFace(const std::vector<int>& faceVertices) {
        faces.emplace_back(faceVertices);
    }

    void printShape() const {
        std::cout << "3D Shape Vertices:\n";
        for (const auto& vertex : vertices) {
            vertex.print();
        }

        std::cout << "3D Shape Edges:\n";
        for (const auto& edge : edges) {
            edge.print();
        }

        std::cout << "3D Shape Faces:\n";
        for (const auto& face : faces) {
            face.print();
        }
    }

    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<Edge>& getEdges() const { return edges; }
    const std::vector<Face>& getFaces() const { return faces; }
};

// Function to export vertices, edges, and faces to a file
void exportToFile(const Shape3D& shape, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error creating file!" << std::endl;
        return;
    }

    file << "3D\n"; // Indicate a 3D object

    // Write vertices
    file << "Vertices:\n";
    for (const auto& vertex : shape.getVertices()) {
        file << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }

    // Write edges
    file << "Edges:\n";
    for (const auto& edge : shape.getEdges()) {
        file << edge.startVertex << " " << edge.endVertex << "\n";
    }

    // Write faces
    file << "Faces:\n";
    for (const auto& face : shape.getFaces()) {
        for (size_t i = 0; i < face.vertices.size(); ++i) {
            file << face.vertices[i];
            if (i < face.vertices.size() - 1) file << " ";
        }
        file << "\n";
    }

    file.close();
    std::cout << "3D shape data exported to " << filename << std::endl;
}

// Function to validate float input
bool getFloatInput(float& input) {
    std::cin >> input;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

void getVertex3D(float& x, float& y, float& z) {
    while (true) {
        std::cout << "Enter coordinates (x y z): ";
        if (getFloatInput(x) && getFloatInput(y) && getFloatInput(z)) {
            break;
        }
        std::cout << "Invalid input. Please enter numeric values for (x y z).\n";
    }
}

int main() {
    int choice;
    std::cout << "Do you want to create a 2D or 3D object?\n";
    std::cout << "1. 2D\n";
    std::cout << "2. 3D\n";
    std::cout << "Enter your choice (1 or 2): ";
    std::cin >> choice;

    if (choice == 1) {
        Shape2D shape2D;
        int numVertices;
        std::cout << "Enter the number of vertices for your 2D object: ";
        std::cin >> numVertices;

        for (int i = 0; i < numVertices; ++i) {
            float x, y;
            std::cout << "Enter coordinates (x y): ";
            getFloatInput(x);
            getFloatInput(y);
            shape2D.addVertex(x, y);
        }

        std::cout << "2D object created with the following vertices:\n";
        shape2D.printVertices();
    } else if (choice == 2) {
        Shape3D shape3D;
        int numVertices, numEdges, numFaces;

        std::cout << "Enter the number of vertices for your 3D object: ";
        std::cin >> numVertices;

        for (int i = 0; i < numVertices; ++i) {
            float x, y, z;
            getVertex3D(x, y, z);
            shape3D.addVertex(x, y, z);
        }

        std::cout << "Enter the number of edges for your 3D object: ";
        std::cin >> numEdges;
        for (int i = 0; i < numEdges; ++i) {
            int start, end;
            std::cout << "Enter edge (start end): ";
            std::cin >> start >> end;
            shape3D.addEdge(start, end);
        }

        std::cout << "Enter the number of faces for your 3D object: ";
        std::cin >> numFaces;
        for (int i = 0; i < numFaces; ++i) {
            int numFaceVertices;
            std::cout << "Enter number of vertices for face " << i + 1 << ": ";
            std::cin >> numFaceVertices;
            std::vector<int> faceVertices(numFaceVertices);
            std::cout << "Enter vertices indices for the face: ";
            for (int& vertex : faceVertices) {
                std::cin >> vertex;
            }
            shape3D.addFace(faceVertices);
        }

        std::cout << "3D object created with the following components:\n";
        shape3D.printShape();

        // Export the 3D object data to file
        exportToFile(shape3D, "3D_object_data.txt");
    } else {
        std::cout << "Invalid choice. Please restart the program.\n";
    }

    return 0;
}


/*
8  // Number of vertices
-0.5 -0.5 -0.5
0.5 -0.5 -0.5
0.5 0.5 -0.5
-0.5 0.5 -0.5
-0.5 -0.5 0.5
0.5 -0.5 0.5
0.5 0.5 0.5
-0.5 0.5 0.5

12  // Number of edges
0 1
1 2
2 3
3 0
4 5
5 6
6 7
7 4
0 4
1 5
2 6
3 7

6  // Number of faces
4 0 1 2 3  // Bottom face
4 4 5 6 7  // Top face
4 0 1 5 4  // Front face
4 1 2 6 5  // Right face
4 2 3 7 6  // Back face
4 3 0 4 7  // Left face
*/
