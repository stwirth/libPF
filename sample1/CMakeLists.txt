
include_directories(../lib/include)

add_executable(sample1
                main.cpp
                MyMovementModel.cpp
                MyObservationModel.cpp
                MyStateDistribution.cpp)

target_link_libraries(sample1 PF)
