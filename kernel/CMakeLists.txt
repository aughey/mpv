INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/common)
INCLUDE_DIRECTORIES(${CCL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PDL_INCLUDE_DIRS})

SET(kernel_PRIVATE_HDRS
   DefaultP.h
   IGCtrlProcessor.h
   Kernel.h
   main.h
   PluginManager.h
   StateMachine.h
)
SET(kernel_SRCS
   DefaultP.cpp
   IGCtrlProcessor.cpp
   Kernel.cpp
   main.cpp
   PluginManager.cpp
   StateMachine.cpp
)

#==========================================================
# Create the executable.
#==========================================================

ADD_EXECUTABLE(mpv 
    ${kernel_PUBLIC_HDRS} ${kernel_PRIVATE_HDRS} ${kernel_SRCS})

#==========================================================
# Link dependencies.
#==========================================================

MPV_TARGET_LINK_CCL_LIBRARIES(mpv)

TARGET_LINK_LIBRARIES(mpv mpvcommon ${PDL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#==========================================================
# Install rule
#==========================================================

INSTALL(TARGETS mpv
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
