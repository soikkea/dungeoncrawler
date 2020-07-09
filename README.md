# dungeoncrawler

A dungeon crawling game written in C++ using SFML.

## How to build

The project uses [SFML](https://www.sfml-dev.org/index.php) version 2.5.1. You need to supply Cmake with the variable `SFML_DIR` which points to your `SFML-2.5.1/lib/cmake/SFML` directory.

### Unit tests

Unit tests use [Catch2](https://github.com/catchorg/Catch2).

## How to run

On Windows the executable requires the following SFML dlls:
```
sfml-graphics-2.dll
sfml-system-2.dll
sfml-windows-2.dll
```