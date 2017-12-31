project( STKToy )

cmake_minimum_required(VERSION 2.4.5)

# ------------------ Boilerplate 
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)
 
if ( COMMAND cmake_policy )
  cmake_policy( SET CMP0003 NEW )  
endif()


# Include...
include_directories(
    /usr/local/include
)

add_library( stkcontrol SHARED
	stkcontrol.cpp
    InstrumentBank.cpp
)
target_link_libraries(stkcontrol stk )

add_executable( tester
    main.cpp
)
target_link_libraries(tester stkcontrol )
