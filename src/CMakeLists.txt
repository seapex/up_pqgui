# define the executable target
set (EXE_TARGET "up_pqgui")

set (SRC_LIST ${SRC_LIST} main.cpp main_window.cpp)
set (SRC_LIST ${SRC_LIST} resource/*.rc)

link_directories("C:/wseapex/Tools/1.Develop/fltk/lib")
set (EXTRA_LIBS ${EXTRA_LIBS} fltk fltk_images fltk_png fltk_z fltk_jpeg fltk_gl)
set (EXTRA_LIBS ${EXTRA_LIBS} glu32 opengl32 ole32 uuid comctl32)
# add the executable
add_executable (${EXE_TARGET} ${SRC_LIST})
target_link_libraries (${EXE_TARGET} ${EXTRA_LIBS})
#aux_source_directory(resource RC_SRC)

include_directories ("C:/wseapex/Tools/1.Develop/fltk" resource ../../up_pqied)
add_definitions(-DWIN32 -DUSE_OPENGL32 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE)
# Set the GCC Additional flags to use when compiling this target's sources
# -O0 -O1 -O2 -O3 ...
#set_property(TARGET ${EXE_TARGET} PROPERTY COMPILE_FLAGS -mwindows)

# Set the GCC Additional flags to use when linking this target
# -shared-libgcc -static-libstdc++ -static-libgcc -static...
set (CMAKE_EXE_LINKER_FLAGS "-static")

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

add_custom_command(TARGET ${EXE_TARGET} POST_BUILD 
  COMMAND cp ${PROJECT_BINARY_DIR}/bin/up_pqgui.exe ${PROJECT_SOURCE_DIR}/../up_pqied/release -a
  COMMENT "Copy up_pqm.exe to up_pqied/release" )

# specifies the directory into which runtime target files should be built
#set_property( TARGET ${EXE_TARGET} PROPERTY RUNTIME_OUTPUT_DIRECTORY
#              ${PROJECT_BINARY_DIR}/bin )

# add the install targets
#install (TARGETS ${EXE_TARGET} DESTINATION ${PROJECT_SOURCE_DIR}/../up_pqied/release)
#install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h" DESTINATION include)
