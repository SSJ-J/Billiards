#include "draw.h"

/* Load Raw File for Terrain */
GLvoid GLhandlers::loadRawFile(LPSTR strName, GLuint nSize, BYTE *pHeightMap) {
	FILE *pFile = NULL;
	fopen_s(&pFile, strName, "rb");
	if (!pFile) {		// cannot open
		MessageBox(NULL, "���ܴ򿪸߶�ͼ�ļ�", "����", MB_OK);
		return;
	}

	fread(pHeightMap, sizeof(BYTE), nSize, pFile);
	if (ferror(pFile))	// faile to read
		MessageBox(NULL, "��ȡ����ʧ��", "����", MB_OK);

	fclose(pFile);
}