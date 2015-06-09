#pragma once
#include "IleanplumWSDK.h"

// CMusicStoreDlg dialog
class CMusicStoreDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMusicStoreDlg)

public:
	CMusicStoreDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMusicStoreDlg();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAlbum(UINT nID);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool m_loadSuccess;
	bool m_maleFirst;

// Dialog Data
	enum { IDD = IDD_DIALOG_MUSIC_STORE };

	CString m_user;
	CString m_gender;

	unsigned int m_userID;
	unsigned int m_deviceID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
    CToolTipCtrl m_ToolTip;
	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

private:
	CFont m_staticFont;
	void InitializeAlbum(bool maleFirst);
	void InitializeControl(UINT buttonID, UINT bitmapID);
	void LoadLeanplumSDK();
	void ReleaseLeanplumSDK();

	HMODULE m_leanplumDLL;
	ILeanplumWSDK* m_leanplum;
	
	unsigned int m_count;
};
