# Signal Processing Demo Repo

[![CMake on multiple platforms](https://github.com/ASethi77/dsp_book_demos/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/ASethi77/dsp_book_demos/actions/workflows/cmake-multi-platform.yml)

I've been enjoying the book **The Scientist and Engineer's Guide to Digital Signal Processing** lately
and wanted to make a supplementary codebase to demonstrate some of the core concepts in the book. I also
have an Obsidian vault where I'm keeping notes (and more code!) of specific concepts in the book, but that will
likely be published in some other repo.

Documentation here is best-effort but I'll try to maintain a list/summary of demos.

## Project structure

These demos are built using CMake and vcpkg, using a toolchain supporting C++20. I'm using clang 18 but any other
compiler should suffice.

The `libdsp` folder will contain common code for signal processing blocks -- think filters, buffers, etc.
The `demos` folder will contain subfolders for individual executables that demonstrate a variety of concepts across
the book.

## Demo list

| Demo Name                 | Description                                                                   |
|---------------------------|-------------------------------------------------------------------------------|
| reference_implot_demo     | Literally a port of the ImPlot demo window. It's just around to help me code. |
| floating_point_error_demo | Demo of floating point numerical error accumulation. See Chapter 4 pg 73.     |
| signal_decomposition_demo | Demo of signal even/odd decomposition                                         |
|                           |                                                                               |
