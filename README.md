# Open Builder 

[![C++ CI](https://github.com/Hopson97/open-builder/workflows/C++%20CI/badge.svg)](https://github.com/Hopson97/open-builder/actions?query=workflow%3A"C%2B%2B+CI")

Open source Minecraft-like voxel sandbox game with multiplayer support.

More information about the project can be fouund in the **[Open Builder Wiki](https://github.com/Hopson97/open-builder/wiki)**

This was made mostly for a YouTube series which can be found here: **[Lets Code A Multiplayer Voxel Game](https://www.youtube.com/watch?v=4Rg1RriQZ9Q&list=PLMZ_9w2XRxiYb-ewSron6jd2fC1UHbDbJ&index=2)**

## Screenshot

Game is in uh very early stages

![player](https://i.imgur.com/RLqoqKw.png)

## Project Structure

A quick overview of the code and project structure can be found in the wiki artle:


**[Project Structure](https://github.com/Hopson97/open-builder/wiki/Project-Structure)**



## Contributing

Please see main article here:

**[Contributing](https://github.com/Hopson97/open-builder/wiki/Contributing)**

## Installing, Building and Running

### Windows [Visual Studio]

Please use the links below:

[Build With Visual Studio](https://github.com/Hopson97/open-builder/wiki/Building-(Visual-Studio))

[Running the Unit Tests With Visual Studio](https://github.com/Hopson97/open-builder/wiki/Unit-Tests-Setup-(Visual-Studio))

### Linux

We offer an automated installer for a bunch of different Linux distributions including Debian, Arch/Manjaro and Fedora. It can be ran via

```sh
sudo sh scripts/install.sh
```

from the project root directory. If you are interested in the manual way of installation, check below instructions. To then finally run the application see [Running](https://github.com/Hopson97/open-builder#running) below.

#### Manual Installation

##### Libraries

These are required for the project, and usually can be downloaded via your distributions

```sh
sudo apt install libsfml-dev libglm-dev
```

If not, then you can download them from the website:

[SFML Download](https://www.sfml-dev.org/download.php)

[GLM Download](https://github.com/g-truc/glm/tags)

##### Building

This project uses CMake to create the build files. You can install using your package manager eg

```sh
sudo apt install cmake
```

First clone the project and `cd`:

```sh
git clone https://github.com/Hopson97/open-builder.git
cd open-builder
```

To build, at the root of the project:

```sh
sh scripts/build.sh
```

#### Running

To run, at the root of the project:

```sh
sh scripts/run.sh               # Launches both client and server  
sh scripts/run.sh -server 8     # Launches server only, allowing upto 8 connections
sh scripts/run.sh -client xyz   # Launches client, with player name xyz
```

To build and run in release mode, simply add the `release` suffix:

```sh
sh scripts/build.sh release
sh scripts/run.sh release <args>
```

#### Unit Tests

The unit tests can be ran using

```sh
sh scripts/tests.sh
```


## Similar Projects

This project was heavily influenced by the following projects:

### Minetest

Another open source voxel sandbox game, one of the original ones. Also created using C++, and also has multiplayer support.

**[Website](http://www.minetest.net)** - **[GitHub](https://github.com/minetest/minetest)**

### Terasolgy

Open source multiplayer voxel sandbox game created using Java like the original minecraft.

**[Website](https://terasology.org)** - **[GitHub](https://github.com/MovingBlocks/Terasology)**

### Craft

Minecraft clone created by Fogleman in C.

**[Website](https://www.michaelfogleman.com/projects/craft/)** - **[GitHub](https://github.com/fogleman/Craft)**

## History

Originally, this was going to be a Zombie game, but I got very bored of that very quickly, and so changed the project to be a voxel game instead.

As a result of this, the code was created for the zombie game, so when I decided to switch over, the code got messy very quickly.

To combat this, I did a masive refactor in just over 2 weeks, re-creating the most the project.

The before refactor can be found **[here](https://github.com/Hopson97/open-builder/tree/a452dfd0a5d8fc94059f7e1ab8ecca03dd06ba3a)**

The commit that adds the refactored code can be found **[here](https://github.com/Hopson97/open-builder/tree/9b6c7c83fb1331fa8740ad12d6df9390d5b31c0c)**
