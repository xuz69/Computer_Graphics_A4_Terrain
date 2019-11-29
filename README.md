CS3GC3 Assignment4 (Work in pair)

1. Members
    1. Amy Xu       [400079409]
    2. Baikai Wang  [400084727]

How To Run:
1. Open terminal
2. Go to the directory that contains all my files
3. run the command: make
4. The "Terrain" program should be running

The Operation Guide of the System:
1. Keyboard:
    1. Pressing 'q' or 'ESC' or 'Q' to exit the system.
    2. Pressing the 'W'(upper case latter) can toggle wireframe mode between three options (poly, wireframe or poly-wire).
    3. Pressing the 'S'(upper case latter) can toggle drawing the terrain using triangles or using quads.
    4. Pressing the ‘r’ or ‘R’ key to reset the terrain (will generate a new terrain).
    5. Pressing ‘l’ or ‘L’ will turn the lights on and off.
    6. Pressing 'c' or 'C' will change the algorithm of drawing terrain. Circle (by default) or Fault\n";
    7. Pressing 't' or 'T' will cycle drawing the terrain using the loaded textures. Three simple textures and one is complex (additional feature).
    8. Pressing 'f' or 'F' can toggle between flat shading and Gouraud shading.
    9. Press '0' to change the light mode to Light0, which means users can move the Light0 position using 'adsw'
    10. Press '1' to change the light mode to Light1, which means users can move the Light1 position using 'adsw'
    11. Press 'a' to change the light0(or 1) position along x axis (negative)
    12. Press 'd' to change the light0(or 1) position along x axis (positive)
    13. Press 'w' to change the light0(or 1) position along y axis (positive)
    14. Press 's' to change the light0(or 1) position along y axis (negative)
    15. Press right arrow button to rotate the camera position about y-axis in CW
    16. Press left arrow button to rotate the camera position about y-axis in CCW
    17. Press down arrow button to rotate the camera position about x-axis in CCW
    18. Press up arrow button to rotate the camera position about x-axis in CW
    19. Press 'm' or 'M' to turn on or off the Snowman character mode

3. Additonal Features:
    1. Elevation texturing:
        We developed an algorithm to achieve that different texture will be appiled to the terrian. The terrain is loaded with a surface terrain. For mountains after a certain point, grassy textures will be applied. And, for dips lower than “ground zero”, a water texture will be applied. And all the textures we used is from the previous requirment(Load textures and add texture key 't'/'T'). To get the elevation texturing stage, you should press the key 't' or 'T' four times.
    2. Add a character, Snowman:
        Press 'm' or 'M' to add a snow man into the scene, and it will moves around according to a random walk algorithm.
    3. Additional Terrain algorithm:
        We added Fault algorithm for generating terrain, ana you can press 'c' or 'C' to change the algorithm

4. Reference:
    - Terrain Tutorial: http://www.lighthouse3d.com/opengl/terrain/index.php?impdetails

Notes:
    My program can be compiled  and executed on GPU server, but some functions will lag in response by pressing a button. However, it works smoothly on my local server.

