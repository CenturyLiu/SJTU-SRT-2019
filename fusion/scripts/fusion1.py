import rospy
import lidar2img
from fusion.msg import lidar3_single
from fusion.msg import lidar3_whole
from fusion.msg import imagel_single
from fusion.msg import imagel_whole
from fusion.msg import cone_pos
from fusion.msg import cone_pos_whole

transf_x = -69.0
transf_y = 0.0
transf_z = 55.0

class cone_on_img():
    def __init__(self, u, v):
        self.u = u
        self.v = v

class cone_fusion():
    def __init__(self):
        self.lidar_raw = []
        self.image_raw = []
        self.fusion_pub = rospy.Publisher("/cone_detected", cone_pos_whole, queue_size = 10)

    def findMinI(self,l):
        length = len(l)
        mins = 0
        mini = l[0]

        for ii in range(1, length):
            if mini > l[ii]:
                mini = l[ii]
                mins = ii

        return mins        

    def Euc_dist(self,pos_1,pos_2):
        pos2_x = (pos_2.start_x + pos_2.end_x) / 2
        pos2_y = (pos_2.start_y + pos_2.end_y) / 2
        return ((pos_1.u - pos2_x) ** 2 + (pos_1.v - pos2_y) ** 2) ** 0.5

    def fusion(self, pose, dt, msg):
        u, v = lidar2img(pose.x, pose.y, pose.z)
        coneTmp = cone_on_img(u, v)
        jj = len(image_raw)
        dist = []

        if jj != 0:
            for ii in range(0, jj):
                dist.append(Euc_dist(coneTmp, image_raw[jj]))

            ii = findMinI(dist)
            cone = cone_pos()
            cone.x = pose.x
            cone.y = pose.y
            cone.color = image_raw[ii].color
            self.lidar_raw.append(cone)

    def lidar_callback(self, msg):
        msgl = len(msg.lidar3_points)
        if msgl != 0:
            for i in msg.lidar_points:
                i.x += transf_x
                i.y += transf_y
                i.z += transf_z
            time_stamp = msg.stamp
            jj = len(self.image_raw) - 1
            delta_t = 1.0
            while jj >= 0:
                if time_stamp > self.image_raw[jj].stamp:
                    delta_t = time_stamp - self.image_raw[jj].stamp
                    break
                jj = jj - 1
            del self.image_raw[0:jj] #the jjth item is not deleted, such that next time we can find at least one ealier time stamp

            length_msg = len(msg.lidar3_points)
            for ii in range(0,length_msg):
                self.fusion(msg.lidar3_points[ii],delta_t,msg)

            kk = len(lidar_raw) - 1
            while kk >= 0:
                self.fusion_pub.publish(self.lidar_raw[kk])
                del self.lidar_raw[kk]
                kk = kk - 1

    def image_callback(self, msg):
        self.image_append(msg)

if __name__ == "__main__":
    rospy.init_node("fusion", anonymous = True)
    Cone_fusion = cone_fusion()
    rospy.Subscriber("/image_processed", imagel_whole, cone_fusion.image_callback)
    rospy.Subscriber("/lidar_processed", lidar3_whole, cone_fusion.lidar_callback)
    rospy.spin()
    
