MPV_PLUGIN_INIT(PluginMotionTrackerJoystickSDL)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})

SET(PluginMotionTrackerJoystickSDL_PRIVATE_HDRS
    JoystickBindings.h
    PluginMotionTrackerJoystickSDL.h
)
SET(PluginMotionTrackerJoystickSDL_SRCS
    JoystickBindings.cpp
    PluginMotionTrackerJoystickSDL.cpp
)

ADD_LIBRARY(PluginMotionTrackerJoystickSDL MODULE
    ${PluginMotionTrackerJoystickSDL_PUBLIC_HDRS}
    ${PluginMotionTrackerJoystickSDL_PRIVATE_HDRS}
    ${PluginMotionTrackerJoystickSDL_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginMotionTrackerJoystickSDL)

TARGET_LINK_LIBRARIES(PluginMotionTrackerJoystickSDL
    ${SDL_LIBRARY}
    mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginMotionTrackerJoystickSDL ${OSG_LIBRARY})
