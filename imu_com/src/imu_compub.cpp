#include "ros/ros.h"
#include "std_msgs/String.h"
#include <serial/serial.h>  //ROS内置的串口包 
#include <imu_com/gpsMsg.h> //自定义话题gpsMsg
#include <sstream>


serial::Serial ser; //声明串口对象 
uint8_t gps_date[83]; //定义串口数据存放数组
int16_t comb16(uint8_t first ,uint8_t second);//声明8位数据合成16位函数
int32_t comb32(uint8_t first ,uint8_t second ,uint8_t third ,uint8_t fouth);//声明8位数据合成32位函数
int64_t comb64(uint8_t first ,uint8_t second ,uint8_t third ,uint8_t fouth,uint8_t fifth,uint8_t sixth,uint8_t seventh,uint8_t eighth);//声明8位数据合成64位函数



/*主函数*/
int main(int argc, char *argv[])
{
	ros::init(argc, argv, "gps_node1");  //建立GPS节点
	ros::NodeHandle n;
    /*创建话题发送欧拉角*/
	//ros::Publisher ruler_pub = n.advertise<gps::rulerMsg>("ruler_pub", 1000);
	/*创建话题发送GPS*/
    ros::Publisher imu_compub = n.advertise<imu_com::gpsMsg>("gps_info1", 1000);
	try 
	{ 
		//设置串口属性，并打开串口 
		ser.setPort("/dev/ttyUSB0"); 
		ser.setBaudrate(460800); 
		serial::Timeout to = serial::Timeout::simpleTimeout(1000); 
		ser.setTimeout(to); 
		ser.open(); 
	} 
	catch (serial::IOException& e) 
	{ 
		ROS_ERROR_STREAM("Unable to open port "); 
		return -1; 
	} 
	//检测串口是否已经打开，并给出提示信息 
	if(ser.isOpen()) 
	{ 
		ROS_INFO_STREAM("Serial Port initialized"); 
	} 
	else 
	{ 
		return -1; 
	} 
	ros::Rate loop_rate(20); //设置频率20Hz
	imu_com::gpsMsg gps_m;//定义了变量用来存储串口读取到的数据

	while (ros::ok())
	{
		if(ser.available())
		{ 
			ser.read(gps_date,30);
			// gps_m.x=gps_date[0];
			// gps_m.y=gps_date[1];
			// gps_m.yaw=gps_date[2];
			// ROS_INFO("Publisher: x = %d ",gps_m.x);
			// ROS_INFO("Publisher: y = %d ",gps_m.y);
			// ROS_INFO("Publisher: z = %d ",gps_m.yaw);
		
			for(int i =0;i<50;i++) 
			{
				if (gps_date[i]==0x00)
				{
					if (gps_date[i+1]==0x80)
					{
						if (gps_date[i+2]==0x00)
						{
							//int32 x
							gps_m.x= comb64(gps_date[i+18],gps_date[i+17],gps_date[i+16],gps_date[i+15],gps_date[i+14],gps_date[i+13],gps_date[i+12],gps_date[i+11]);
							//int32 y
							gps_m.y= comb64(gps_date[i+36],gps_date[i+25],gps_date[i+24],gps_date[i+23],gps_date[i+22],gps_date[i+21],gps_date[i+20],gps_date[i+19]);
							//int32 yaw 
							gps_m.yaw = comb16(gps_date[i+6],gps_date[i+5]);
							//int32 v_imu_x
							gps_m.v_imu_x = comb32(gps_date[i+34],gps_date[i+33],gps_date[i+32],gps_date[i+31]);
							//int32 v_imu_y
							gps_m.v_imu_y = comb32(gps_date[i+38],gps_date[i+37],gps_date[i+36],gps_date[i+35]);
							//int64 stamp
							gps_m.stamp = comb32(gps_date[i+52],gps_date[i+51],gps_date[i+50],gps_date[i+49]);
							
							
							ROS_INFO("Publisher: x = %d ",gps_m.x);
							ROS_INFO("Publisher: y = %d ",gps_m.y);
							ROS_INFO("Publisher: yaw = %d ",gps_m.yaw);
							ROS_INFO("Publisher: vx = %d ",gps_m.v_imu_x);
							ROS_INFO("Publisher: vy = %d ",gps_m.v_imu_y);
							ROS_INFO("Publisher: time = %d ",gps_m.stamp);
						}

					}
				}


			}

			


			
		}

	    ser.flush ();//清空串口存储空间

		//ROS_INFO("Publisher: yaw = %d ",gps_m);

		imu_compub.publish(gps_m);//发布GPS数据主题

		ros::spinOnce();
		loop_rate.sleep();
		
       
	}

	return 0;
}



int16_t comb16(uint8_t first ,uint8_t second)  //数据解析8->16
{
	int16_t temp=0;
	temp = ((int16_t)first<<8);
	temp |=((int16_t)second);
	
	return temp;
}

int32_t comb32(uint8_t first ,uint8_t second ,uint8_t third ,uint8_t fouth) //数据解析8->32
{
	int32_t temp=0;
	temp  = ((int32_t)first<<24);
	temp |= ((int32_t)second<<16);
	temp |= ((int32_t)third<<8);
	temp |= ((int32_t)fouth); 
	return temp;
}

int64_t comb64(uint8_t first ,uint8_t second ,uint8_t third ,uint8_t fouth,uint8_t fifth,uint8_t sixth,uint8_t seventh,uint8_t eighth)
{
	int64_t temp=0;
	temp  = ((int64_t)first<<56);
	temp |= ((int64_t)second<<48);
	temp |= ((int64_t)third<<40);
	temp |= ((int64_t)fouth<<32); 
	temp |= ((int64_t)fifth<<24);
	temp |= ((int64_t)sixth<<16);
	temp |= ((int64_t)seventh<<8);
	temp |= ((int64_t)eighth); 
	return temp;

}

