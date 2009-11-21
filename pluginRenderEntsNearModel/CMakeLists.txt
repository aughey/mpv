MPV_PLUGIN_INIT(PluginRenderEntsNearModel)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderEntsNearModel_PRIVATE_HDRS
    glClearDrawable.h
    NearFarManipulatorElement.h
    NearFarManipulatorElementFactory.h
    PluginRenderEntsNearModel.h
    ProjectionCallback.h
)
SET(PluginRenderEntsNearModel_SRCS
    glClearDrawable.cpp
    NearFarManipulatorElement.cpp
    NearFarManipulatorElementFactory.cpp
    PluginRenderEntsNearModel.cpp
    ProjectionCallback.cpp
)

ADD_LIBRARY(PluginRenderEntsNearModel MODULE
    ${PluginRenderEntsNearModel_PUBLIC_HDRS}
    ${PluginRenderEntsNearModel_PRIVATE_HDRS}
    ${PluginRenderEntsNearModel_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderEntsNearModel)

TARGET_LINK_LIBRARIES(PluginRenderEntsNearModel
    mpvcommon mpvcommonosg)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderEntsNearModel
    ${OSG_LIBRARY})
TARGET_LINK_LIBRARIES(PluginRenderEntsNearModel ${OPENGL_LIBRARIES})
