# SJTU-SRT-2019
> SJTU-SRT-2019 contains codes for ShangHai JiaoTong University's 2019 autonomous racing car 

## Overview 
This project is designed for a FSAE competition racing car, not for a real life autonomous car. Before proceeding, please read competition rules page xx to page xx so as to have a better understanding of the environment we expect our car to be in.

To control the autonomous racing car, we need to perceive the surrouding environment, manage our route by the perception outcome and send control signal to the bottom level of the car. As introduced in the competition rule, the track is indicated by cones of different color. To recognize those cones, we choose lidar and depth camera as our sensor. Lidar is used for measuring the 3D position of the cones, and camera will provide the color of the cones. After perceiving the track, we use the pure-pursuit algorithm to manage our route and control the car (for detailed perception and control logic, see [fusion](https://github.com/CenturyLiu/srt-2019-fusion)). 

Based on the design idea introduced above, the codes contained in this repository can be divided into three levels: sensor utilize level, perception and fusion level and car control level.
* Sensor Utilize level: 
     * MYNT-EYE-D-SDK (a modified version of slightech/MYNT-EYE-D-SDK [MYNT](https://github.com/slightech/MYNT-EYE-D-SDK)), codes for utilizing the MYNT-EYE I-120 depth camera
     * ros_rslidar-master (directly derived from RoboSense-LiDAR/ros_rslidar [rslidar](https://github.com/RoboSense-LiDAR/ros_rslidar)), codes for utilizing the Robosense 16-line lidar
     * imu-com, codes for our imu (haven't been put into real use yet)
* Perception level:
     * fusion (fully self-created package, responsible for cone detection, sensor fusion and route management)
* Car control level
     * simple_controller (self-created package, responsible for translating upper level control commands into CAN signal and send the signal to the car)
## Prerequest
   * Knowledge: basic knowledge of linux and ROS
   * Computer: A computer with Ubuntu 16.04 and ROS-kinetic (the computer should be local,don't use virtual machine or development boards like TX1/TX2, make sure the computer has 3 or more usb ports and an ethernet port)
   * Sensors: Robosense 16-line lidar; Mynteye I-120 depth camera
## Installation
   * git clone SJTU-SRT-2019 into your ROS workspace, i.e. "~/catkin_ws/src"
   * Install ros_rslidar: follow this tutorial [rslidar_install](https://github.com/Suoivy/ros_rslidar_robosense) by Suoivy
   * Install MYNT-EYE-D-SDK: follow this tutorial [MYNT-install](https://mynt-eye-d-sdk.readthedocs.io/en/latest/sdk/install_ubuntu_src.html)
   * Install following python modules: pillow, tensorflow, sympy, sklearn (these are essential modules, but not all the modules)
   * Install simple_controller: execute the following codes
      cd ~/catkin_ws/src/simple_controller/src
      sudo cp libusbcan.so /lib /usr/lib /usr/local/lib
## Implementation
   * roslaunch rslidar_pointcloud rs_lidar_16.launch (start the lidar)
   * roslaunch mynteye_wrapper_d mynteye.launch (start the camera)
   * roslaunch fusion camera_lidar_fusion_visual.launch (start the perception and fusion nodes)
