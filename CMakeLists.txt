cmake_minimum_required(VERSION 3.29.2)
project(silver_utilities)

find_package(Boost REQUIRED COMPONENTS unit_test_framework system filesystem process)

file(GLOB SOURCE
    ${PROJECT_SOURCE_DIR}/silver/utilities/*.cpp
    ${PROJECT_SOURCE_DIR}/silver/utilities/*.h
    ${PROJECT_SOURCE_DIR}/silver/utilities/*.hpp
)

add_library(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS} 
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
    LINKER_LANGUAGE CXX
    CXX_STANDARD 23
) 

if(${TESTS_ENABLED})
    enable_testing()
    add_subdirectory(tests/program/)
    add_subdirectory(tests/registry/)
    add_subdirectory(tests/singleton/)
endif()

