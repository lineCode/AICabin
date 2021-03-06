#pragma once
class CAIActivityWnd
	: public CAILearnBaseWnd
{
public:
	CAIActivityWnd();
	virtual ~CAIActivityWnd();

	UIBEGIN_MSG_MAP
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw"), OnBtnLeftPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw_un"), OnBtnLeftPanelArrawUn);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw"), OnBtnRightPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw_un"), OnBtnRightPanelArrawUn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_leave"), OnEventLeave);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_activity_two_layout"), OnBtnactivityTwoLayout);
	UIEND_MSG_MAP

	HWND			CreateWnd(HWND hParent);

    void            ShowWindowData();//显示窗口及播放标题声音

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

private:
    bool                OnEventReturn(TNotifyUI* pTNotify);//返回 响应事件
    bool                OnEventLeave(TNotifyUI* pTNotify);//离开 响应事件
	bool                OnBtnactivityTwoLayout(TNotifyUI* pTNotify);//离开 响应事件
    bool                OnLayoutBodyPartsEvent(TEventUI& evt);
    bool                OnLayoutListenReadEvent(TEventUI& evt);
    bool                OnLayoutVrAiEvent(TEventUI& evt);

    void                SpeechTitle();//语音播放标题
};

