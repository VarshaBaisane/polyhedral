import matplotlib.pyplot as plt

def main():
    # Ask the user for the filename
    filename = input("Enter the name of the file to visualize (including extension): ")
    vertices = []

    # Read the vertices from the specified file
    try:
        with open(filename, 'r') as file:
            file.readline()  # Skip the first line (e.g., 2D)
            for line in file:
                # Split the line into coordinates
                coords = list(map(float, line.split()))
                vertices.append((coords[0], coords[1]))

    except FileNotFoundError:
        print(f"Error: The file '{filename}' was not found.")
        return

    except Exception as e:
        print(f"An error occurred: {e}")
        return

    # Unzip the list of vertices into x and y coordinates
    x, y = zip(*vertices)

    # Plotting the 2D shape
    plt.figure()
    plt.plot(x, y, 'o-')  # Plot vertices with lines connecting them
    plt.title('2D Shape Visualization')
    plt.xlabel('X-axis')
    plt.ylabel('Y-axis')
    plt.axis('equal')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    main()
