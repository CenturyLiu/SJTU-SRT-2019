#!/usr/bin/env python

import rospy
from simple_controller.msg import Srt_Control

def main():
    rospy.init_node("controltest", anonymous = True)
    pub = rospy.Publisher('/srt_control', Srt_Control, queue_size=10)
    test_angle = [0, 100, 0.000001, -50, 30, -30, 100000, -300]   # test angle set
    angle = 0.0
    while not rospy.is_shutdown():
        for i in range(0, 8):
            angle = test_angle[i]
            rate = rospy.Rate(0.2)
            test_order = Srt_Control(0, angle)
            pub.publish(test_order)
            rate.sleep()

if __name__ == "__main__":
    main()
