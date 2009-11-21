
#include <stdio.h>
#include <dlfcn.h>

int main ( int argc, char** argv )
{
	void * handle = NULL;
	
	printf( 
	"=========================================================================\n"
	"MPV uses the LTDL library to load plugins.  Unfortunately, the LTDL library \n"
	"gives horrible, misleading error messages when a plugin fails to load.\n"
	"This utility will load the shared library file specified on the command line \n"
	"using the dlopen() mechanism.  Any errors will be printed to the console.\n"
	"These error messages are much more useful than those issued by LTDL.  This \n"
	"allows you to find errors in your plugin which are otherwise difficult \n"
	"to diagnose using the MPV.\n"
	"Note - Don't forget to set your LD_LIBRARY_PATH to include the directories \n"
	"where your MPV libraries are located (libmpvcommon, etc).  Also, you need \n"
	"to give the *full* path name of the plugin library.  Example:\n"
	"\t%s /junk/mpv/mpv/pluginEntityMgr/.libs/libpluginEntityMgr.so\n"
	"=========================================================================\n\n",
	argv[0] );

	
	if( argc != 2 )
	{
		printf( "usage: %s /path/to/pluginFileName.so\n", argv[0] );
		exit( 1 );
	}
	
	handle = dlopen( argv[1], RTLD_NOW );
	if( handle == NULL )
	{
		printf( "dlopen of %s failed.\n", argv[1] );
		printf( "the error, as reported by dlerror(), is:\n\t" );
		printf( "%s\n", dlerror() );
	}
	else
	{
		printf( "dlopen of %s succeeded.\n", argv[1] );
		dlclose( handle );
	}
	
	printf( "exiting...\n" );
	
	return 0;
}


