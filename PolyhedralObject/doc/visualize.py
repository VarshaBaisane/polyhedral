import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def visualize_vertices(filename):
    with open(filename, 'r') as file:
        shape_type = file.readline().strip()
        vertices = []

        # Read vertices from file
        for line in file:
            x, y, z = map(float, line.split())
            vertices.append((x, y, z))

    if shape_type == "2D":
        # Plot 2D object
        plt.figure()
        x_coords = [v[0] for v in vertices]
        y_coords = [v[1] for v in vertices]
        plt.fill(x_coords, y_coords, edgecolor='r', fill=False)
        plt.scatter(x_coords, y_coords, color='b')
        plt.title('2D Object Visualization')
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.grid(True)
        plt.show()

    elif shape_type == "3D":
        # Plot 3D object
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        x_coords = [v[0] for v in vertices]
        y_coords = [v[1] for v in vertices]
        z_coords = [v[2] for v in vertices]
        
        # Plot points and lines connecting them (basic visualization)
        ax.plot_trisurf(x_coords, y_coords, z_coords, edgecolor='k', alpha=0.5)
        ax.scatter(x_coords, y_coords, z_coords, color='r', s=50)

        ax.set_title('3D Object Visualization')
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        plt.show()

# Visualize vertices from the file "vertices.txt"
visualize_vertices("vertices.txt")
