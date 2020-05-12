# Project 5 - Home Service Robot

## Description
This is the final project for the Udacity Robotics Software Engineering Nanodegree. A turtlebot is deployed in a custom gazebo environment (modelled after my apartment). The turtlebot is given a goal position that it navigates to using the `slam_gmapping` package. The `slam_gmapping` package was also used to generate the map of the environment used for navigation. The initial goal position is represented by a green square virtual object shown in Rviz. After picking up the virtual object, the robot navigates to a different goal position and drops off the virtual object.

## Packages
To implement this project several ROS packages are used:

`my_robot`: This package, created in [Project 2](https://github.com/SagarSaxena/Robotics-Nano-Degree/tree/master/Project2), contains the environment, modelled in Gazebo.

`slam_gmapping`: 
`turtlebot`:
`turtlebot_interactions`:
`turtlebot_simulator`:
`teleop_twist_keyboard`: 
`pick_objects`:
`add_markers`:

## Results
![Home Service Robot demo1](GIF/HomeServiceRobot.gif)
**Shown above:** Visualization in Rviz. The robot can be seen navigating to two different goal positions and picking up and dropping off a virtual object (green square).

![Home Service Robot demo2](GIF/HomeServiceRobot2.gif)
**Shown above:** Running the same script but visualizing the robot and environment in Gazebo.


TBC: Local costmap



## Install
```
$ git clone https://github.com/SagarSaxena/Home-Service-Robot.git
```

## Generating a Map
source the environment and launch the script:
```
$ cd ~/Home-Service-Robot/
$ source devel/setup.bash
$ cd src/scripts/
$ ./test_slam.sh
```
after mapping the environment open a terminal and save the map:
```
rosrun map_server map_saver
```
Note this will save two files, map.pgm and map.yaml in the current directory

## Testing the navigation stack
source the environment and launch the script:
```
$ cd ~/Home-Service-Robot/
$ source devel/setup.bash
$ cd src/scripts/
$ ./test_navigation.sh
```
Use the `2D Nav Goal` button in Rviz to set goal positions and watch the robot navigate to them

## Run the full project
source the environment and launch the script:
```
$ cd ~/Home-Service-Robot/
$ source devel/setup.bash
$ cd src/scripts/
$ ./home_service.sh
```
The robot will autonomously navigate to a pick up position indicated by a square marker, hold position for 5 second to simulate a "pick up", and then navigate to a drop off position and drop off the marker. 

