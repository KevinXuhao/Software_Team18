#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
using namespace std;

int main()
{
	GDALDataset* poSrcDS;
	GDALDataset* poDstDS;
	char* srcPath = "54square.jpg";
	char* dstPath = "54square2.jpg";
	//图像内存存储
	GByte* buffTmp1;
	GByte* buffTmp2;
	GByte* buffCopy;
	//图像波段数
	int i, j, bandNum;
	int imgXlen, imgYlen;
	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();
	int startX = 300, startY = 300, tmpXlen = 100, tmpYlen = 50;
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//buffcopy用于将原图像转存储到podstds中
	buffCopy = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	buffTmp1 = (GByte*)CPLMalloc(tmpXlen*tmpYlen * sizeof(GByte));

	//把原图像copy存储到poDstDS中，注意buffCopy的大小声明，必须为原图像的大小
	for (i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffCopy, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffCopy, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	//poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, startX, startY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	//将要改变的位置颜色信息存入到bufftmp中
	//第一个方块为白色，将bufftmp设置为255值，然后将三个颜色通道的值都设为255，即得到白色方框
	for (j = 0; j<tmpYlen; j++)
		for (i = 0; i < tmpXlen; i++)
		{
			buffTmp1[j*tmpXlen + i] = (GByte)255;
		}

	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp1, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp1, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp1, tmpXlen, tmpYlen, GDT_Byte, 0, 0);


	//第二个方块为黑色，将bufftmp设置为0值，然后将三个颜色通道的值都设为0，即得到黑色方框
	startX = 500, startY = 500, tmpXlen = 50; tmpYlen = 100;
	buffTmp2 = (GByte*)CPLMalloc(tmpXlen*tmpYlen * sizeof(GByte));
	for (j = 0; j<tmpYlen; j++)
		for (i = 0; i < tmpXlen; i++)
		{
			buffTmp2[j*tmpXlen + i] = (GByte)0;
		}
	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp2, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp2, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, startX, startY, tmpXlen, tmpYlen, buffTmp2, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	
	CPLFree(buffTmp1);
	CPLFree(buffTmp2);
	CPLFree(buffCopy);
	GDALClose(poSrcDS);
	GDALClose(poDstDS);
	system("PAUSE");
	return 0;


}
