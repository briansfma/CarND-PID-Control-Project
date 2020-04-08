# PID for Steering Control Along a Path
Udacity Self-Driving Car Engineer Nanodegree Program

In this project our goal is to use PID controls only to have a car follow a predefined path around a track. We are provided only the distance (positive or negative) our car is off from the ideal path - from there, the car adjusts its
steering angle automatically to try and maintain a close distance to the path.

All is not simple, however - the path is often discontinuous, triggering the car to make sudden jerks in steering to try and correct its sudden error. The focus of this project is to balance between aggressively staying close to the ideal path and being able to dampen the sudden changes in path such that the final path driven is as smooth (and within the boundaries) as possible.

[//]: # (Image References)
[image1]: data-errors.jpg "CTE Spikes"
[image2]: 2020-04-07_Frame.jpg "Runtime Example"

## Basic Build Instructions

Running the code requires connecting to the Udacity CarND Term 2 Simulator, which can be downloaded [here](https://github.com/udacity/self-driving-car-sim/releases).

This repository includes two files that can be used to set up and install [uWebSocketIO](https://github.com/uWebSockets/uWebSockets) for either Linux or Mac systems. For windows you can use either Docker, VMware, or even [Windows 10 Bash on Ubuntu](https://www.howtogeek.com/249966/how-to-install-and-use-the-linux-bash-shell-on-windows-10/) to install uWebSocketIO. Please see the uWebSocketIO Starter Guide page in the classroom within the EKF Project lesson for the required version and installation scripts.

Once the install for uWebSocketIO is complete, the main program can be built and run by executing in the Linux bash:

```
$ git clone https://github.com/briansfma/CarND-PID-Control-Project
$ cd CarND-PID-Control-Project
$ mkdir build
$ cd build
$ cmake ..
$ make $*
$ ./pid
```

## Running the Program

If it is not running already, launch the project.

```
$ cd CarND-PID-Control-Project
$ cd build
$ ./pid
```

When `pid` has initialized successfully, it will output to terminal

```
Listening to port 4567
```

Launch the simulator `term2_sim.exe`. Select "Project 4: PID Controller" from the menu. Upon successful connection to the simulator, `pid` will output to terminal

```
Connected!!!
```

The simulator will automatically start.

## Details

The ego car normally uses straightforward PID controls to guide the steering towards the ideal path (minimizing error, denoted as `cte` in the code). The coefficient `Kd` for the differential error is set especially high to provide "damping" to the steering response, compromising a higher `cte` for a visually smoother line through the track. The `Kp` for proportional error acts as the "spring" in the system, so this has been set low to prevent overreacting to changes in path. The `Ki` for integral error is good for correcting bias, and also useful as an "inertial" factor for damping out sudden inputs. But since the track is not a constant target, using integral error normally causes the car to spiral out of control as soon as the track turns. This code changes the integral error implementation to limit the timeframe the PID controller "remembers". Thus it can react to a changing track, without having zero inertia to damp out sudden steering changes.

Additionally, it was noticed early on that `cte` jumps suddenly throughout the lap, indicating that the path itself is discontinuous. `diff_cte` (`cte - previous_cte`) often spikes over 0.5 during runtime, but when the path is continuous, it should vary within the (-0.05, 0.05) range.

![alt text][image1]

In this situation, high `Kd` is a liability instead of a useful damping feature. If `diff_cte` is above 0.5, `Kd` = 4.0 so the steering input will instantly max out. So instead, when high `diff_cte` is encountered, the code switches to using PI control only, modifying the `Kp` coefficient to compensate for the lack of `Kd`.

With this situational toggle implemented, the PID/PI controller is able to handle most discontinuities elegantly, and the car travels smoothly where it would otherwise jerk suddenly.

(Click frame image to view/download video clip)

[![alt text][image2]](2020-04-07.mp4 "Runtime Example")

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).



