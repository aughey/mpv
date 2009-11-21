IF(OSGAL_FOUND)

    MPV_PLUGIN_INIT(PluginRenderEntsSoundOSGAL)
    INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(../commonOSG)
    INCLUDE_DIRECTORIES(${OSGAL_INCLUDE_DIRS})

    SET(PluginRenderEntsSoundOSGAL_PRIVATE_HDRS
        PluginRenderEntsSoundOSGAL.h
        SoundElement.h
        SoundElementFactory.h
        )
    SET(PluginRenderEntsSoundOSGAL_SRCS
        PluginRenderEntsSoundOSGAL.cpp
        SoundElement.cpp
        SoundElementFactory.cpp
        )

    ADD_LIBRARY(PluginRenderEntsSoundOSGAL MODULE
        ${PluginRenderEntsSoundOSGAL_PUBLIC_HDRS}
        ${PluginRenderEntsSoundOSGAL_PRIVATE_HDRS}
        ${PluginRenderEntsSoundOSGAL_SRCS})
    MPV_PLUGIN_PROCESS_TARGET(PluginRenderEntsSoundOSGAL)

    MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderEntsSoundOSGAL
        ${OSG_LIBRARY} ${OPENTHREADS_LIBRARY})
    LINK_DIRECTORIES(${OSGAL_LIBRARY_DIRS})
    TARGET_LINK_LIBRARIES(PluginRenderEntsSoundOSGAL
        mpvcommon mpvcommonosg openal osgAL)

ENDIF()
