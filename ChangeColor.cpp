#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
using namespace std;

int main1()
{
	GDALDataset* poSrcDS;
	GDALDataset* poDstDS;
	char* srcPath = "54square.jpg";
	char* dstPath = "54square.tif";
	//Í¼ÏñÄÚ´æ´æ´¢
	GByte* buffTmp;
	//Í¼Ïñ²¨¶ÎÊý
	int i,j, bandNum;
	//×¢²áÇý¶¯
	GDALAllRegister();
	//´ò¿ªÍ¼Ïñ
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	int startX = 100, startY = 100, tmpXlen = 200, tmpYlen = 150;
	buffTmp = (GByte*)CPLMalloc(tmpXlen*tmpYlen * sizeof(GByte));
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, 300, 300,1, GDT_Byte, NULL);
	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	for(j=0;j<tmpYlen;j++)
		for (i = 0; i < tmpXlen; i++)
		{
			buffTmp[j*tmpXlen + i] = (GByte)255;
		}

	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	CPLFree(buffTmp);
	GDALClose(poSrcDS);
	GDALClose(poDstDS);
	system("PAUSE");
	return 0;


}