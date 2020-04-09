# Build and Run for Mac

First install SFML:

```sh
brew install SFML
```

Download the repo:

```sh
git clone https://github.com/Hopson97/open-builder.git
cd open-builder
```

Next, build with cmake:
(from root directory)

```sh
mkdir build
cd build
cmake ..
# or cmake .. -DCMAKE_BUILD_TYPE=Release ../..
make
```

Run using:

```sh
./build/open-builder
```

Can also run client and server separately:

```sh
./build/open-builder -server 8
./build/open-builder -client xyz
```
