#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* poSrcDS;
	GDALDataset* poDstDS;
	char* srcPath = "54square.jpg";
	char* dstPath = "54square1.jpg";
	//图像内存存储
	GByte* buffTmp;
	GByte* buffCopy;
	//图像波段数
	int i,j, bandNum;
	int imgXlen, imgYlen;
	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();
	int startX = 100, startY = 100, tmpXlen = 200, tmpYlen = 150;
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//buffcopy用于将原图像转存储到podstds中
	buffCopy = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmp = (GByte*)CPLMalloc(tmpXlen*tmpYlen * sizeof(GByte));

	//把原图像copy存储到poDstDS中，注意buffCopy的大小声明，必须为原图像的大小
	for (i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffCopy, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffCopy, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	
	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	//将要改变的位置颜色信息存入到bufftmp中
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
