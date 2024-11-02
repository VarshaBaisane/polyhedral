#visualize load_3d_object

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np

# Load the data from the file
def load_3d_object(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    # Read vertices
    vertices = []
    i = 1
    while ' ' in lines[i].strip():
        vertices.append(list(map(float, lines[i].strip().split())))
        i += 1

    # Read number of edges and edges
    num_edges = int(lines[i].strip())
    edges = []
    i += 1
    for _ in range(num_edges):
        edges.append(tuple(map(int, lines[i].strip().split())))
        i += 1

    # Read number of faces and faces
    num_faces = int(lines[i].strip())
    faces = []
    i += 1
    for _ in range(num_faces):
        faces.append(tuple(map(int, lines[i].strip().split())))
    
    return np.array(vertices), edges, faces

# Rotation function
def rotate_vertices(vertices, angle_degrees, axis='z'):
    angle_radians = np.radians(angle_degrees)
    
    # Define rotation matrix for each axis
    if axis == 'z':
        rotation_matrix = np.array([
            [np.cos(angle_radians), -np.sin(angle_radians), 0],
            [np.sin(angle_radians), np.cos(angle_radians), 0],
            [0, 0, 1]
        ])
    elif axis == 'y':
        rotation_matrix = np.array([
            [np.cos(angle_radians), 0, np.sin(angle_radians)],
            [0, 1, 0],
            [-np.sin(angle_radians), 0, np.cos(angle_radians)]
        ])
    elif axis == 'x':
        rotation_matrix = np.array([
            [1, 0, 0],
            [0, np.cos(angle_radians), -np.sin(angle_radians)],
            [0, np.sin(angle_radians), np.cos(angle_radians)]
        ])
    else:
        raise ValueError("Invalid axis. Choose 'x', 'y', or 'z'.")
    
    # Apply rotation matrix to each vertex
    rotated_vertices = vertices @ rotation_matrix.T
    return rotated_vertices

# Custom function to set an equal aspect ratio in 3D
def set_axes_equal(ax):
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

# Visualize the rotated 3D object
def plot_3d_object(vertices, edges, faces):
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
filename = 'vertices3d.txt'
vertices, edges, faces = load_3d_object(filename)

# Rotate vertices by 45 degrees around the z-axis
rotated_vertices = rotate_vertices(vertices, 45, axis='z')

# Plot rotated object
plot_3d_object(rotated_vertices, edges, faces)
