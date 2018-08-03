#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>
#include <direct.h>
#include <string>
#include <iostream>

using namespace std;

DWORD WINAPI WatchChanges(LPVOID lpParameter);

//ThreadParameter�ṹ��Ķ��� 
/*
*���ϲ���Ϊһ���ṹ�崫�����̵߳�ԭ�����ڴ����߳�ʱ
*ָ�����߳���ں���ֻ����һ��LPVOID���͵�ָ�룬�������ݿ��Բο�msdn��
*ʵ���������̴߳��ݲ�������һ�ַ�����ȫ�ֱ�����
*/
typedef struct ThreadParameter
{
	LPTSTR in_directory;//��ص�·�� 
	FILE_NOTIFY_INFORMATION *in_out_notification;//�洢��غ���������Ϣ��ַ 
	DWORD in_MemorySize;//���ݴ洢������Ϣ���ڴ���ֽ��� 
	DWORD *in_out_BytesReturned;//�洢��غ���������Ϣ���ֽ��� 
	DWORD *in_out_version;//���ذ汾��Ϣ 
	FILE_NOTIFY_INFORMATION *temp_notification;//���õ�һ������ 
}ThreadParameter;

int main()
{
	//���ݸ�WatchChanges�����Ĳ���
	TCHAR* Directory = _T("F://");
	char notify[1024];
	memset(notify, '\0', 1024);
	FILE_NOTIFY_INFORMATION *Notification = (FILE_NOTIFY_INFORMATION *)notify;
	FILE_NOTIFY_INFORMATION *TempNotification = NULL;
	DWORD BytesReturned = 0;
	DWORD version = 0;


	//���ϴ������̵߳Ĳ������ýṹ��Ķ���ο�����Ķ��� 
	ThreadParameter ParameterToThread = { Directory,
		Notification,
		sizeof(notify),
		&BytesReturned,
		&version,
		TempNotification };

	//����һ���߳�ר�����ڼ���ļ��仯 
	HANDLE ThreadWatch = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)WatchChanges,
		&ParameterToThread,
		0,
		NULL);
	WaitForSingleObject(ThreadWatch, INFINITE);
	return 0;
}

//  ����: WatchChanges(LPVOID lpParameter) 
// 
//  Ŀ��: ���Ŀ¼�ĳ��� 
// 
//  ע��:�����������߳�ʱ�ƶ��������������� 
//       ��ʱ���ӳ����Զ�����ִ�С� 

DWORD WINAPI WatchChanges(LPVOID lpParameter)//���ذ汾��Ϣ 
{
	ThreadParameter *parameter = (ThreadParameter*)lpParameter;
	LPCTSTR WatchDirectory = parameter->in_directory;

	//����һ��Ŀ¼��� 
	HANDLE handle_directory = CreateFile(WatchDirectory,
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);
	if (handle_directory == INVALID_HANDLE_VALUE)
	{
		DWORD ERROR_DIR = GetLastError();
		MessageBox(NULL, TEXT("��Ŀ¼����!"), TEXT("�ļ����"), 0);
	}


	BOOL watch_state;
	int counter = 0;
	int i;

	while (TRUE)
	{
		watch_state = ReadDirectoryChangesW(handle_directory,
			(LPVOID)parameter->in_out_notification,
			parameter->in_MemorySize,
			TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE| FILE_NOTIFY_CHANGE_LAST_ACCESS,
			(LPDWORD)parameter->in_out_BytesReturned,
			NULL,
			NULL);
		printf("########%d########\n",watch_state);//�����Ϊ1�����������ѳɹ�ִ�� 


		if (GetLastError() == ERROR_INVALID_FUNCTION)
		{
			MessageBox(NULL,
				TEXT("ϵͳ��֧��!"),
				TEXT("�ļ����"),
				0);
		}
		else if (watch_state == 0)
		{
			MessageBox(NULL,
				TEXT("���ʧ��!"),
				TEXT("�ļ����"),
				0);
		}
		else if (GetLastError() == ERROR_NOTIFY_ENUM_DIR)
		{
			MessageBox(NULL,
				TEXT("�ڴ����!"),
				TEXT("�ļ����"),
				0);
		}
		else
		{
			//�����ַ����͵�FileName����ת����string������д��log�ļ�������д����ȥ��ȷ���ļ��� 
			string file_name;
			DWORD length = WideCharToMultiByte(0,
				0,
				parameter->in_out_notification->FileName,
				-1,
				NULL,
				0,
				NULL,
				NULL);
			PSTR ps = new CHAR[length];
			if (length >= 0)
			{
				WideCharToMultiByte(0,
					0,
					parameter->in_out_notification->FileName,
					-1,
					ps,
					length,
					NULL,
					NULL);
				file_name = string(ps);
				delete[] ps;
			}

			//������Ҫ���Ǽ�ⷵ�ص���Ϣ����Ҫע��FILE_NOTIFY_INFORMATION�ṹ��Ķ��壬�Ա���ȷ���÷�����Ϣ 
				if (parameter->in_out_notification->Action == FILE_ACTION_ADDED)
				{
					cout << "�����ļ� : " << file_name << "\n" << flush;

					//��ȡ�ļ�·��������ʽת��LPCWSTR��ʽ���ƶ��������ļ�
					char buffer1[MAX_PATH] = "f:\\test\\";
					strcat(buffer1, file_name.c_str());
					char buffer2[MAX_PATH] = "f:\\gwe\\";
					strcat(buffer2, file_name.c_str());

					WCHAR source[256];
					WCHAR dest[256];
					memset(source, 0, sizeof(source));
					memset(dest, 0, sizeof(dest));
					MultiByteToWideChar(CP_ACP,
						0,
						buffer1,
						strlen(buffer1) + 1,
						source,
						sizeof(source) / sizeof(source[0]));
					MultiByteToWideChar(CP_ACP,
						0,
						buffer2,
						strlen(buffer2) + 1,
						dest,
						sizeof(dest) / sizeof(dest[0]));

					if (MoveFile(source, dest))
						cout << "moved successed!" << endl;


				}
				//if (parameter->in_out_notification->Action == FILE_ACTION_REMOVED)
				//{
				//WriteLog << ctime(&ChangeTime) << "ɾ���ļ� : " << file_name << "\n" << flush;
				//}

				if (parameter->in_out_notification->Action == FILE_ACTION_MODIFIED)
				{
					counter++;
					cout <<"�ļ�"<< file_name<<"�ķ��ʴ���Ϊ " << counter << "\n" << flush;
				}


				/*if (parameter->in_out_notification->Action == FILE_ACTION_MODIFIED)
				{
					counter++;
					cout << "���ʴ���Ϊ "<<counter<<"\n" << endl;*/
					//edit_flag++;
					//if (edit_flag == 1)
					//WriteLog << ctime(&ChangeTime) << "�޸��ļ� : " << file_name << "\n" << flush;
					//else if (edit_flag == 2)
					//{
					//edit_flag = 0;
					//(*(parameter->in_out_version))--;
					//}
					//else
					//return -1;//break;
					//}


					////�����������������Action����Ҳ���ļ�����������old_nameҲ������new_name��
				/*	if (parameter->in_out_notification->Action == FILE_ACTION_RENAMED_OLD_NAME)
					{
					WriteLog << ctime(&ChangeTime) << "������\"" << file_name << "\"�ļ�\n" << flush;
					}
					if (parameter->in_out_notification->Action == FILE_ACTION_RENAMED_NEW_NAME)
					{
					WriteLog << ctime(&ChangeTime) << "������\"" << file_name << "\"�ļ�Ϊ\"" << parameter->in_out_notification->Action << "\"\n" << flush;

					}*/
				(*(parameter->in_out_version))++;
				memset(parameter->in_out_notification, '\0', 1024);
			}
		Sleep(500);
	}
	return 0;
}
