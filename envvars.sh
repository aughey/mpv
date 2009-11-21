LTDL_LIBRARY_PATH=
for i in plugin* 
do
	LTDL_LIBRARY_PATH=$LTDL_LIBRARY_PATH:$PWD/$i
done
export LTDL_LIBRARY_PATH

for name in \
	$PWD/common/.libs \
	$PWD/commonOSG/.libs \
	$PWD/xlobject/src/.libs \
	$PWD/hps/.libs \
	$PWD/osgSDL/src/osgSDL/.libs
do
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$name
done
export LD_LIBRARY_PATH

export DEF_FILE_PATH=$PWD/deffilesamples
