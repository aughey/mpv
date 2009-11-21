MPV_PLUGIN_INIT(PluginEphemerisModel)

SET(PluginEphemerisModel_PRIVATE_HDRS
    AstronomicalBody.h
    Colorspace.h
    Date.h
    ExceptionBase.h
    Export.h
    Moon.h
    Namespace.h
    PluginEphemerisModel.h
    SkyColors.h
    SkyShader.h
    Sun.h
    Timing.h
    Uniform.h
)
SET(PluginEphemerisModel_SRCS
    AstronomicalBody.cpp
    Colorspace.cpp
    Date.cpp
    ExceptionBase.cpp
    Moon.cpp
    PluginEphemerisModel.cpp
    SkyColors.cpp
    SkyShader.cpp
    Sun.cpp
    Timing.cpp
)

ADD_LIBRARY(PluginEphemerisModel MODULE
    ${PluginEphemerisModel_PUBLIC_HDRS}
    ${PluginEphemerisModel_PRIVATE_HDRS}
    ${PluginEphemerisModel_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginEphemerisModel)

TARGET_LINK_LIBRARIES(PluginEphemerisModel mpvcommon)
MPV_TARGET_LINK_CCL_LIBRARIES(PluginEphemerisModel)
