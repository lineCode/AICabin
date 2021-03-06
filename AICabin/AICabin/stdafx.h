// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <mmsystem.h>
#include <atlbase.h>
#include <atlcom.h>
#include <shobjidl.h>
#include <atlbase.h>
#include <atlwin.h>

// TODO:  在此处引用程序需要的其他头文件
#include "DirectUI.h"
#include "NDHttp/curl/curl.h"
#include "spdlog_wrapper.hpp"
#include "rapidjson_wrapper.hpp"

//由于DUI导出类用到stl相关模板类，简单禁用掉此类警告
#pragma warning( disable: 4251 )

#ifdef _DEBUG
#pragma comment( lib,"DirectUI_ud.lib" ) 
#pragma comment(lib, "NDHttp_ud.lib")
#else
#pragma comment( lib,"DirectUI_u.lib" )
#pragma comment(lib, "NDHttp_u.lib")
#endif

#pragma comment(lib, "WINMM.LIB")


using namespace rapidjson;
using namespace rjwrapper;