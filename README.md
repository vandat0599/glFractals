# glFractals
glFractals is a simple C++14 application to view fractals in realtime using 
OpenGL (>=3.3).

![Julia set demo](docs/julia.gif)

## Build
Build with your favorite CMake generator. For example (Linux):
```
git submodule update --init
mkdir build
cd build
cmake .. -G "Unix Makefiles"
make
```
Note: On Debian/Ubuntu, you may need libxorg-dev and libgl1-mesa-dev prior to running cmake for GLFW.
```
sudo apt-get install xorg-dev libx11-dev libgl1-mesa-dev
```
## Run
To view the Mandelbrot set:
```
./build/glFractals
```

To view the Julia set:
```
./build/glFractals julia
```

## Controls
- **WASD** - moves the camera
- **Q** - decreases iterations
- **E** - increases iterations
- **mouse scroll** - zoom
- **left click and drag** - drag the camera around
- **right click and drag** - for the Julia fractal, changes the seed value

## Future work
- Zooming in resolution is limited by floating point accuracy as GLSL shaders
primarilly use single precision floats.
- Right now the only way to change the color profile is by editing the fragment
shader.
## Resources

https://learnopengl.com/

http://nuclear.mutantstargoat.com/articles/sdr_fract/

