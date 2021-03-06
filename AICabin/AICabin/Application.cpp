#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "AILearnBaseWnd.h"
#include "AIViewResultsWnd.h"
#include "AIFaceLearnWnd.h"
#include "AISpeakLearnWnd.h"
#include "AIActivityWnd.h"
#include "AICabinWnd.h"
#include "MainWnd.h"
#include "NDHttp/NDHttp.h"
#include "NDHttp/NDHttpBiz.h"
#include "NDHttp_Wrapper.hpp"
#include "Application.h"
#include "NewUI/UIAutoSizeButton.h"


CApplication::CApplication()
{
	InitializeCriticalSection(&m_ControlLock);
	InitializeCriticalSection(&m_ControlLockFace);
}


CApplication::~CApplication()
{
	if (m_pMainWnd)
	{
		delete m_pMainWnd;
		m_pMainWnd = nullptr;
	}

	DeleteCriticalSection(&m_ControlLock);
	DeleteCriticalSection(&m_ControlLockFace);
}

bool CApplication::Initialize(HINSTANCE hInstance, LPTSTR lpCmdLine)
{
	if (!InitDirectUI(hInstance))
		return false;

	CMainWnd::RegisterWnd();

	return true;
}

bool CApplication::Run()
{
	// 开舱接口测试
	/*CHttpRequestDataModel Model;
	Model.strHost = "ai-learning-box-service.beta.101.com";
	Model.strUrl = "/v0.1/visitor/opening_infos?cabin_code=ai_learning_box";
	Model.strMethod = "PUT";
	Model.strHeader = "Accept: application/json\r\nContent-Type: application/json\r\nsdp-app-id:44cebc7f-7b35-4e4d-95d0-3c7af9db955e";
	Model.nPort = 443;

	string strData;
	DWORD code = NDhttp_Wrapper::Excute(&Model, strData);
	wstring strUnicode = CommonUtil::Utf8ToUnicode(strData.c_str());*/

	m_pMainWnd = new CMainWnd();
	if (m_pMainWnd)
	{
		m_hMsgWnd = m_pMainWnd->CreateWnd();
	}

	CAICabinWnd* pAICabinWnd = new CAICabinWnd;
	if (!pAICabinWnd)
		return false;

	if (!pAICabinWnd->CreateWnd())
		return false;

	pAICabinWnd->SetAutoDel(true);

	pAICabinWnd->ShowWindow(SW_SHOW, false);

	pAICabinWnd->SetFullScreen(true);
	pAICabinWnd->HideAIHelloLayout();
	OpenAICabin();

	//message loop
	m_pMainWnd->LoopMessage();

	//LeaveAICabin(true);

	return true;
}

HWND CApplication::GetMainHwnd()
{
	return m_hMsgWnd;
}

bool CApplication::InitDirectUI(HINSTANCE hInstance)
{
	GetUIEngine()->InitSkin();
	GetUIRes()->SetDefaultImageType(UITYPE_IMAGE::UIIAMGE_BITMAP);
    GetUIRes()->SetLanguage(_T("zh_CN"));

	tstring strLocalPath = CommonUtil::GetLocalPath();

	tstring strFrameworkPath = strLocalPath + _T("\\framework\\");
	GetUIRes()->InitResDir(strFrameworkPath.c_str());

	tstring strSkinsPath = strLocalPath + _T("\\Skins\\");
	GetUIRes()->InitResDir(strSkinsPath.c_str(), true);

    UI_OBJECT_REGISTER(CAutoSizeButtonUI);

	return true;
}

bool CApplication::OpenAICabin()
{
	m_strUserID = _T("");
	m_strUserName = _T("");

	//启动线程开始轮询
	std::thread t([this]{

        LeaveAICabin();

		do
		{
			::Sleep(1000);

			CHttpRequestDataModel Model;
			Model.strHost = "ai-learning-box-service.sdp.101.com";

#ifdef _DEBUG
			Model.strUrl = "/v0.1/visitor/opening_infos?cabin_code=ai_learning_box2";
#else
			Model.strUrl = "/v0.1/visitor/opening_infos?cabin_code=ai_learning_box";
#endif
			
			Model.strMethod = "PUT";
			Model.strHeader = "Content-Type: application/json\r\nsdp-app-id:44cebc7f-7b35-4e4d-95d0-3c7af9db955e";
			Model.nPort = 443;

			wstring strUnicodeTmp = _T("{\"status\":1}");
			Model.strPost = CommonUtil::UnicodeToUtf8(strUnicodeTmp.c_str());

			string strData;
//#ifdef _DEBUG
			wstring strDataUnicode = _T("{\"id\":1,\"user_id\":\"705301\",\"user_name\":\"林贵华\",\"cabin_code\":\"ai_learning_box\",\"status\":1,\"create_time\":1577153025000,\"update_time\":1577153025000}");
			strData = CommonUtil::UnicodeToUtf8(strDataUnicode.c_str());
//#else
			/*DWORD code = NDhttp_Wrapper::Excute(&Model, strData);
			if (code != 0)
			{
				logwrapper::OutputInfo("{} error code:{}",
					__FUNCTION__,
					code);

				continue;
			}*/

			wstring strUnicode = CommonUtil::Utf8ToUnicode(strData.c_str());

			do 
			{
				Document document;
				if (document.Parse((char *)strData.c_str()).HasParseError())
					break;

				if (document.HasMember("user_id"))
					m_strUserID = CommonUtil::Utf8ToUnicode(GetStringByKey(document, "user_id").c_str());

				if (document.HasMember("user_name"))
					m_strUserName = CommonUtil::Utf8ToUnicode(GetStringByKey(document, "user_name").c_str());

				if (!m_strUserID.empty()
					&& !m_strUserName.empty())
				{
					//往消息窗口发送消息
					m_bRequest = false;

					::PostMessage(m_hMsgWnd, WM_AICABIN_OPEN, NULL, NULL);
				}

			} while (false);

		} while (m_bRequest);
	});

	t.detach();

	return true;
}

bool CApplication::LeaveAICabin(bool bSync /*= false*/)
{
	std::thread t([this]{

		CHttpRequestDataModel Model;
		Model.strHost = "ai-learning-box-service.sdp.101.com";

#ifdef _DEBUG
		Model.strUrl = "/v0.1/visitor/opening_infos?cabin_code=ai_learning_box2";
#else
		Model.strUrl = "/v0.1/visitor/opening_infos?cabin_code=ai_learning_box";
#endif

		Model.strMethod = "PUT";
		Model.strHeader = "Content-Type: application/json\r\nsdp-app-id:44cebc7f-7b35-4e4d-95d0-3c7af9db955e";
		Model.nPort = 443;

		wstring strUnicodeTmp = _T("{\"status\":2}");
		Model.strPost = CommonUtil::UnicodeToUtf8(strUnicodeTmp.c_str());

		string strData;
		DWORD code = NDhttp_Wrapper::Excute(&Model, strData);

		if (code != 0)
		{
			logwrapper::OutputInfo("{} error code:{}", __FUNCTION__, code);
		}
		else
		{
			wstring strUnicode = CommonUtil::Utf8ToUnicode(strData.c_str());
			logwrapper::OutputInfo("{} strData:{}", __FUNCTION__, strData);
		}
	});

	if (bSync)
	{
		t.join();
	}
	else
	{
		t.detach();
	}

	return true;
}
