MPV_PLUGIN_INIT(PluginRenderStatisticsOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderStatisticsOSG_PRIVATE_HDRS
	CheckeredColorScreen.h
    EntityScreen.h
    FrameRateScreen.h
	GradientColorScreen.h
    GridColorScreen.h
	GridLinesScreen.h
    PluginRenderStatisticsOSG.h
    SolidColorScreen.h
)
SET(PluginRenderStatisticsOSG_SRCS
    CheckeredColorScreen.cpp
	EntityScreen.cpp
    FrameRateScreen.cpp
	GradientColorScreen.cpp
    GridColorScreen.cpp
	GridLinesScreen.cpp
    PluginRenderStatisticsOSG.cpp
    SolidColorScreen.cpp
)

ADD_LIBRARY(PluginRenderStatisticsOSG MODULE
    ${PluginRenderStatisticsOSG_PUBLIC_HDRS}
    ${PluginRenderStatisticsOSG_PRIVATE_HDRS}
    ${PluginRenderStatisticsOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderStatisticsOSG)

TARGET_LINK_LIBRARIES(PluginRenderStatisticsOSG
    mpvcommon mpvcommonosg)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderStatisticsOSG
    ${OSGTEXT_LIBRARY} ${OSGGA_LIBRARY} ${OSG_LIBRARY})

