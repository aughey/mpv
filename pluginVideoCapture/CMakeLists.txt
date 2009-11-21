MPV_PLUGIN_INIT(PluginVideoCapture)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginVideoCapture_PRIVATE_HDRS
    JpegImage.h
    PluginVideoCapture.h
)
SET(PluginVideoCapture_SRCS
    JpegImage.cpp
    PluginVideoCapture.cpp
)

ADD_LIBRARY(PluginVideoCapture MODULE
    ${PluginVideoCapture_PUBLIC_HDRS}
    ${PluginVideoCapture_PRIVATE_HDRS}
    ${PluginVideoCapture_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginVideoCapture)

TARGET_LINK_LIBRARIES(PluginVideoCapture
    mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginVideoCapture
    ${OSGDB_LIBRARY} ${OSGGA_LIBRARY} ${OSG_LIBRARY})

IF(JPEG_FOUND)
    TARGET_LINK_LIBRARIES(PluginVideoCapture ${JPEG_LIBRARY})
ENDIF()
IF(USE_FFMPEG AND FFMPEG_FOUND)
    TARGET_LINK_LIBRARIES(PluginVideoCapture ${FFMPEG_avcodec_LIBRARY})
ENDIF()
