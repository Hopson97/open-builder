As of ~April 2020, I stopped working on this project for several reasons.

The way I created this was quite "over done" to the point it got hard to work out how to add new features, perhaps due to trying to be an engine rather than a game. However, I am interested in one day revisiting this concept, however perhaps in a different repository rather than this one.

# Open Builder

![Linux C/C++ CI](https://github.com/Hopson97/open-builder/workflows/Linux%20C/C++%20CI/badge.svg)
![macOS C/C++ CI](https://github.com/Hopson97/open-builder/workflows/macOS%20C/C++%20CI/badge.svg)

Open source Minecraft-like voxel sandbox game with multiplayer support.

More information about the project can be found in the **[Open Builder Wiki](https://github.com/Hopson97/open-builder/wiki)**

This was made mostly for a YouTube series which can be found here: **[Lets Code A Multiplayer Voxel Game](https://www.youtube.com/watch?v=4Rg1RriQZ9Q&list=PLMZ_9w2XRxiYb-ewSron6jd2fC1UHbDbJ&index=2)**

## Building and Running

### Windows (Visual Studio)

The easiest way to build is to use [vcpkg](https://vcpkg.io/en/index.html) and install dependencies through this:

```bash
vcpkg install sfml
vcpkg install imgui
vcpkg install glm
vcpkg integrate install
```

Then open the Visual Studio project file to build and run.

### Linux

#### Pre-requisites

Install Vcpkg and other required packages using your distribution's package manager:

```sh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

# These are required to build some packages
sudo apt install cmake make autoconf libtool pkg-config

# The following are required for SFML
sudo apt install libx11-dev xorg-dev freeglut3-dev libudev-dev
```

Ensure paths are set correctly:

```sh
export VCPKG_ROOT=/path/to/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```

RECOMMENDED: Add the above lines to your `.bashrc` or `.zshrc` file:

```sh
echo 'export VCPKG_ROOT=/path/to/vcpkg' >> ~/.bashrc
echo 'export PATH=$VCPKG_ROOT:$PATH' >> ~/.bashrc
```

#### Build and Run

To build, at the root of the project:

```sh
vcpkg install # First time only
sh scripts/build.sh
```

To run, at the root of the project:

```sh
sh scripts/run.sh
```

To build and run in release mode, simply add the `release` suffix:

```sh
sh scripts/build.sh release
sh scripts/run.sh release
```

## Project Structure

A quick overview of the code and project structure can be found in the wiki article:

**[Project Structure](https://github.com/Hopson97/open-builder/wiki/Project-Structure)**

## Contributing

Please see main article here:

**[Contributing](https://github.com/Hopson97/open-builder/wiki/Contributing)**

## Screenshots

Terrain gen:

![player](https://i.imgur.com/FSz0iWu.png)

Early screenshot:
![player](https://i.imgur.com/RLqoqKw.png)

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

## Special Thanks

[D3PSI](https://github.com/D3PSI) - Added GitHub actions, and created the install.sh file to automate the installation process on Linux systems

CalvinSpace - Created a lot of the textures for the blocks

## History

Originally, this was going to be a Zombie game, but I got very bored of that very quickly, and so changed the project to be a voxel game instead.

As a result of this, the code was created for the zombie game, so when I decided to switch over, the code got messy very quickly.

To combat this, I did a massive refactor in just over 2 weeks, re-creating large parts of the project.

The before-refactor-project can be found **[here](https://github.com/Hopson97/open-builder/tree/a452dfd0a5d8fc94059f7e1ab8ecca03dd06ba3a)**

The commit that adds the refactored code can be found **[here](https://github.com/Hopson97/open-builder/tree/9b6c7c83fb1331fa8740ad12d6df9390d5b31c0c)**
