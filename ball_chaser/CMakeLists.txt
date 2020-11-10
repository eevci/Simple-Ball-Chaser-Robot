cmake_minimum_required(VERSION 2.8.3)
project(ball_chaser)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
  controller_manager
)
## Generate services in the 'srv' folder
 add_service_files(
   FILES
   DriveToTarget.srv
 )


## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
 )

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(drive_bot src/drive_bot.cpp)
add_dependencies(drive_bot ball_chaser_generate_messages_cpp)
target_link_libraries(drive_bot
   ${catkin_LIBRARIES}
 )

add_executable(process_image src/process_image.cpp)
add_dependencies(process_image ball_chaser_generate_messages_cpp)
target_link_libraries(process_image
   ${catkin_LIBRARIES}
 )

