# Motor-Speed-Controller
This is a motor speed controller for arduino

# Description
This motor controller uses a PID algorithm to regulate the speed of a motor using an infrarred optocoupler mounted over a encoder disk (connected to the output shaft of the motor) using the pulses from the optocoupler as the disk spins as a feedback and a H bridge as an output to control the direction and speed of the motor rotations.
The controller will receive a string via I2C or Serial containing the direction of the rotations (In the form of the plus or minus sign) and the speed of the rotations (In rpm).

# The PID controller
The controller requires to be tuned to ensure the desired operation
The Ziegler-Nichols second tuning method for PID controllers in the book "Modern Control Engineering" by Katsuhiko Ogata is recommended, due to its simplicity and the small amount of time required.
