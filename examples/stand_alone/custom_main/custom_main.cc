/*
 * Copyright (C) 2012 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_client.hh>

volatile sig_atomic_t stop;
const std::string TOPIC = "/chatter";

void inthand(int signum) {
    stop = 1;
}

void cb(ConstGzStringPtr &_msg)
{
  // Dump the message contents to stdout.
  std::cout << _msg->data() << std::endl;
}

void client_main(int argc, char** argv)
{
  gazebo::client::setup(argc, argv);

  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();
  gazebo::transport::SubscriberPtr sub = node->Subscribe(TOPIC, cb);
  while (!stop) {
    gazebo::common::Time::MSleep(10);
  }

  gazebo::client::shutdown();
}

void client_publisher(int argc, char ** argv)
{
  gazebo::client::setup(argc, argv);
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();
  gazebo::transport::PublisherPtr pub = node->Advertise<gazebo::msgs::GzString>(TOPIC);
  while (!stop) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    gazebo::msgs::GzString msg;
    msg.set_data("wfup");
    pub->Publish(msg);
  }
  gazebo::client::shutdown();
}

void server_main(int argc, char **argv)
{
  gazebo::setupServer(argc, argv);
  gazebo::physics::WorldPtr world = gazebo::loadWorld("worlds/empty.world");

  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  gazebo::transport::PublisherPtr pub =
    node->Advertise<gazebo::msgs::GzString>(TOPIC);
  pub.reset();

  while (!stop) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    gazebo::msgs::GzString msg;
    msg.set_data("arst");
    if (pub) {
      pub->Publish(msg);
    }
  }

  gazebo::shutdown();
}

/////////////////////////////////////////////////
int main(int argc, char **argv)
{
  signal(SIGINT, inthand);
  if (fork() == 0) {
    client_main(argc, argv);
  } else {
    server_main(argc, argv);
  }

  return 0;
}
