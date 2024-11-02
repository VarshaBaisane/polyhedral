import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection

# Define vertices, edges, and faces of the cube
vertices = np.array([
    [0.5, -0.5, -0.5], [0.5, 0.5, -0.5], [-0.5, 0.5, -0.5],
    [0.5, -0.5, 0.5], [0.5, 0.5, 0.5], [-0.5, 0.5, 0.5]
])

edges = [(0, 1), (1, 2), (3, 4), (4, 5), (0, 3), (1, 4), (2, 5)]

faces = [
    [0, 1, 2], [3, 4, 5], [0, 3], [0, 1, 4, 3], [1, 2, 5, 4], [2, 5]
]

# Define the hole parameters
hole_diameter = 1.13137  # transformed major axis diameter
hole_depth_inward = 0.3   # depth inside the cube
hole_offset_outward = 0.25  # offset above the cube's top face

# Function to plot the cube
def plot_cube(ax, vertices, edges, faces, color='cyan'):
    # Draw edges
    for edge in edges:
        points = vertices[np.array(edge)]
        ax.plot(points[:, 0], points[:, 1], points[:, 2], color="k")

    # Draw faces
    for face in faces:
        face_vertices = vertices[face]
        poly = Poly3DCollection([face_vertices], color=color, alpha=0.5)
        ax.add_collection3d(poly)

# Function to plot the hole as a partial ellipse with depth
def plot_partial_ellipse_with_depth(ax, center, diameter, depth_inward, offset_outward):
    # Calculate elliptical parameters
    major_axis = diameter / 2
    minor_axis = major_axis * np.cos(np.radians(45))  # Adjusted by the angle of cut

    # Create the partial ellipse (arc) for the external part of the hole above the surface
    theta = np.linspace(-np.pi / 2, np.pi / 2, 100)
    x = center[0] + major_axis * np.cos(theta)
    y = center[1] + minor_axis * np.sin(theta)
    z = np.full_like(x, center[2])  # Offset outward from the cube's top face

    # Plot partial ellipse above the surface
    ax.plot(x, y, z, color='red', linewidth=2, label="Hole Cross-section Above Cube")

    # Plot the internal depth of the hole within the cube
    z_depth = np.linspace(center[2], center[2] - depth_inward, 50)
    x_depth_in = center[0] * np.ones_like(z_depth)
    y_depth_in = center[1] * np.ones_like(z_depth)

    ax.plot(x_depth_in, y_depth_in, z_depth, color="red", linestyle="--", linewidth=1.5, label="Hole Depth Inside Cube")

    # Bottom internal ellipse (inside the cube at the end of the hole's depth)
    x_in = center[0] + major_axis * np.cos(theta)
    y_in = center[1] + minor_axis * np.sin(theta)
    z_in = np.full_like(x_in, center[2] - depth_inward)

    # Plot internal end of the hole as a partial ellipse
    ax.plot(x_in, y_in, z_in, color='gray', linewidth=2, label="Internal End of Hole")

# Set up the plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_box_aspect([1, 1, 1])

# Plot original cube
plot_cube(ax, vertices, edges, faces)

# Define hole center and adjust its z-coordinate to sit above the top face
top_face_center = np.array([0, 0, 0.5])  # Center of the top face
hole_center = top_face_center + np.array([0, 0, hole_offset_outward])

# Plot the hole as a partial ellipse with depth
plot_partial_ellipse_with_depth(ax, hole_center, hole_diameter, hole_depth_inward, hole_offset_outward)

# Set axis labels and display
ax.set_xlabel("X-axis")
ax.set_ylabel("Y-axis")
ax.set_zlabel("Z-axis")
ax.legend()
plt.show()
