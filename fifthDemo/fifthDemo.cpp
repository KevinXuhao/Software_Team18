#include"stdafx.h"
#include<iostream>
using namespace std;
#include"./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main() {
	//输入图像
	GDALDataset* poSrcDS_mul;
	GDALDataset* poSrcDS_pan;
	//输出图像
	GDALDataset* poDstDS;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//输入图像路径
	char* srcPath1 = "American_mul.bmp";
	char* srcPath2 = "American_pan.bmp";
	//输出图像路径
	char* dstPath = "American_FUS.tif";
	//图像内存存储
	float* buffTmp_r,* buffTmp_g,*buffTmp_b;
	float* buffTmp_i,*buffTmp_h,*buffTmp_s;
	//图像波段数
	int i, j, k, b,bandNum;
	//注册驱动
	GDALAllRegister();
	//打开图像
	poSrcDS_mul = (GDALDataset*)GDALOpenShared(srcPath1, GA_ReadOnly);
	poSrcDS_pan = (GDALDataset*)GDALOpenShared(srcPath2, GA_ReadOnly);
	//获取图像的宽度和高度
	imgXlen = poSrcDS_mul->GetRasterXSize();
	imgYlen = poSrcDS_mul->GetRasterYSize();
	bandNum = poSrcDS_mul->GetRasterCount();
	//根据图像的宽度和高度分配内存
	
	buffTmp_r = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	buffTmp_g = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	buffTmp_b = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	buffTmp_i = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	buffTmp_h = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	buffTmp_s = (float*)CPLMalloc(imgXlen*imgYlen * sizeof(float));
	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	
	poSrcDS_mul->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp_r, imgXlen, imgYlen, GDT_Float32, 0, 0);
	poSrcDS_mul->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp_g, imgXlen, imgYlen, GDT_Float32, 0, 0);
	poSrcDS_mul->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp_b, imgXlen, imgYlen, GDT_Float32, 0, 0);
	poSrcDS_pan->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp_i, imgXlen, imgYlen, GDT_Float32, 0, 0);
	//进行RGB-》IHS正变换
	
		for (i = 0;i < imgYlen*imgXlen;i++) {
				buffTmp_h[i] = (-sqrt(2.0)/6) * buffTmp_r[i] + (-sqrt(2.0) / 6 * buffTmp_g[i]) +(sqrt(2.0)/3  * buffTmp_b[i]);
				buffTmp_s[i] = 1 /sqrt(2.0) * buffTmp_r[i] + (-1/ sqrt(2.0)*buffTmp_g[i]);

				buffTmp_r[i]=buffTmp_i[i] +(-1/sqrt(2.0))*buffTmp_h[i] +1/sqrt(2.0)*buffTmp_s[i];
				buffTmp_g[i] =1* buffTmp_i[i] - 1 / sqrt(2.0) *buffTmp_h[i] +  (-1)/sqrt(2.0) *buffTmp_s[i];
				buffTmp_b[i] = 1/sqrt(2.0)*buffTmp_i[i] +sqrt(2.0) *buffTmp_h[i];
		}
		poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp_r, imgXlen, imgYlen, GDT_Float32, 0, 0);
		poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp_g, imgXlen, imgYlen, GDT_Float32, 0, 0);
		poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp_b, imgXlen, imgYlen, GDT_Float32, 0, 0);

		CPLFree(buffTmp_r);
		CPLFree(buffTmp_g);
		CPLFree(buffTmp_b);
		CPLFree(buffTmp_i);
		CPLFree(buffTmp_h);
		CPLFree(buffTmp_s);

		GDALClose(poDstDS);
		GDALClose(poSrcDS_mul);
		GDALClose(poSrcDS_pan);

		system("pause");
		return 0;
}