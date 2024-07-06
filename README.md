# Data Structures and Algorithms in C

This library contains a well-tested and documented but non-exhaustive set of common Data Structures and Algorithms.

## Installation

This project is build with [the meson build system](https://mesonbuild.com/).

1) Clone this repository
2) cd into the repository
3) Setup the project
```sh  
meson setup build
```
4) (Optional): Configure the project
```sh
meson configure build -Dweight_type=int -Dfail_test=abort
```
5) Build the project
```sh
meson compile -C build
```
6) (Optional): Test the project
```sh
meson test -C build
```
Alternatively if you have it installed you can launch the tests with valgrind:
```sh
meson test --wrap='valgrind --leak-check=full --error-exitcode=1' -C build
```
6) Install the project
```sh
meson install -C build
```

## Features

### Data structures:
- Doubly-linked lists
- Binary trees
- Graphs with adjacency matrix
- Graphs with adjacency lists

### Algorithms
- Dijkstra
- Bellmann
