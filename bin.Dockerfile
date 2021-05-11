from rostooling/setup-ros-docker:ubuntu-focal-latest

run apt update && apt install -y vim curl less
run apt update && apt install -y gazebo11 libgazebo11-dev gazebo11-dbg

