# dungeoncrawler

A dungeon crawling game written in C++ using SFML.

## How to build

The project uses SFML version 2.5.1. You need to supply Cmake with the variable `SFML_DIR` which points to your `SFML-2.5.1/lib/cmake/SFML` directory.

On Windows the executable requires the following SFML dlls:
```
sfml-graphics-2.dll
sfml-system-2.dll
sfml-windows-2.dll
```