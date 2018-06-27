# DattaQuantumTransportAtomToTransistor

This project aims to provide implementations of the exercises in the book "Quantum Transport: Atom to Transistor" by S. Datta (2005), using the C++ library [TBTK](https://github.com/dafer45/TBTK/) for second quantized models.
The project is meant to demonstrate the ease with which a wide range of quantum mechanical calculations can be performed using TBTK.
It also aims to be an entry point for students interested in quantum transport.
Aiding such students by providing an implementation that puts focus on high level concepts rather than irrelevant numerical nuisance.

## Prerequisits
To compile the exercises, the [TBTK](https://github.com/dafer45/TBTK/) library must be installed.
Follow the instructions on the project page for TBTK.

## Compile and run exercises
Each exercise is implemented in a separate folder called E_X_X, where X_X is the exercise number.
Once TBTK is installed, the exercises can be compiled and run as follows
```bash
cd E_X_X
cmake .
make
./build/Application
```
Results are written to the figures folder.

## Currently completed exercises
The project is under active development and all available exercises are therefore not in a finished state.
The following exercises are considered completed.

* Exercise 2.1

## Contact
Please do not hesitate to get in contact if you have questions or would like to see quicker progress.
The larger the user engagement, the more motivation there is to get this project rolling quickly.
Contact me at kristofer.bjornson@physics.uu.se or see more at http://www.second-tech.com.
