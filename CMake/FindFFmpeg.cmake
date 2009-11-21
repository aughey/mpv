#
# Find the native FFMPEG includes and library
#
# This module defines
# FFMPEG_INCLUDE_DIR, where to find avcodec.h, avformat.h ...
# FFMPEG_LIBRARIES, the libraries to link against to use FFMPEG.
# FFMPEG_FOUND, If false, do not try to use FFMPEG.
#
# also defined, but not for general use are
# FFMPEG_avformat_LIBRARY and FFMPEG_avcodec_LIBRARY, where to find the FFMPEG library.
# This is usefull to do it this way so that we can always add more libraries
# if needed to FFMPEG_LIBRARIES if ffmpeg ever changes...
#
# Based on http://www.vtk.org/cgi-bin/viewcvs.cgi/CMake/FindFFMPEG.cmake?view=markup

FIND_PATH(FFMPEG_INCLUDE_DIR ffmpeg/avformat.h) 

FIND_LIBRARY(FFMPEG_avformat_LIBRARY avformat)
FIND_LIBRARY(FFMPEG_avcodec_LIBRARY avcodec)
FIND_LIBRARY(FFMPEG_avutil_LIBRARY avutil)
FIND_LIBRARY(FFMPEG_vorbis_LIBRARY vorbis)
FIND_LIBRARY(FFMPEG_dc1394_LIBRARY dc1394_control)
FIND_LIBRARY(FFMPEG_vorbisenc_LIBRARY vorbisenc)
FIND_LIBRARY(FFMPEG_theora_LIBRARY theora)
FIND_LIBRARY(FFMPEG_dts_LIBRARY dts)
FIND_LIBRARY(FFMPEG_gsm_LIBRARY gsm)
FIND_LIBRARY(FFMPEG_z_LIBRARY z) 

IF(FFMPEG_INCLUDE_DIR)
  IF(FFMPEG_avformat_LIBRARY)
    IF(FFMPEG_avcodec_LIBRARY)
      IF(FFMPEG_avutil_LIBRARY)
        SET( FFMPEG_FOUND "YES" )
        SET( FFMPEG_BASIC_LIBRARIES 
          ${FFMPEG_avcodec_LIBRARY} 
          ${FFMPEG_avformat_LIBRARY}
          ${FFMPEG_avutil_LIBRARY} 
          )
        SET( FFMPEG_LIBRARIES 
          ${FFMPEG_BASIC_LIBRARIES}
          ${FFMPEG_vorbis_LIBRARY} 
          ${FFMPEG_dc1394_LIBRARY} 
          ${FFMPEG_vorbisenc_LIBRARY} 
          ${FFMPEG_theora_LIBRARY} 
          ${FFMPEG_dts_LIBRARY} 
          ${FFMPEG_gsm_LIBRARY} 
          ${FFMPEG_z_LIBRARY})
      ENDIF(FFMPEG_avutil_LIBRARY)
    ENDIF(FFMPEG_avcodec_LIBRARY)
  ENDIF(FFMPEG_avformat_LIBRARY)
ENDIF(FFMPEG_INCLUDE_DIR)

#MARK_AS_ADVANCED(
#  FFMPEG_INCLUDE_DIR
#  FFMPEG_avformat_LIBRARY
#  FFMPEG_avcodec_LIBRARY
#  FFMPEG_avutil_LIBRARY
#  FFMPEG_vorbis_LIBRARY
#  FFMPEG_dc1394_LIBRARY
#  FFMPEG_vorbisenc_LIBRARY
#  FFMPEG_theora_LIBRARY
#  FFMPEG_dts_LIBRARY
#  FFMPEG_gsm_LIBRARY
#  FFMPEG_z_LIBRARY
#  )
