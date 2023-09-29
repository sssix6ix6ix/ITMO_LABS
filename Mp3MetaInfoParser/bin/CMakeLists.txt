add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC mp3)
target_link_libraries(${PROJECT_NAME} PUBLIC frames)
target_link_libraries(${PROJECT_NAME} PUBLIC Header)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})