This program is a robot that navigates a grid using a depth-first algorithm.
I have attempted to complete all six stages.

The graphical representation of the robot navigating is shown by:
- a grid of the given dimensions
- a green triangle: robot
- a blue square: home (starting) square
- grey squares: collectible markers
- black squares: immovable blocks

To compile and run the program:
- gcc main.c
- ./a.out <start x position> <start y position> <start angle in degrees> | java -jar drawapp-3.0.jar
    - Note that:
        - the top left square of the grid is the origin position (0,0).
        - 0 degrees = facing north, 90 degrees = facing east, 180 degrees = facing south, 270 degrees = facing west
    - e.g. "./a.out 4 1 90 | java -jar drawapp-3.0.jar" will initialise the program with:
        - the robot beginning its search from the position (4,1)
        - said robot facing an angle of 90 degrees. 

The program finds every marker in the grid, and the robot stores it. When it returns home, it prints the amount of markers it found and then each marker location.