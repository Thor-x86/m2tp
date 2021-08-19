# Multi-Mode Transmission Protocol

## Requirement

- CMake 3.14 or newer
- C and C++ compiler (GNU is recommended)
- Python 3 (for running `project.py` script)

## How to Initialize

Run this everytime you **clone** this repo:

```bash
./project.py init
```

> **ATTENTION:** Unit test requires [GTest](https://github.com/google/googletest#readme) which requires internet connection everytime you initialize to download GTest framework. Otherwise unit test throws errors.

## How to Build

**Do everytime you made changes**. [Make sure it's initialized](#how-to-initialize). If yes, then go run this command:

```bash
./project.py build
```

## How to Install

Make sure you did [initialize](#how-to-initialize) and [build](#how-to-build), then run:

```bash
./project.py install
```

> **NOTE:** It runs under sudo automatically, so you'll be asked for password. Make sure you have enough privilege otherwise it shows errors.

## How to Test

There are 3 type of test: **unit**, **integration**, and **e2e**.

To run them all, type this:

```bash
./project.py test
```

or if you only need specific test, as example unit test then run this command

```bash
./project.py test unit
```

## I messed up something on CMake, how to fix?

Simple, just run this command:

```bash
./project.py rebuild
```
