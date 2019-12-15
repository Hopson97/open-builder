# Building

## Windows [Visual Studio]

Please use the links below:

[Build With Visual Studio](https://github.com/Hopson97/open-builder/blob/9b6c7c83fb1331fa8740ad12d6df9390d5b31c0c/docs/Build_With_Visual_Studio.md)

[Running the Unit Tests With Visual Studio](https://github.com/Hopson97/open-builder/blob/9b6c7c83fb1331fa8740ad12d6df9390d5b31c0c/docs/Visual_Studio_Unit_Tests.md)

## Linux

### Download SFML and GLM

These are required for the project, and usually can be downloaded via your distrobutions

```sh
sudo apt install libsfml-dev libglm-dev
```

If not, then you can download them from the website:

[SFML Download](https://www.sfml-dev.org/download.php)

[GLM Download](https://github.com/g-truc/glm/tags)

### Build and Run

To build, at the root of the project:

```sh
sh scripts/build.sh
```

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

### Unit Tests

The unit tests can be ran using

```sh
sh scripts/tests.sh
```
