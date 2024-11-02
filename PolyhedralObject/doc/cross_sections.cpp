#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

struct Vertex {
    float x, y, z;
    Vertex(float x_ = 0, float y_ = 0, float z_ = 0) : x(x_), y(y_), z(z_) {}
};

struct Edge {
    int start, end;
};

struct Face {
    int numVertices;
    vector<int> vertices;
};

struct Hole {
    string location;
    string position;
    int face;
    float diameter;
    float depth;
    float startDepth;
    // Adjusted values for after the cut
    float effectiveDepth;
    float adjustedDiameter;
};

class Cube {
public:
    vector<Vertex> vertices;
    vector<Edge> edges;
    vector<Face> faces;
    Hole hole;

    Cube() {
        // Initialize vertices
        vertices = {
            Vertex(-0.5, -0.5, -0.5), Vertex(0.5, -0.5, -0.5), Vertex(0.5, 0.5, -0.5), Vertex(-0.5, 0.5, -0.5),
            Vertex(-0.5, -0.5, 0.5), Vertex(0.5, -0.5, 0.5), Vertex(0.5, 0.5, 0.5), Vertex(-0.5, 0.5, 0.5)
        };

        // Initialize edges
        edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };

        // Initialize faces
        faces = {
            {4, {0, 1, 2, 3}}, {4, {4, 5, 6, 7}},
            {4, {0, 1, 5, 4}}, {4, {1, 2, 6, 5}},
            {4, {2, 3, 7, 6}}, {4, {3, 0, 4, 7}}
        };

        // Initialize hole data
        hole.location = "external";
        hole.position = "center";
        hole.face = 2;            // Top face of the cube
        hole.diameter = 0.8;
        hole.depth = 0.6;
        hole.startDepth = -0.25;  // Above the top face of the cube
    }

    void visualizeCrossSection() {
        float angle;
        cout << "Enter the cutting angle from the center (in degrees): ";
        cin >> angle;
        cutAndStore(angle);
    }

    void cutAndStore(float angleDegrees) {
        float angleRadians = angleDegrees * M_PI / 180.0;
        Vertex normal(cos(angleRadians), sin(angleRadians), 0);  // Normal for plane rotated around Z-axis

        vector<Vertex> newVertices;
        vector<Edge> newEdges;
        vector<Face> newFaces;

        // Filter vertices based on position relative to the plane
        for (const auto& vertex : vertices) {
            float distance = vertex.x * normal.x + vertex.y * normal.y + vertex.z * normal.z;
            if (distance >= 0) {  // Retain vertices on or above the plane
                newVertices.push_back(vertex);
            }
        }

        // Re-map vertices to indices in the newVertices list
        vector<int> vertexMap(vertices.size(), -1);
        for (size_t i = 0; i < newVertices.size(); ++i) {
            for (size_t j = 0; j < vertices.size(); ++j) {
                if (newVertices[i].x == vertices[j].x && newVertices[i].y == vertices[j].y && newVertices[i].z == vertices[j].z) {
                    vertexMap[j] = i;
                    break;
                }
            }
        }

        // Update edges to retain only those within the new vertices
        for (const auto& edge : edges) {
            int newStart = vertexMap[edge.start];
            int newEnd = vertexMap[edge.end];
            if (newStart != -1 && newEnd != -1) {
                newEdges.push_back({newStart, newEnd});
            }
        }

        // Update faces similarly
        for (const auto& face : faces) {
            Face newFace;
            newFace.numVertices = 0;
            for (int vertexIndex : face.vertices) {
                int newIndex = vertexMap[vertexIndex];
                if (newIndex != -1) {
                    newFace.vertices.push_back(newIndex);
                    newFace.numVertices++;
                }
            }
            if (newFace.numVertices > 0) {
                newFaces.push_back(newFace);
            }
        }

        // Update hole data based on angle cut
        hole.effectiveDepth = hole.depth * cos(angleRadians);  // Adjust depth
        hole.adjustedDiameter = hole.diameter / cos(angleRadians);  // Elliptical major axis

        // Save result to file
        ofstream outputFile("cut_cube_data.txt");
        if (outputFile.is_open()) {
            outputFile << newVertices.size() << endl;
            for (const auto& v : newVertices) {
                outputFile << v.x << " " << v.y << " " << v.z << endl;
            }

            outputFile << newEdges.size() << endl;
            for (const auto& e : newEdges) {
                outputFile << e.start << " " << e.end << endl;
            }

            outputFile << newFaces.size() << endl;
            for (const auto& f : newFaces) {
                outputFile << f.numVertices;
                for (int vertexIndex : f.vertices) {
                    outputFile << " " << vertexIndex;
                }
                outputFile << endl;
            }

            outputFile << "Hole:" << endl;
            outputFile << hole.location << " " << hole.position << " " << hole.face << " "
                       << hole.adjustedDiameter << " " << hole.effectiveDepth << " " << hole.startDepth << endl;

            outputFile.close();
            cout << "Cut data saved to cut_cube_data.txt" << endl;
        } else {
            cerr << "Error: Could not open output file." << endl;
        }
    }
};

int main() {
    Cube cube;
    cube.visualizeCrossSection();

    return 0;
}
