INCLUDE_DIRECTORIES( ${XINE_INCLUDE_DIR} )

SET(TARGET_SRC 
    video_out_rgb.c
    ReaderWriterXine.cpp
)

SET(TARGET_LIBRARIES_VARS XINE_LIBRARY )

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
ENDIF()


#### end var setup  ###
SETUP_PLUGIN(xine)
