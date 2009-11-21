IF(OSGAL_FOUND)

    MPV_PLUGIN_INIT(PluginRenderSoundRootOSGAL)
    INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(../commonOSG)
    INCLUDE_DIRECTORIES(${OSGAL_INCLUDE_DIRS})

    SET(PluginRenderSoundRootOSGAL_PRIVATE_HDRS
        PluginRenderSoundRootOSGAL.h
        )
    SET(PluginRenderSoundRootOSGAL_SRCS
        PluginRenderSoundRootOSGAL.cpp
        )

    ADD_LIBRARY(PluginRenderSoundRootOSGAL MODULE
        ${PluginRenderSoundRootOSGAL_PUBLIC_HDRS}
        ${PluginRenderSoundRootOSGAL_PRIVATE_HDRS}
        ${PluginRenderSoundRootOSGAL_SRCS})
    MPV_PLUGIN_PROCESS_TARGET(PluginRenderSoundRootOSGAL)

    LINK_DIRECTORIES(${OSGAL_LIBRARY_DIRS})
    MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderSoundRootOSGAL
        ${OSG_LIBRARY})
    TARGET_LINK_LIBRARIES(PluginRenderSoundRootOSGAL
        mpvcommon openal osgAL)
ENDIF()
