The CigiManager plugin (pluginCigiManager) forwards CIGI traffic from the 
upstream system (the Host) to other CIGI IG systems (the "renderers").  If the 
CigiManager plugin is running in an MPV instance that has a functioning motion 
tracker, it can forward that motion tracker's data to the renderers.  

This plugin, pluginMotionTrackerRecvr, is able to process the motion tracker 
data forwarded by pluginCigiManager.  It appears to the pluginMotionTrackerMgr 
as an ordinary motion tracker.

This feature is a bit of a hack.  See pluginMotionTrackerMgr for more 
information on this feature, especially the comments in
"pluginMotionTrackerMgr/PluginMotionTrackerMgr.h".

Do not use this plugin as a basis for other motion tracker plugins.
