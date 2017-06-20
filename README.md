# Reversi

## Building

```console
git submodule update --init
mkdir out
cd out
cmake ..
make && ./tests
```

## Debug mode

```
cmake -DCMAKE_BUILD_TYPE=Debug .. # For debug mode.
make && ./tests
```
