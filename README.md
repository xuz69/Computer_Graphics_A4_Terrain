CS3GC3 Assignment4 (Work in pair)

1. Members
    1. Amy Xu       [400079409]
    2. Baikai Wang  [400084727]

How To Run:
1. Open terminal
2. Go to the directory that contains all my files
3. run the command: make
4. The "particles_fountain" program should be running

The Operation Guide of the System:
1. Keyboard:
    1. Pressing 'q' or 'ESC' or 'Q' to exit the system.
    2. Pressing the 'arrow keys' on the keyboard can rotate the camera.
    3. Pressing the 'W'(upper case latter) can toggle wireframe mode between three options.
    4. Pressing the 'S'(upper case latter) can toggle drawing the terrain using triangles or using quads.
    5. Pressing the ‘r’ or ‘R’ key can re-draw terrain.
    6. Pressing ‘l’ or ‘L’ will toggle the lights on and off.
    7. Pressing 'c' or 'C' will change the algorithm of drawing terrain.
    8. Pressing 't' or 'T' will cycle drawing the terrain using the loaded textures.
    9. Pressing 'f' or 'F' can toggle between flat shading and Gouraud shading.
    10. Pressing “0” to choose light 0, you can change the light 0 position
    11. Pressing “1” to choose light 1, you can change the light 1 position.
    12. Pressing (lowercase letter)'w', 's', 'a', or 'd' to move the light position. 

3. Additonal Features:
    1. Additional Terrain algorithm:
        We added Fault algorithm for our terrain, ana you can press 'c' or 'C' to change the algorithm
    2. Add a character:
        We added a snow man on the scene, and it moves around according to a random walk algorithm.
    3. Elevation texturing:
    We developed an algorithm to achieve that different texture will be appiled to the terrian. The terrain is loaded with a surface terrain. For mountains after a certain point, grassy textures will be applied. And, for dips lower than “ground zero”, a water texture will be applied. And all the textures we used is from the previous requirment(Load textures and add texture key 't'/'T'). To get the elevation texturing stage, you should press the key 't' or 'T' four times.
    
4. Reference:
Terrain Tutorial:
http://www.lighthouse3d.com/opengl/terrain/index.php?impdetails

