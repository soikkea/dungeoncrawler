cmake_minimum_required (VERSION 3.8)

project ("dungeoncrawler")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES
	"./source/headers/*.h"
	"./source/src/*.cpp"
)

# Set this in cmake cache
#set(SFML_DIR "path/to/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(dungeoncrawler ${SOURCES})

target_include_directories(dungeoncrawler PUBLIC
	"./source/headers"
)

if(MSVC)
	target_compile_options(dungeoncrawler PRIVATE /W4)
else()
	target_compile_options(dungeoncrawler PRIVATE -Wall -Wextra -pedantic)
endif()

target_link_libraries(dungeoncrawler PUBLIC
	"sfml-graphics"
	"sfml-window"
	"sfml-system"
)

add_custom_command(TARGET dungeoncrawler POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:dungeoncrawler>/resources/
)
