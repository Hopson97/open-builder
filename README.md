# open-builder

Open source Minecraft-like voxel sandbox game with multiplayer support.

## Inspirations

This project was heavily inspired by the following projects:

### Minetest

Another open source voxel sandbox game, one of the original ones. Also created using C++, and also has multiplayer support.

[Website](http://www.minetest.net)

[GitHub](https://github.com/minetest/minetest)

### Terasolgy

Open source multiplayer voxel sandbox game created using Java like the original minecraft.

[Website](https://terasology.org)

[GitHub](https://github.com/MovingBlocks/Terasology)

### Craft 

Minecraft clone created by Fogleman in C.

[Website](https://www.michaelfogleman.com/projects/craft/)

[GitHub](https://github.com/fogleman/Craft)

## History

Originally, this was going to be a Zombie game, but I got very bored of that very quickly, and so changed the project to be a voxel game instead.

As a result of this, the code was created for the zombie game, so when I decided to switch over, the code got messy very quickly.

To combat this, I did a masive refactor in just over 2 weeks, re-creating the most the project.

The before refactor can be found [here](https://github.com/Hopson97/open-builder/tree/a452dfd0a5d8fc94059f7e1ab8ecca03dd06ba3a)

## Building and Running

### Libraries

SFML and GLM are required.

These can be installed from your project manager. For example, on Debian/ Ubuntu:

```sh
sudo apt install libsfml-dev libglm-dev
```

If this is not possible (eg windows), you can install these manually from their respective websites:

[SFML Download](https://www.sfml-dev.org/download.php)

[GLM Download](https://github.com/g-truc/glm/tags)

### Linux

To build, at the root of the project:

```sh
sh scripts/build.sh
```

To run, at the root of the project:

```sh
sh scripts/run.sh               #Launches both client and server  
sh scripts/run.sh -server 8     #Launches server only, allowing upto 8 connections
sh scripts/run.sh -client       #Launches client
```

To build and run in release mode, simply add the `release` suffix:

```sh
sh scripts/build.sh release
sh scripts/run.sh release <args>
```

You can also create a deployable build (that can be sent) by doing:

```sh
sh scripts/deploy.sh
```
