MPV_PLUGIN_INIT(PluginDefFileReader)

SET(PluginDefFileReader_PRIVATE_HDRS
    DefFileReader.h
)
SET(PluginDefFileReader_SRCS
    DefFileReader.cpp
)

ADD_LIBRARY(PluginDefFileReader MODULE
    ${PluginDefFileReader_PUBLIC_HDRS}
    ${PluginDefFileReader_PRIVATE_HDRS}
    ${PluginDefFileReader_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginDefFileReader)

TARGET_LINK_LIBRARIES(PluginDefFileReader mpvcommon)
