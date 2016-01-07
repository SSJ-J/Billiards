#include "draw.h"

/* Load Raw File for Terrain */
GLvoid GLhandlers::loadRawFile(LPSTR strName, GLuint nSize, BYTE *pHeightMap) {
	FILE *pFile = NULL;
	fopen_s(&pFile, strName, "rb");
	if (!pFile) {		// cannot open
		MessageBox(NULL, "不能打开高度图文件", "错误", MB_OK);
		return;
	}

	fread(pHeightMap, sizeof(BYTE), nSize, pFile);
	if (ferror(pFile))	// faile to read
		MessageBox(NULL, "读取数据失败", "错误", MB_OK);

	fclose(pFile);
}