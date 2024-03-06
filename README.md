# delta-crdt-decomposition

Delta based CRDTs with join decompositions and state diffs.

## usage

This project uses meson as a build system and it is written in C++20.
Ensure that you have `meson` available in your system.
Its installation instructions are [here](https://mesonbuild.com/Getting-meson.html).

The first setup is to setup the build system itself with:

```bash
$ meson setup build
```

This command will create a build directory.
To run the test suit execute the following commands:

```bash
$ cd build
$ meson test
```

