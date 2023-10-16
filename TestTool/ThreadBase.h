#pragma once

// CThreadBase

class CThreadBase : public CWinThread
{
	DECLARE_DYNCREATE(CThreadBase)

protected:
	CThreadBase();           // protected constructor used by dynamic creation
	virtual ~CThreadBase();
//ϵͳ���غ���
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

//�ӿڶ���
public:

	//��������
	inline void Lock(void)  {::EnterCriticalSection(&m_cs);}
	//��������
	inline void UnLock(void){::LeaveCriticalSection(&m_cs);}
protected:
	DECLARE_MESSAGE_MAP()
//�麯��,����̳�
protected:
	//��������������Դ
	virtual void fnReleaseResource(){}
	//��ȡ������ص�������Ϣ(��ָ�������ļ�)
	virtual void fnReadSpecailConfig(){}
//Attributes
public:
	//�߳��˳����
	BOOL	m_bExit;
protected:
	//��������
	HWND	m_hMainWnd;
	//�߳̾��
	UINT	m_nMainThreadID;
	//�߳��Ƿ������ڹ���״̬
	BOOL	m_bWorking;
	//�ٽ�����������
	CRITICAL_SECTION m_cs;
};


