# open-builder

Open source Minecraft-like game with multiplayer support.

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
