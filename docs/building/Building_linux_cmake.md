# Building and Running for Linux

## Manual Building

Install SFML via your package manager eg:

```sh
sudo apt install libsfml-dev                # Debian/Ubuntu
sudo pacman -S sfml                         # Arch/Manjaro
sudo yum install sfml-devel                 # Fedora/RHEL
```

If it is not available, then it can instead be downloaded and installed manaually from:

[SFML Download](https://www.sfml-dev.org/download.php)

Download the repo:

```sh
git clone https://github.com/Hopson97/open-builder.git
cd open-builder
```

Build the project (Shell script will use CMake):

```sh
sh scripts/build.sh # For building in debug mode
sh scripts/build.sh release # For building in release mode
```

## Installer build

This will automatically install SFML and build the project:

```sh
sudo bash scripts/install.sh
```

## Running

Run the project (This will launch both client and server on localhost):

```sh
sh scripts/run.sh
# or
sh scripts/run.sh release  # Launches both client and server  
```

You can launch server standalone using:

```sh
sh scripts/run.sh -server 8 # Allows up to 8 connections
sh scripts/run.sh release -server 4 # Release mode, allows up to 4 connections
```

You can launch client standalone using:

```sh
sh scripts/run.sh -client Bob # Join server with name Bob
sh scripts/run.sh release -client Alice # Release mode, join server with name Alice
```

TODO EXPLAIN HOW TO CONNECT TO ANOTHER COMPUTER

## Unit Tests

The unit tests can be built and ran using

```sh
sh scripts/tests.sh
```
