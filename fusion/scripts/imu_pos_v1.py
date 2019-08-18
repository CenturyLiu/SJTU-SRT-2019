#!/usr/bin/env python
# -*- coding: UTF-8 -*-
from __future__ import absolute_import, division, print_function
import os
import sys
import cv2
import numpy as np
import rospy

from sensor_msgs.msg import Imu
from visualization_msgs.msg import *



if __name__ == "__main__":
    rospy.init_node('image_process',anonymous = True)
    pos = Pos();
    rospy.Subscriber('/mynteye/imu/data_raw',Imu,pos.callback,queue_size = 1, buff_size = 52428800)
    rospy.spin()
