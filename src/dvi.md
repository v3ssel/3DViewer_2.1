# 3DViewer 2.0

## Info
A simple program for viewing wireframe models from .obj files

Created with Qt + OpenGL, QtGifImage is taken from [here](https://github.com/dbzhang800/QtGifImage)

## Dependencies
1. Qt6.2 + QtOpenGLWidgets + OpenGL
2. g++ + g++
3. Make and CMake


## Install
1. Download the repository
2. cd to src folder
3. Type `make install`
4. Go to Desktop and run program **3DViewer 2.0**

## Uninstall
Go to src folder and type `make uninstall` or just delete app icon

## Usage
* To open a *.obj file, go to File -> Open or Press **⌘1** and open the desired file.
* To close a file, go to the tab File -> Close or Press **⌘2** and the object will be deleted.
* To see information about the object, you need to go to the tab File -> Info or Press **I**, after which the information about the current object will be displayed.
* To hide the side window, go to Settings -> Hide/Show or Press **⌘3**, then the side window will be closed or opened if it was closed.
* To change the projection type, go to Settings -> (Press **O**)Orthographic/Perspective(Press **P**), then the projection type will be changed.
* To make a screenshot, go to the Create -> Image tab, then select the format of the screenshot and save it.
* To make a gif-animation, you need to go to the tab Create -> GIF, after which the main window shows a red frame, indicating that the recording is in progress. After 5 seconds, the recording will be saved(10fps).
* To set the color of the lines, click the **Set Lines Color** button and select the desired color.
* To change the thickness of the lines, move the **Line Width** slider to the right to increase and to the left to decrease.
* To draw a line with dashed lines, click the **Dashed** button, and to draw a solid line, click the **Solid** button.
* To change the color of the vertices, click the **Set Vertex Color** button and select the desired color.
* To change the size of the vertices, slide the **Vertex Size** slider to the right to increase and to the left to decrease.
* To remove the display of vertices, click the **None** button.
* To draw the vertices as a circle, click the **Circle** button.
* To draw the vertices as a square, click the **Square** button.
* To rotate the figure relative to the X, Y, Z axes, hover over the desired coordinate in the **Rotate** section and change it with the mouse wheel.
* To move the figure relative to the X, Y, Z axes, hover over the desired coordinate in the **Move** section and change it with the mouse wheel.
* To resize the shape, move the **Scale** slider to the right to zoom in, or hover the mouse over the spinbox to zoom out.
* To change the background color, click the **Set Background Color** button, select the desired color, and then the background will change.

## Created and tested on MacOS by v3ssel, madamkyl and bailagda in 2022
