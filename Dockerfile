from rostooling/setup-ros-docker:ubuntu-focal-latest

shell ["/bin/bash", "-c"]

run sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
run wget https://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -
run apt-get update
run apt-get update && apt-get install -y vim gdb
copy dependencies.sh dependencies.sh
run apt-get update && \
    GAZEBO_MAJOR_VERSION=9 ROS_DISTRO=dummy source dependencies.sh && \
    echo $BASE_DEPENDENCIES $GAZEBO_BASE_DEPENDENCIES | tr -d '\\' | xargs apt-get install -y

# run apt-get update && wget https://raw.githubusercontent.com/ignition-tooling/release-tools/master/jenkins-scripts/lib/dependencies_archive.sh -O /tmp/dependencies.sh \
#    && GAZEBO_MAJOR_VERSION=9 ROS_DISTRO=dummy source /tmp/dependencies.sh # \

