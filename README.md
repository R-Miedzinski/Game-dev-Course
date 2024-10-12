# Game Dev Course Project

This project is a game developed during `Programowanie gier komputerowych` course at the Faculty of Physics and Astronomy of the University of Wrocław. 

# Setup

## Linux (Ubuntu)

To setup the project on Ubuntu one needs:
 - `c++` compiler like `g++`
 - `cmake`
 - installed SFML library

### Installing SFML on Linux

On Linux SFML can be installed from package manager with the following command:

```bash
sudo apt-get install libsfml-dev
```

for full guide refer to [official SFML Linux guide]{https://www.sfml-dev.org/tutorials/2.6/start-linux.php}

### Setup build directory

`setup.sh` script is provided to manage build setup process of the project.

First run 

```bash
bash ./setup.sh
```

to generate `build` directory and setup `cmake`.

### Building project

Setup script will automatically build the project's executable.

In order to rebuild project manually, in the `build` directory run:

```bash
cmake --build .
```

### Running project

To run project run compiled executable `Project.out` in `build`.