#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

char getKey()
{
    char c;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    read(STDIN_FILENO, &c, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "keyboard_node");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<std_msgs::Int32>("/servo_angle", 10);

    int angle = 90;
    ROS_INFO("A/D pomeranje | Q izlaz");

    while (ros::ok())
    {
        char key = getKey();

        if (key == 'a')
            angle = std::min(angle + 5, 160);
        else if (key == 'd')
            angle = std::max(angle - 5, 20);
        else if (key == 'q')
            break;

        std_msgs::Int32 msg;
        msg.data = angle;
        pub.publish(msg);

        ROS_INFO("Ugao: %d", angle);
    }

    return 0;
}

