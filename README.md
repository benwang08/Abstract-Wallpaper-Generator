# Wallpaper Generator
Description:  
---
Generate cool abstract wallpaper from an input image.
  
Requirements:  
---
  * g++ v9 or newer
  * make

Instructions:  
---
Add an input image to the folder (ex. "INPUT_IMG.jpg")

Program reads and writes in .ppm format. Imagemagick can be used to easily convert to P3 ppm:
```
  $ sudo apt install imagemagick
  $ convert INPUT_IMG.jpg -compress none INPUT_IMG.ppm
```
To run: 
```
  $ make all
```
Scan takes in 3 arguments (img source, output wallpaper width, output wallpaper height) and produces two files ("color_palette.ppm" and "data.txt")
Example with output image resolution 1920x1080:
```
  $ ./scan INPUT_IMG.ppm 1920 1080
```
Generator takes in 0 arguments and uses created "data.txt" to render a random abstract wallpaper
Produces file called "Wallpaper.ppm"
```
  $ ./generator
```



