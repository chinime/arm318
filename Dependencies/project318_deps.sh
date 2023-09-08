#!/bin/sh

apt-get update && apt-get upgrade -y && apt-get install -y lsb-core g++ git

rosdep init && rosdep update

apt-get install git

apt install ros-noetic-moveit -y
apt-get install ros-noetic-ros-control ros-noetic-ros-controllers
apt install ros-noetic-joint-state-publisher-gui

apt-get install python3-catkin-tools

cd ~

git clone https://github.com/chinime/arm318.git

mkdir -p /./catkin_ws/src

cd /catkin_ws/src

git clone https://github.com/ros-drivers/rosserial.git

mv -v ~/arm318/* .

/bin/bash -c '. /opt/ros/noetic/setup.bash; cd /catkin_ws; catkin build'

echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc

echo "source /catkin_ws/devel/setup.bash" >> ~/.bashrc