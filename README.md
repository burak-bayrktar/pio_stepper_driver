# pio_stepper_driver
 A simple stepper motor driver with pio

 There is not much in main but code under motion is self explanatory. This code written for a two-wheeled robot with both wheel driven by individual stepper notors.
 The robot never made it to completion but stepper driving code is neat.
 PIO state machines has fifo's in both ways. Simply pushing the amount of steps needed to be taken then start the state machine. 
 PIO code fetches the number and starts to generate pwm (assuming frequencies are set according to your need).
 The neat part is that if the robot needs to stop for a reason before completing given number of steps (check haltmotion func) pio pushes
 back the remaining amount of steps (that have not been taken). This is useful to approximate the location of the robot.
