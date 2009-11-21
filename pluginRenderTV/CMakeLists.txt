MPV_PLUGIN_INIT(PluginRenderTV)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

IF(UNIX)

    SET(PluginRenderTV_PRIVATE_HDRS
        PluginRenderTV.h
        Video.h
        osgTV.h
        BuildRectangle.h
        )
    SET(PluginRenderTV_SRCS
        PluginRenderTV.cpp
        Video.cc
        osgTV.cc
        BuildRectangle.cc
        )

    ADD_LIBRARY(PluginRenderTV MODULE
        ${PluginRenderTV_PUBLIC_HDRS}
        ${PluginRenderTV_PRIVATE_HDRS}
        ${PluginRenderTV_SRCS})
    MPV_PLUGIN_PROCESS_TARGET(PluginRenderTV)

    TARGET_LINK_LIBRARIES(PluginRenderTV mpvcommon)
    MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderTV ${OSG_LIBRARY})
    
    #
    # tvTest
    #
    ADD_EXECUTABLE(tvTest tvTest.cc
        Video.h Video.cc)

    #
    # tvViewer
    #
    ADD_EXECUTABLE(tvViewer osgviewer.cpp
         Video.h Video.cc osgTV.h osgTV.cc BuildRectangle.h BuildRectangle.cc)
    MPV_TARGET_LINK_OSG_LIBRARIES(tvViewer
       ${OSG_LIBRARY} ${OSGDB_LIBRARY} ${OSGUTIL_LIBRARY}
       ${OSGTEXT_LIBRARY} ${OSGVIEWER_LIBRARY} ${OSGGA_LIBRARY})

ENDIF(UNIX)
