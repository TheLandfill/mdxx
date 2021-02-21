#if defined(_MSC_FULL_VER)
	#if defined(WINDOWS_PLUGIN)
		#define DLL_IMPORT_EXPORT __declspec(dllexport)
	#else
		#define DLL_IMPORT_EXPORT __declspec(dllimport)
	#endif
#else
	#define DLL_IMPORT_EXPORT
#endif
