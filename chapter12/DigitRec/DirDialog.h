/////////////////////////////////////////////////
// DirDialog.h文件

#ifndef __DIRDIALOG_H_
#define __DIRDIALOG_H_

#include <shlobj.h>

class CDirDialog
{
public:
	CDirDialog()
	{
		memset(&m_bi, 0, sizeof(m_bi));
		
		m_bi.hwndOwner = NULL;	 
		m_bi.pidlRoot = NULL;		 	
		m_bi.pszDisplayName = m_szDisplay; 
		m_bi.lpszTitle = NULL;		 	
		m_bi.ulFlags = BIF_RETURNONLYFSDIRS;
		
		m_szPath[0] = '\0';
	}


	// 显示对话框
	BOOL DoBrowse(HWND hWndParent, LPCTSTR pszTitle = NULL);
	// 取得用户选择的目录名称
	LPCTSTR GetPath() { return m_szPath; }

protected:
	BROWSEINFOA m_bi; 

	// 用来接受用户选择目录的缓冲区
	char m_szDisplay[MAX_PATH];
	char m_szPath[MAX_PATH];
};

inline BOOL CDirDialog::DoBrowse(HWND hWndParent, LPCTSTR pszTitle)
{
	if(pszTitle == NULL)
		m_bi.lpszTitle = "选择目标文件夹";
	else
		m_bi.lpszTitle = pszTitle;

	m_bi.hwndOwner = hWndParent;
	LPITEMIDLIST pItem = ::SHBrowseForFolder(&m_bi);
	if(pItem != 0)
	{
		::SHGetPathFromIDList(pItem, m_szPath);
		return TRUE;
	}

	return FALSE;
}

#endif //__DIRDIALOG_H_