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

find_package(Catch2)

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

if(Catch2_FOUND)
	add_executable(dungeoncrawler-tests
		"source/tests/test-main.cpp"
		"source/tests/test-level.cpp"
		"source/headers/globals.h"
		"source/headers/level.h"
		"source/src/level.cpp"
		"source/headers/bspdungeon.h"
		"source/src/bspdungeon.cpp"
		"source/headers/rng.h"
		"source/src/rng.cpp"
		"source/headers/creature.h"
		"source/src/creature.cpp"
		"source/headers/item.h"
		"source/src/item.cpp"
		"source/headers/stat.h"
		"source/src/stat.cpp"
		"source/headers/skillset.h"
		"source/src/skillset.cpp"
		"source/headers/sprite.h"
		"source/src/sprite.cpp"
		"source/headers/player.h"
		"source/src/player.cpp"
		"source/headers/hud.h"
		"source/src/hud.cpp"
		"source/headers/hudbutton.h"
		"source/src/hudbutton.cpp"
		"source/headers/assets.h"
		"source/src/assets.cpp"
		"source/headers/game.h"
		"source/src/game.cpp"
	)

	target_include_directories(dungeoncrawler-tests PUBLIC
		"./source/headers"
	)

	target_link_libraries(dungeoncrawler-tests
		"sfml-graphics"
		"sfml-window"
		"sfml-system"
		Catch2::Catch2
	)
endif()