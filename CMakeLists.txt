cmake_minimum_required(VERSION 3.15)
project(broker)

add_subdirectory("${PROJECT_SOURCE_DIR}/qc_broker_lib" "${PROJECT_SOURCE_DIR}/qc_broker_lib_output ")
add_subdirectory("${PROJECT_SOURCE_DIR}/qc_publisher_app" "${PROJECT_SOURCE_DIR}/qc_publisher_app_output ")
add_subdirectory("${PROJECT_SOURCE_DIR}/qc_subscriber_app" "${PROJECT_SOURCE_DIR}/qc_subscriber_app_output ")
add_subdirectory("${PROJECT_SOURCE_DIR}/qc_broker_app" "${PROJECT_SOURCE_DIR}/qc_broker_app_output ")