#include "StdAfx.h"
#include "Ini.h"


Ini::Ini(void)
{
	ZeroMemory(this->PATH,sizeof(this->PATH));
}

char* Ini::GetPath(void)
{
	return this->PATH;
}

int Ini::GetIntKey(IN char *File,IN char *section,IN char *key)
{
	::GetModuleFileName(NULL,this->GetPath(),MAX_PATH);
	*(strrchr(this->GetPath(),'\\')+1)=0x00;
	strcat_s(this->GetPath(),MAX_PATH,File);
	return ::GetPrivateProfileInt(section,key,0,this->GetPath());
	
}

 void Ini::GetStrKey(IN char *File,IN char *section,IN char *key,OUT char *pOutBuffer)
{
	::GetModuleFileName(NULL,this->GetPath(),MAX_PATH);
	*(strrchr(this->GetPath(),'\\')+1)=0x00;
	strcat_s(this->GetPath(),MAX_PATH,File);
	GetPrivateProfileString(section,key,0,pOutBuffer,MAX_PATH,this->GetPath());
}

Ini::~Ini(void)
{
}


