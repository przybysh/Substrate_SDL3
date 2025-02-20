# cmake minimal version
cmake_minimum_required(VERSION 3.16)

# project name
# change to whatever you like
project(substrate)

# this is required to 
# fetch build and use external projects
# we will download SDL3 and all necessary
# dependecies with it
include(ExternalProject)
include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

# now let's fetch content SDL3 and other deps
FetchContent_Declare(SDL3
   GIT_REPOSITORY git@github.com:libsdl-org/SDL.git
   GIT_PROGRESS TRUE
   GIT_TAG main
)
FetchContent_MakeAvailable(SDL3)

# type in here all .cpp files of your project
set(MAIN_SRC_FILES main.cpp)


# make executable 
# first parameter it's a executbale name that will be equal to whatever have you set
# in line 6
# second parameter is list of your project files
add_executable(${PROJECT_NAME} ${MAIN_SRC_FILES})

# now link your executable with all the depenedencies of your project
target_link_libraries(${PROJECT_NAME} 
                        PRIVATE SDL3::SDL3)