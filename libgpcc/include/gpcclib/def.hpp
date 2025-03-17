/**
* @file def.hpp
* @brief DLLExport用マクロ定義ファイル
* @author KDDI CORPORATION
* @details Windows向けDLLExport用マクロを定義したファイル
*/
#pragma once

//#define DLL_BUILD

#if defined(GPCCLIB_DYNAMIC_LINK_LIBRARY_BUILD)
#	if defined(_MSC_VER)
#		define GPCCLIB_INTERFACE_SPEC __declspec(dllexport)
#
#	elif defined(__GNUC__)
#		ifdef WIN32
#			define GPCCLIB_INTERFACE_SPEC __declspec(dllexport)
#		else
#			define GPCCLIB_INTERFACE_SPEC
#		endif
#	endif
#else // #if defined(GPCCLIB_DYNAMIC_LINK_LIBRARY_BUILD)
#	define GPCCLIB_INTERFACE_SPEC
#endif // #if defined(GPCCLIB_DYNAMIC_LINK_LIBRARY_BUILD)
