# Open Builder

[![C++ CI](https://github.com/Hopson97/open-builder/workflows/C++%20CI/badge.svg)](https://github.com/Hopson97/open-builder/actions?query=workflow%3A"C%2B%2B+CI")

Open source Minecraft-like voxel sandbox game with multiplayer support.

More information about the project can be found in the **[Open Builder Wiki](https://github.com/Hopson97/open-builder/wiki)**

This was made mostly for a YouTube series which can be found here: **[Lets Code A Multiplayer Voxel Game](https://www.youtube.com/watch?v=4Rg1RriQZ9Q&list=PLMZ_9w2XRxiYb-ewSron6jd2fC1UHbDbJ&index=2)**

## Screenshot

Game is in uh very early stages

![player](https://i.imgur.com/RLqoqKw.png)

## Project Structure

A quick overview of the code and project structure can be found in the wiki article:

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
sudo bash scripts/install.sh
```

from the project root directory. If you are interested in the [manual way of installation](https://github.com/Hopson97/open-builder#manual-installation), check below instructions. To then finally run the application see [Running](https://github.com/Hopson97/open-builder#running) below.

#### Manual Installation

##### Libraries

These are required for the project, and can usually be downloaded via your distribution-specific package manager:

```sh
sudo apt install libsfml-dev                # Debian/Ubuntu
sudo pacman -S sfml                         # Arch/Manjaro
sudo yum install sfml-devel                 # Fedora/RHEL
```

Feel free to create a pull request to add support for your system's package manager!

If there is no mirror available, then you can download and manually install them from their respective websites:

[SFML Download](https://www.sfml-dev.org/download.php)

##### Building

This project uses CMake to create the build files. You can install the tool using your package manager:

```sh
sudo apt install cmake  # Debian
sudo pacman -S cmake    # Arch/Manjaro
sudo yum install cmake  # Fedora/RHEL
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

## Special Thanks

[D3PSI](https://github.com/D3PSI) - Added GitHub actions, and created the install.sh file to automate the installation process on Linux systems

CalvinSpace - Created a lot of the textures for the blocks

## History

Originally, this was going to be a Zombie game, but I got very bored of that very quickly, and so changed the project to be a voxel game instead.

As a result of this, the code was created for the zombie game, so when I decided to switch over, the code got messy very quickly.

To combat this, I did a massive refactor in just over 2 weeks, re-creating large parts of the project.

The before-refactor-project can be found **[here](https://github.com/Hopson97/open-builder/tree/a452dfd0a5d8fc94059f7e1ab8ecca03dd06ba3a)**

The commit that adds the refactored code can be found **[here](https://github.com/Hopson97/open-builder/tree/9b6c7c83fb1331fa8740ad12d6df9390d5b31c0c)**
