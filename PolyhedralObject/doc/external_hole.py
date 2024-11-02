import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np

# Define vertices, edges, faces for a 1x1x1 meter cube centered at the origin
vertices = np.array([
    [-0.5, -0.5, -0.5],
    [0.5, -0.5, -0.5],
    [0.5, 0.5, -0.5],
    [-0.5, 0.5, -0.5],
    [-0.5, -0.5, 0.5],
    [0.5, -0.5, 0.5],
    [0.5, 0.5, 0.5],
    [-0.5, 0.5, 0.5]
])

edges = [
    (0, 1), (1, 2), (2, 3), (3, 0),  # Bottom face edges
    (4, 5), (5, 6), (6, 7), (7, 4),  # Top face edges
    (0, 4), (1, 5), (2, 6), (3, 7)   # Vertical edges
]

faces = [
    [0, 1, 2, 3],  # Bottom face
    [4, 5, 6, 7],  # Top face
    [0, 1, 5, 4],  # Side face
    [1, 2, 6, 5],
    [2, 3, 7, 6],
    [3, 0, 4, 7]
]

# Hole parameters
hole_diameter = 0.8
hole_depth = 0.6
external_depth = 0.25  # Part of the hole outside the cube
internal_depth = hole_depth - external_depth  # Remaining depth inside the cube

# Target face index (e.g., the top face of the cube)
hole_face_index = 1
face_vertices = vertices[faces[hole_face_index]]

# Calculate the center of the specified face
hole_center = face_vertices.mean(axis=0)

# Starting point of the hole: offset from the face center to extend outside
hole_start = hole_center + np.array([0, 0, external_depth])

# Visualization
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Draw the cube as a transparent object
for face in faces:
    poly = [vertices[face]]
    ax.add_collection3d(Poly3DCollection(poly, color='cyan', edgecolor='k', alpha=0.3))

# Draw edges of the cube
for edge in edges:
    ax.plot([vertices[edge[0]][0], vertices[edge[1]][0]],
            [vertices[edge[0]][1], vertices[edge[1]][1]],
            [vertices[edge[0]][2], vertices[edge[1]][2]], 'k')

# Define hole parameters for a centered, cylindrical hole
hole_steps = 30
theta = np.linspace(0, 2 * np.pi, hole_steps)
hole_radius = hole_diameter / 2

# Calculate hole coordinates for the top (outside) and bottom (inside) of the hole cylinder
hole_x = hole_center[0] + hole_radius * np.cos(theta)
hole_y = hole_center[1] + hole_radius * np.sin(theta)
hole_z_outside = np.full(hole_steps, hole_start[2])
hole_z_inside = np.full(hole_steps, hole_start[2] - hole_depth)

# Plot the hole as a cylinder with part extending outside the cube
for i in range(hole_steps - 1):
    # Draw circular edges at the top and bottom of the hole
    ax.plot([hole_x[i], hole_x[i + 1]], [hole_y[i], hole_y[i + 1]], [hole_z_outside[i], hole_z_outside[i + 1]], 'b')
    ax.plot([hole_x[i], hole_x[i + 1]], [hole_y[i], hole_y[i + 1]], [hole_z_inside[i], hole_z_inside[i + 1]], 'b')
    # Draw vertical lines connecting top and bottom circles of the hole
    ax.plot([hole_x[i], hole_x[i]], [hole_y[i], hole_y[i]], [hole_z_outside[i], hole_z_inside[i]], 'b')

# Set labels and display
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_box_aspect([1, 1, 1])
ax.set_title('3D Shape with External Hole (Centered)')

plt.show()
