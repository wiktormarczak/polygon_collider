# Polygon Collider
2D collision simulator with bouncing polygons. Made in C, SDL3 and OpenGL.

## Running
Project is build with CMake. Compiled executable must be run from the main project directory.
```
git clone --recurse-submodules https://github.com/wiktormarczak/polygon_collider.git
cd polygon_collider
mkdir build
cmake -S . -B build
make -C build
build/polygon_collider
```

## License
This program is licensed under GNU General Public License v3.
