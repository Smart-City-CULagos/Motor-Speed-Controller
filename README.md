# Motor-Speed-Controller
A simple PID motor speed controller for Arduino

# Description
This motor controller uses a PID algorithm to regulate the speed of a motor using an infrarred optocoupler mounted over a encoder disk (connected to the output shaft of the motor) using the pulses from the optocoupler as the disk spins as a feedback and a H bridge as an output to control the direction and speed of the motor rotations.
The controller will receive a string via I2C or Serial containing the direction of the rotations (In the form of the plus or minus sign) and the speed of the rotations (In rpm).

# The PID controller
The controller requires to be tuned to ensure the desired operation.
The Ziegler-Nichols second tuning method for PID controllers in the book "Modern Control Engineering" by Katsuhiko Ogata followed by a manual tuning is recommended, due to its simplicity and the small amount of time required.

# Files and Folders
- **Tuning** - This folder contains a simple tuning code for the PID controller 
- **Serial** - This folder contains a Serial (UART) version of the motor speed controller
  - **main.cpp** - The arduino main code file
  - **constants.h** - Some constants for the main file
- **I2C** - This folder contains a I2C version of the motor speed controller
  - **main.cpp** - The arduino main code file
  - **constants.h** - Some constants for the main file
