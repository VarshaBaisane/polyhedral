#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>

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

// Define Hole class
class Hole {
public:
    std::string location;       // internal or external
    std::string position;       // left, center, or right
    int face;                   // Face number
    float diameter;
    float depth;
    float startDepthFromSurface; // Starting depth from object surface

    Hole(const std::string& location, const std::string& position, int face, float diameter, float depth, float startDepthFromSurface)
        : location(location), position(position), face(face), diameter(diameter), depth(depth), startDepthFromSurface(startDepthFromSurface) {}

    void print() const {
        std::cout << "Hole Location: " << location << ", Position: " << position 
                  << ", Face: " << face << ", Diameter: " << diameter 
                  << ", Depth: " << depth << ", Start Depth from Surface: " 
                  << startDepthFromSurface << "\n";
    }
};

// Define Shape3D class
class Shape3D {
public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;
    std::vector<Hole> holes;

    void addVertex(float x, float y, float z) {
        vertices.emplace_back(x, y, z);
    }

    void addEdge(int start, int end) {
        edges.emplace_back(start, end);
    }

    void addFace(const std::vector<int>& faceVertices) {
        faces.emplace_back(faceVertices);
    }

    void addHole(const std::string& location, const std::string& position, int face, float diameter, float depth, float startDepthFromSurface) {
        holes.emplace_back(location, position, face, diameter, depth, startDepthFromSurface);
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

        std::cout << "Holes in Shape:\n";
        for (const auto& hole : holes) {
            hole.print();
        }
    }
};

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

// Function to get hole details
void getHoleDetails(Shape3D& shape) {
    int numHoles;
    std::cout << "Does this object have any holes? (yes/no): ";
    std::string hasHole;
    std::cin >> hasHole;

    if (hasHole == "yes" || hasHole == "Yes") {
        std::cout << "How many holes does the object have? ";
        std::cin >> numHoles;

        for (int i = 0; i < numHoles; ++i) {
            std::string location, position;
            float diameter, depth, startDepthFromSurface;
            int face;

            std::cout << "\nEnter details for hole " << i + 1 << ":\n";
            std::cout << "Is this hole external or internal? ";
            std::cin >> location;

            std::cout << "Enter the position of the hole (left, center, or right): ";
            std::cin >> position;

            std::cout << "Enter the face number where this hole is located: ";
            std::cin >> face;

            std::cout << "Enter the diameter of the hole: ";
            getFloatInput(diameter);

            std::cout << "Enter the depth of the hole: ";
            getFloatInput(depth);

            std::cout << "Enter the start depth of the hole from the object's surface: ";
            getFloatInput(startDepthFromSurface);

            shape.addHole(location, position, face, diameter, depth, startDepthFromSurface);
        }
    }
}

// Function to import shape from file
bool importFromFile(Shape3D& shape, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
    }

    std::string line;

    // Read vertices
    while (std::getline(file, line) && line != "Vertices:");
    while (std::getline(file, line) && line != "Edges:") {
        float x, y, z;
        std::istringstream iss(line);
        iss >> x >> y >> z;
        shape.addVertex(x, y, z);
    }

    // Read edges
    while (std::getline(file, line) && line != "Faces:") {
        int start, end;
        std::istringstream iss(line);
        iss >> start >> end;
        shape.addEdge(start, end);
    }

    // Read faces
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int numVertices, vertexIndex;
        iss >> numVertices;
        std::vector<int> faceVertices(numVertices);
        for (int i = 0; i < numVertices; ++i) {
            iss >> vertexIndex;
            faceVertices[i] = vertexIndex;
        }
        shape.addFace(faceVertices);
    }

    file.close();
    return true;
}

// Function to export shape with holes to file
void exportToFile(const Shape3D& shape, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error creating file!" << std::endl;
        return;
    }

    file << "Vertices:\n";
    for (const auto& vertex : shape.vertices) {
        file << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }

    file << "Edges:\n";
    for (const auto& edge : shape.edges) {
        file << edge.startVertex << " " << edge.endVertex << "\n";
    }

    file << "Faces:\n";
    for (const auto& face : shape.faces) {
        file << face.vertices.size();
        for (const auto& vertex : face.vertices) {
            file << " " << vertex;
        }
        file << "\n";
    }

    file << "Holes:\n";
    for (const auto& hole : shape.holes) {
        file << "Location: " << hole.location << ", Position: " << hole.position
             << ", Face: " << hole.face << ", Diameter: " << hole.diameter 
             << ", Depth: " << hole.depth << ", Start Depth from Surface: " 
             << hole.startDepthFromSurface << "\n";
    }

    file.close();
    std::cout << "Shape and holes exported to " << filename << "\n";
}

int main() {
    Shape3D shape3D;
    std::string filename;

    std::cout << "Would you like to import a file with the 3D object structure? (yes/no): ";
    std::string choice;
    std::cin >> choice;

    if (choice == "yes" || choice == "Yes") {
        std::cout << "Enter the filename to import the 3D object: ";
        std::cin >> filename;

        if (importFromFile(shape3D, filename)) {
            std::cout << "Imported 3D shape:\n";
            shape3D.printShape();
        } else {
            std::cerr << "Failed to import 3D shape from " << filename << ".\n";
            return 1;
        }
    }

    getHoleDetails(shape3D);

    std::cout << "\nFinal shape details after including holes:\n";
    shape3D.printShape();

    std::cout << "Enter the filename to export the 3D object with holes: ";
    std::cin >> filename;
    exportToFile(shape3D, filename);

    return 0;
}
