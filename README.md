# Game Dev Course Project

This project is a game developed during `Programowanie gier komputerowych` course at the Faculty of Physics and Astronomy of the University of Wrocław. 

# Setup

## Linux (Ubuntu)

To setup the project on Ubuntu one needs:
 - `c++` compiler like `g++`
 - installed SFML library

### Installing SFML on Linux

On Linux SFML can be installed from package manager with the following command:

```bash
sudo apt-get install libsfml-dev
```

for full guide refer to [official SFML Linux guide]{https://www.sfml-dev.org/tutorials/2.6/start-linux.php}

### Building project

In order to compile project run:

```bash
make compile
```

This will create `sfml-app` file to run the application.

### Running project

To run project compiled program execute:

```bash
make run
```