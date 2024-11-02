# file created_cube.txt

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np

# Load the data from the file with debug statements
def load_3d_object(filename):
    vertices = []
    edges = []
    faces = []

    try:
        with open(filename, 'r') as file:
            lines = [line.strip() for line in file.readlines()]
        
        # Ensure the file starts with "3D"
        if lines[0] != "3D":
            raise ValueError("File does not start with '3D' header.")

        # Read vertices (skip "Vertices:" header)
        vertex_index = lines.index("Vertices:") + 1
        while lines[vertex_index] and lines[vertex_index] != "Edges:":
            vertices.append(list(map(float, lines[vertex_index].split())))
            vertex_index += 1

        # Read edges (skip "Edges:" header)
        edge_index = lines.index("Edges:") + 1
        while lines[edge_index] and lines[edge_index] != "Faces:":
            edges.append(tuple(map(int, lines[edge_index].split())))
            edge_index += 1

        # Read faces (skip "Faces:" header)
        face_index = lines.index("Faces:") + 1
        for line in lines[face_index:]:
            face_data = list(map(int, line.split()))
            faces.append(tuple(face_data[1:]))  # Skip the first integer (vertex count)

        return np.array(vertices), edges, faces

    except Exception as e:
        print(f"Error loading 3D object: {e}")
        return None, None, None

# Custom function to set an equal aspect ratio in 3D
def set_axes_equal(ax):
    '''Make the 3D plot axes have equal scale so the object doesn't look distorted.'''
    x_limits = ax.get_xlim3d()
    y_limits = ax.get_ylim3d()
    z_limits = ax.get_zlim3d()

    x_range = abs(x_limits[1] - x_limits[0])
    y_range = abs(y_limits[1] - y_limits[0])
    z_range = abs(z_limits[1] - z_limits[0])

    max_range = max(x_range, y_range, z_range)

    x_middle = np.mean(x_limits)
    y_middle = np.mean(y_limits)
    z_middle = np.mean(z_limits)

    ax.set_xlim3d([x_middle - max_range / 2, x_middle + max_range / 2])
    ax.set_ylim3d([y_middle - max_range / 2, y_middle + max_range / 2])
    ax.set_zlim3d([z_middle - max_range / 2, z_middle + max_range / 2])

# Visualize the 3D object with equal aspect ratio
def plot_3d_object(vertices, edges, faces):
    if vertices is None or edges is None or faces is None:
        print("Failed to load 3D object data.")
        return

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # Plot vertices
    ax.scatter(vertices[:, 0], vertices[:, 1], vertices[:, 2], color='black')

    # Plot edges
    for edge in edges:
        x = [vertices[edge[0]][0], vertices[edge[1]][0]]
        y = [vertices[edge[0]][1], vertices[edge[1]][1]]
        z = [vertices[edge[0]][2], vertices[edge[1]][2]]
        ax.plot(x, y, z, color='blue')

    # Plot faces
    face_vertices = [[vertices[face[i]] for i in range(len(face))] for face in faces]
    poly3d = Poly3DCollection(face_vertices, color='cyan', alpha=0.5, edgecolor='k')
    ax.add_collection3d(poly3d)

    # Set equal aspect ratio
    set_axes_equal(ax)

    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()

# Usage
filename = 'created_cube.txt'
vertices, edges, faces = load_3d_object(filename)
plot_3d_object(vertices, edges, faces)
