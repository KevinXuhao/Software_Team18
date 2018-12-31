#include"stdafx.h"
#include<iostream>
using namespace std;
#include"./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main1() {
	//输入图像
	GDALDataset* poSrcDS;
	//输出图像
	GDALDataset* poDstDS;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//输入图像路径
	char* srcPath = "lena.jpg";
	//输出图像路径
	char* dstPath = "lena_1.tif";
	//图像内存存储
	float* srcBTmp;
	float* dstBTmp;
	//图像波段数
	int i, j, k, bandNum;
	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	//获取图像的宽度和高度
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//根据图像的宽度和高度分配内存
	srcBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	dstBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	for (k = 1;k < 4;k++) {
		poSrcDS->GetRasterBand(k)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, srcBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
		for (i = 1;i < imgXlen - 1;i++) {
			for (j = 1;j < imgYlen - 1;j++) {
				dstBTmp[i*imgXlen + j] = (float)((srcBTmp[(i - 1)*imgXlen + j] + srcBTmp[(i + 1)*imgXlen + j] + srcBTmp[i*imgXlen + j - 1] + srcBTmp[i*imgXlen + j + 1] + srcBTmp[i*imgXlen + j])*0.2);//上下左右自己0.2
			}
		}
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, dstBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
	}

	//清除内存
	CPLFree(srcBTmp);
	CPLFree(dstBTmp);
	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);
	
	system("PAUSE");
	return 0;

}

int main2() {
	//输入图像
	GDALDataset* poSrcDS;
	//输出图像
	GDALDataset* poDstDS;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//输入图像路径
	char* srcPath = "lena.jpg";
	//输出图像路径
	char* dstPath = "lena_2.tif";
	//图像内存存储
	float* srcBTmp;
	float* dstBTmp;
	//图像波段数
	int i, j, k, bandNum;
	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	//获取图像的宽度和高度
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//根据图像的宽度和高度分配内存
	srcBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	dstBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	for (k = 1;k<4;k++) {
		poSrcDS->GetRasterBand(k)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, srcBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
		for (i = 2;i<imgYlen - 2;i++) {
			for (j = 2;j<imgXlen - 2;j++) {
				dstBTmp[i*imgXlen + j] = (GByte)((srcBTmp[(i - 2)*imgXlen + j - 2] + srcBTmp[(i - 1)*imgXlen + j - 1] + srcBTmp[i*imgXlen + j] + srcBTmp[(i + 1)*imgXlen + j + 1] + srcBTmp[(i + 2)*imgXlen + j + 2])*0.2);
			}
		}
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, dstBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
	}

	//清除内存
	CPLFree(srcBTmp);
	CPLFree(dstBTmp);
	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	system("PAUSE");
}

int main3() {
	//输入图像
	GDALDataset* poSrcDS;
	//输出图像
	GDALDataset* poDstDS;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//输入图像路径
	char* srcPath = "lena.jpg";
	//输出图像路径
	char* dstPath = "lena_3.tif";
	//图像内存存储
	float* srcBTmp;
	float* dstBTmp;
	//图像波段数
	int i, j, k,  bandNum;
	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	//获取图像的宽度和高度
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//根据图像的宽度和高度分配内存
	srcBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	dstBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	for (k = 1;k<4;k++) {
		poSrcDS->GetRasterBand(k)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, srcBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
		for (i = 1;i <imgYlen - 1;i++) {
			for (j = 1;j < imgXlen - 1;j++) {
				dstBTmp[i*imgXlen + j] = (float)(
					-srcBTmp[(i - 1)*imgXlen + j - 1] - srcBTmp[(i - 1)*imgXlen + j] - srcBTmp[(i - 1)*imgXlen + j + 1]
					- srcBTmp[i*imgXlen + j - 1] + 8.0*srcBTmp[i*imgXlen + j] - srcBTmp[i*imgXlen + j + 1]
					- srcBTmp[(i + 1)*imgXlen + j - 1] - srcBTmp[(i + 1)*imgXlen + j] - srcBTmp[(i + 1)*imgXlen + j + 1]);//上下左右-1自己8
			}
		}
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, dstBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
	}	

	//清除内存
	CPLFree(srcBTmp);
	CPLFree(dstBTmp);
	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	system("PAUSE");

}

int main4() {
	//输入图像
	GDALDataset* poSrcDS;
	//输出图像
	GDALDataset* poDstDS;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//输入图像路径
	char* srcPath = "lena.jpg";
	//输出图像路径
	char* dstPath = "lena_4.tif";
	//图像内存存储
	float* srcBTmp;
	float* dstBTmp;
	//图像波段数
	int i, j, k, bandNum;
	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	//获取图像的宽度和高度
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//根据图像的宽度和高度分配内存
	srcBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	dstBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	for (k = 1; k<4; k++) {
		poSrcDS->GetRasterBand(k)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, srcBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
		for (i = 1;i < imgYlen - 1;i++) {
			for (j = 1;j < imgXlen - 1;j++) {
				dstBTmp[i*imgXlen + j] = (float)(
					-srcBTmp[(i - 1)*imgXlen + j - 1] - srcBTmp[(i - 1)*imgXlen + j] - srcBTmp[(i - 1)*imgXlen + j + 1]
					- srcBTmp[i*imgXlen + j - 1] + 9.0*srcBTmp[i*imgXlen + j] - srcBTmp[i*imgXlen + j + 1]
					- srcBTmp[(i + 1)*imgXlen + j - 1] - srcBTmp[(i + 1)*imgXlen + j] - srcBTmp[(i + 1)*imgXlen + j + 1]);//上下左右-1自己9
			}
		}
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, dstBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
	}

	//清除内存
	CPLFree(srcBTmp);
	CPLFree(dstBTmp);
	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	system("PAUSE");
}

int main5() {
	//输入图像
	GDALDataset* poSrcDS;
	//输出图像
	GDALDataset* poDstDS;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//输入图像路径
	char* srcPath = "lena.jpg";
	//输出图像路径
	char* dstPath = "lena_5.tif";
	//图像内存存储
	float* srcBTmp;
	float* dstBTmp;
	//图像波段数
	int i, j, k, bandNum;
	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	//获取图像的宽度和高度
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//根据图像的宽度和高度分配内存
	srcBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	dstBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	for (k = 1;k < 4;k++) {
		poSrcDS->GetRasterBand(k)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, srcBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
		for (i = 1;i < imgYlen - 1;i++) {
			for (j = 1;j < imgXlen - 1;j++) {
				dstBTmp[i*imgXlen + j] = (float)(
					-srcBTmp[(i - 1)*imgXlen + j - 1] - srcBTmp[(i - 1)*imgXlen + j] - srcBTmp[i*imgXlen + (j - 1)] + srcBTmp[i*imgXlen + (j + 1)]
					+ srcBTmp[(i + 1)*imgXlen + j] + srcBTmp[(i + 1)*imgXlen + j + 1]
					);
			}
		}
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, dstBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
	}
		
	//清除内存
	CPLFree(srcBTmp);
	CPLFree(dstBTmp);
	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	system("PAUSE");
}

int main6() {
	//输入图像
	GDALDataset* poSrcDS;
	//输出图像
	GDALDataset* poDstDS;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//输入图像路径
	char* srcPath = "lena.jpg";
	//输出图像路径
	char* dstPath = "lena_6.tif";
	//图像内存存储
	float* srcBTmp;
	float* dstBTmp;
	//图像波段数
	int i, j, k, bandNum;
	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	//获取图像的宽度和高度
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	//根据图像的宽度和高度分配内存
	srcBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	dstBTmp = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	for (k = 1;k < 4;k++) {
		poSrcDS->GetRasterBand(k)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, srcBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
		for (i = 2;i < imgYlen - 2;i++) {
			for (j = 2;j < imgXlen - 2;j++) {
				dstBTmp[i*imgXlen + j] = (float)(
					0.012*srcBTmp[(i - 2)*imgXlen + j - 2] + 0.1253*srcBTmp[(i - 2)*imgXlen + j - 1] + 0.2736*srcBTmp[(i - 2)*imgXlen + j] +0.1253*srcBTmp[(i - 2)*imgXlen + j + 1] + 0.012*srcBTmp[(i - 2)*imgXlen + j + 2] +
					0.1253*srcBTmp[(i - 1)*imgXlen + j - 2] +1.3054*srcBTmp[(i - 1)*imgXlen + j - 1] + 2.8514*srcBTmp[(i - 1)*imgXlen + j] + 1.3054*srcBTmp[(i - 1)*imgXlen + j + 1] + 0.1253*srcBTmp[(i - 1)*imgXlen + j + 2] +
					0.2736*srcBTmp[i*imgXlen + j - 2] + 2.8514*srcBTmp[i*imgXlen + j - 1] + 6.2279*srcBTmp[i*imgXlen + j] + 2.8514*srcBTmp[i*imgXlen + j + 1] + 0.2736*srcBTmp[i*imgXlen + j + 2] +
					0.1253*srcBTmp[(i + 1)*imgXlen + j - 2] + 1.3054*srcBTmp[(i + 1)*imgXlen + j - 1] + 2.8514*srcBTmp[(i + 1)*imgXlen + j] + 1.3054*srcBTmp[(i + 1)*imgXlen + j + 1] + 0.1253*srcBTmp[(i + 1)*imgXlen + j + 2] +
					0.012*srcBTmp[(i + 2)*imgXlen + j - 2] + 0.1253*srcBTmp[(i + 2)*imgXlen + j - 1] + 0.2736*srcBTmp[(i + 2)*imgXlen + j] + 0.1253*srcBTmp[(i + 2)*imgXlen + j + 1] + 0.0012*srcBTmp[(i + 2)*imgXlen + j + 2]
					)/25.0;
			}
		}
		poDstDS->GetRasterBand(k)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, dstBTmp, imgXlen, imgYlen, GDT_Float32, 0, 0);
	}

	//清除内存
	CPLFree(srcBTmp);
	CPLFree(dstBTmp);
	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	system("PAUSE");
}