#if defined(_MSC_FULL_VER)
	#if defined(MDXX_EXTERNAL_CONTEXT) || defined(MDXX_INTERFACE_DLL)
		#define DLL_IMPORT_EXPORT __declspec(dllexport)
	#else
		#define DLL_IMPORT_EXPORT __declspec(dllimport)
	#endif
#else
	#define DLL_IMPORT_EXPORT
#endif

#define MDXX_STRX(X) #X
#define MDXX_STR(X) MDXX_STRX(X)
