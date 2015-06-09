// MusicStoreDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ABTestingDemo.h"
#include "MusicStoreDlg.h"
#include "afxdialogex.h"
#include "APIKeysDefinition.h"

// CMusicStoreDlg dialog

IMPLEMENT_DYNAMIC(CMusicStoreDlg, CDialogEx)

void StatusCallBack(ActionType type, bool success, std::string errorMsg, void* extraInfo, void* context)
{
	switch (type)
	{
	case ActionType_None:
	case ActionType_Start:
	case ActionType_Stop:
	case ActionType_SetVars:	
	case ActionType_SetUserAttributes:
	case ActionType_Track:
		break;
	case ActionType_GetVars:
		{
			if (!context)
			{
				break;
			}

			CMusicStoreDlg* dlg = static_cast<CMusicStoreDlg*>(context);
			if (success)
			{
				dlg->m_loadSuccess = true;
				std::vector<leanplumVar>* varList = reinterpret_cast<std::vector<leanplumVar>*> (extraInfo);
				if (!varList)
				{
					break;
				}

				for (size_t i=0; i<varList->size(); ++i)
				{
					if ("AlbumOrder" == varList->at(i).name)
					{						
						if ("MaleFirst" == varList->at(i).sValue)
							dlg->m_maleFirst = true;	
						else
							dlg->m_maleFirst = false;
						break;
					}
				}
			}			
		}
		break;
	}
}

CMusicStoreDlg::CMusicStoreDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMusicStoreDlg::IDD, pParent)
	, m_leanplum(NULL)
	, m_loadSuccess(false)
	, m_count(0)
	, m_maleFirst(false)
{

}

CMusicStoreDlg::~CMusicStoreDlg()
{
}

void CMusicStoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMusicStoreDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMusicStoreDlg::OnBnClickedOk)
	ON_COMMAND_RANGE(IDC_BUTTON_ALBUM1, IDC_BUTTON_ALBUM4, &CMusicStoreDlg::OnBnClickedAlbum)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMusicStoreDlg message handlers
BOOL CMusicStoreDlg::OnInitDialog()
{	
	if(!m_ToolTip.Create(this))
	{
	   TRACE("Unable to create the ToolTip!");
	}
	else
	{
		m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_ALBUM1), _T("Click to get this album for only $10"));
		m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_ALBUM2), _T("Click to get this album for only $20"));
		m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_ALBUM3), _T("Click to get this album for only $25"));
		m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_ALBUM4), _T("Click to get this album for only $19"));
		m_ToolTip.Activate(TRUE);
	}

	CFont* pFont = GetDlgItem(IDC_STATIC_PREV)->GetFont();
    LOGFONT lf = {0};
    pFont->GetLogFont(&lf);	
    lf.lfUnderline = TRUE;
    m_staticFont.CreateFontIndirect(&lf);
	GetDlgItem(IDC_STATIC_PREV)->SetFont(&m_staticFont);
	GetDlgItem(IDC_STATIC_NEXT)->SetFont(&m_staticFont);
	
	bool asyncMode = false; // update it

	bool maleFirst = true;
	LoadLeanplumSDK();
	if (m_leanplum)
	{
		m_leanplum->Initialize(
			leanplumAPIKeys(APP_ID,
							PROD_KEY,
							DEV_KEY,
							EXP_KEY,
							CRO_KEY),
							m_userID,
							m_deviceID);

		if (asyncMode)
		{
			m_leanplum->EnableAsyncMode(true, StatusCallBack, this);
		}
		else
		{
			m_loadSuccess = true;
		}

		std::vector<leanplumVar> varList;
/*		leanplumVar var;
		var.name = "AlbumOrder";
		var.type = eLeanplumVarType_String;
		var.sValue = "FemaleFirst";
		varList.push_back(var);
		m_leanplum->SetVars(varList);
*/
		leanplumVar var;
		var.name = "Gender";
		var.type = eLeanplumVarType_String;
		var.sValue.assign((LPCSTR)m_gender.GetBuffer());
		varList.push_back(var);
		m_leanplum->SetUserAttributes(varList);
		varList.clear();
		m_leanplum->Start();
		m_leanplum->GetVars(varList);
		if (!asyncMode)
		{
			for (size_t i=0; i<varList.size(); ++i)
			{
				if ("AlbumOrder" == varList[i].name)
				{						
					if ("MaleFirst" == varList[i].sValue)
						m_maleFirst = true;	
					else
						m_maleFirst = false;
					break;
				}
			}
		}
		SetTimer(1, 200, NULL);
	}	

	return TRUE;
}

void CMusicStoreDlg::LoadLeanplumSDK()
{
	HMODULE m_leanplumDLL = LoadLibraryA("leanplumWSDK.dll");
	if (NULL != m_leanplumDLL)
	{
		pfnGetLeanplumWSDK getLeanplumWSDK = reinterpret_cast<pfnGetLeanplumWSDK> (GetProcAddress(m_leanplumDLL, "GetLeanplumWSDK"));
		if (getLeanplumWSDK)
			m_leanplum = getLeanplumWSDK();
	}
}

void CMusicStoreDlg::ReleaseLeanplumSDK()
{
	if (NULL != m_leanplumDLL)
	{
		OutputDebugStringA("Release leanplum SDK");
		FreeLibrary(m_leanplumDLL);
	}
}

void CMusicStoreDlg::InitializeControl(UINT buttonID, UINT bitmapID)
{
	CButton* pBtn= (CButton*)GetDlgItem(buttonID);
	pBtn->ModifyStyle(0, BS_BITMAP);

	HBITMAP hIcn = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
									MAKEINTRESOURCE(bitmapID),
									IMAGE_BITMAP,
									160, 170, // use actual size
									LR_DEFAULTCOLOR);
	pBtn->SetBitmap(hIcn);
}

void CMusicStoreDlg::InitializeAlbum(bool maleFirst)
{
	if (maleFirst)
	{
		InitializeControl(IDC_BUTTON_ALBUM1, IDB_BITMAP_M1);
		InitializeControl(IDC_BUTTON_ALBUM2, IDB_BITMAP_M2);
		InitializeControl(IDC_BUTTON_ALBUM3, IDB_BITMAP_M3);
		InitializeControl(IDC_BUTTON_ALBUM4, IDB_BITMAP_M4);
	}
	else
	{
		InitializeControl(IDC_BUTTON_ALBUM1, IDB_BITMAP_F1);
		InitializeControl(IDC_BUTTON_ALBUM2, IDB_BITMAP_F2);
		InitializeControl(IDC_BUTTON_ALBUM3, IDB_BITMAP_F3);
		InitializeControl(IDC_BUTTON_ALBUM4, IDB_BITMAP_F4);
	}
}

BOOL CMusicStoreDlg::PreTranslateMessage(MSG* pMsg)
{
     m_ToolTip.RelayEvent(pMsg);
     return CDialog::PreTranslateMessage(pMsg);
}

void CMusicStoreDlg::OnBnClickedOk()
{	
	if (m_leanplum)
	{
		m_leanplum->Stop();
	}
	ReleaseLeanplumSDK();
	CDialogEx::OnOK();
}

void CMusicStoreDlg::OnBnClickedAlbum(UINT nID)
{
	int value = 0;
	switch (nID)
	{
	case IDC_BUTTON_ALBUM1:
		value = 10;
		break;
	case IDC_BUTTON_ALBUM2:
		value = 20;
		break;
	case IDC_BUTTON_ALBUM3:
		value = 25;
		break;
	case IDC_BUTTON_ALBUM4:
		value = 19;
		break;
	}

	if (m_leanplum)
	{
		m_leanplum->Track("Purchase", value);
	}
}

void CMusicStoreDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (1 == nIDEvent)
	{	
		if (m_loadSuccess)
		{
			m_count = 0;
			GetDlgItem(IDC_BUTTON_ALBUM1)->ShowWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ALBUM2)->ShowWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ALBUM3)->ShowWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ALBUM4)->ShowWindow(TRUE);

			GetDlgItem(IDC_STATIC_MSG)->ShowWindow(FALSE);

			InitializeAlbum(m_maleFirst);
		}
		else
		{
			++m_count;
			if (0 == m_count%3)
			{
				GetDlgItem(IDC_STATIC_MSG)->SetWindowTextW(L"Loading.");
			}
			else if (1 == m_count%3)
			{
				GetDlgItem(IDC_STATIC_MSG)->SetWindowTextW(L"Loading..");
			}
			else
			{
				GetDlgItem(IDC_STATIC_MSG)->SetWindowTextW(L"Loading...");
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
