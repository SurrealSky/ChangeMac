#pragma once
class Ini
{
public:
	Ini(void);
	~Ini(void);
public:
	int GetIntKey(IN char *File,IN char *section,IN char *key);
	void GetStrKey(IN char *File,IN char *section,IN char *key,OUT char *pOutBuffer);
	char *GetPath(void);
private:
	char PATH[MAX_PATH];
};

