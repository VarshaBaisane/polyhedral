# visualize_2d.py

import matplotlib.pyplot as plt
filename=''
def read_vertices_from_file(filename):
    vertices = []
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith("Vertex"):
                parts = line.split("(")[1].split(")")[0].split(",")
                x, y = float(parts[0]), float(parts[1])
                vertices.append((x, y))
    return vertices

def plot_2d_shape(vertices):
    # Close the shape by appending the first vertex at the end
    vertices.append(vertices[0])
    
    # Unzip the vertices into X and Y coordinates
    x_coords, y_coords = zip(*vertices)

    # Plotting the shape
    plt.figure()
    plt.fill(x_coords, y_coords, 'b', alpha=0.3)  # Filling the shape with light blue color
    plt.plot(x_coords, y_coords, 'r')  # Plotting the edges with red color
    plt.scatter(x_coords, y_coords, color='red')  # Plotting the vertices

    # Setting plot properties
    plt.title('2D Shape Visualization')
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.grid(True)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()

def main():
    filename = "shape2D_properties.txt"  # The file exported from C++ code
    vertices = read_vertices_from_file(filename)
    
    if not vertices:
        print("No vertices found in the file.")
        return

    plot_2d_shape(vertices)

if __name__ == "__main__":
    main()
