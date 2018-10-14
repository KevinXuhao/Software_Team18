

#include<iostream>
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")
using namespace std;
int main()
{
	//输入图像
	GDALDataset* poSrcDS;
	GDALDataset* poDstDS;

	//图像的宽度和高度
	int imgXlen, imgYlen;
	//输入图像路径
	char* srcPath = "trees.jpg";


	//输出图像路径
	char* dstPath = "res.tif";

	//图像内存存储
	GByte* buffTmp;
	//图像波段数
	int i, bandNum;
	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

	//获取图像宽度,高度和波段数
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();
	//输出获取的结果
	cout << "Image X length:" << imgXlen << endl;
	cout << "Image Y length:" << imgYlen << endl;
	cout << "Band Number:" << bandNum << endl;

	buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));

	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	for (i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		printf("... ... band %d processing ... ...\n", i);
	}
	CPLFree(buffTmp);
	GDALClose(poDstDS);
	GDALClose(poSrcDS);
	system("PAUSE");
	return 0;




}

