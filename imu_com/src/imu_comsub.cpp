#include <ros/ros.h>

#include <imu_com/gpsMsg.h>

  //ROS自带的浮点类型，类似 float，但是不同



void gpsCallback(const imu_com::gpsMsg::ConstPtr &msg){   //回调函数，参数类型为 ConstPtr 类型的指针，它被定义在之前编译生成的 gps.h 中，指向 gps 的消息

                //声明一个距离变量 distance

     //之所以是 distance.data，是因为 Floa32 是一个结构体，成员变量 data 才存储着值

   ROS_INFO("Subscriber: x = %d ",msg->x);
   ROS_INFO("Subscriber: y = %d ",msg->y);
   ROS_INFO("Subscriber: yaw = %d ",msg->yaw);
   ROS_INFO("Subscriber: vx = %d ",msg->v_imu_x);
   ROS_INFO("Subscriber: vy = %d ",msg->v_imu_y);
   ROS_INFO("Subscriber: time = %d ",msg->stamp);

}



int main(int argc,char** argv){

   ros::init(argc,argv,"imu_comsub");  

   ros::NodeHandle n;

   ros::Subscriber sub = n.subscribe("gps_info1",1000,gpsCallback);  //

   ros::spin();

   return 0;

}
