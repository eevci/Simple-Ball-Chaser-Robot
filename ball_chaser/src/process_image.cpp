#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>

// Define a global client that can request services
ros::ServiceClient client;
int white_pixel = 255;
// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
	ball_chaser::DriveToTarget message;

	message.request.linear_x = lin_x;
	message.request.angular_z = ang_z;
	if (!client.call(message))
        ROS_ERROR("Failed to call service drive_bot");

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
	int numberOfChannels = sensor_msgs::image_encodings::numChannels(img.encoding);

	int found_j, index;
	bool is_white_pixel_found = false;
    for(int i = 0; i<img.height; i++)
	{
		for(int j = 0; j<img.step - numberOfChannels; j+=numberOfChannels)
		{
			index = (i*img.step)+j;
			is_white_pixel_found = true;
			for(int k=0; k<numberOfChannels; k++)
			{
				if(img.data[index+k]!=white_pixel)
				{
					is_white_pixel_found = false;
					break;
				}
			}
			if(is_white_pixel_found == true)
			{
				found_j = j;
				break;	
			}
		}
		if(is_white_pixel_found == true)
			break;	
	}
	if(is_white_pixel_found == true)
	{
		if(found_j < img.step*0.25)
			drive_robot(0.0, 1);
		else if(found_j > img.step*0.75)
			drive_robot(0.0, -1);
		else
			drive_robot(1,0);	
	}
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
