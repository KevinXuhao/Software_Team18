// sixthDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime> //
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")



int main()
{
	//输入图像
	GDALDataset* poSrcDS_MUL;
	GDALDataset* poSrcDS_PAN;
	//输出图像
	GDALDataset* poDstDS;
	//图像的宽度和高度
	int imgXlen, imgYlen;
	//输入图像路径
	char* srcPath_MUL = "Mul_large.tif";//多光谱图
	char* srcPath_PAN = "Pan_large.tif";//全色图像
	//输出图像路径
	char* dstPath = "Blending_Image.tif";
	//图像波段数
	int bandNum;
	// 设立时间
	clock_t startTime, endTime;

	//注册驱动
	GDALAllRegister();

	//打开图像
	poSrcDS_MUL = (GDALDataset*)GDALOpenShared(srcPath_MUL, GA_ReadOnly);
	poSrcDS_PAN = (GDALDataset*)GDALOpenShared(srcPath_PAN, GA_ReadOnly);

	//获取图像宽度，高度和波段数量
	imgXlen = poSrcDS_MUL->GetRasterXSize();
	imgYlen = poSrcDS_MUL->GetRasterYSize();
	bandNum = poSrcDS_MUL->GetRasterCount();
	//输出获取的结果
	cout <<" Image X Length: " << imgXlen << endl;
	cout <<" Image Y Length: " << imgYlen << endl;
	cout <<" Band number: " << bandNum << endl;
	
	

	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	int col = imgYlen / 256; //纵向块数
	int num = 0; //记录正在处理第几个分块

	//根据图像的宽度和高度分配内存
	//得到RGB
	float* buff_R = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	float* buff_G = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	float* buff_B = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	//得到IHS
	float* buffTmp = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	float* buff_H = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	float* buff_S = (float*)CPLMalloc(imgXlen*265 * sizeof(float));
	
	//设置大小为imgXlen×256的分块
	startTime = clock(); //开始分块处理

		for (int n = 0; n < col; n++)
		{
			num++;
			cout << "正在处理第 " << num << " 个分块" << endl;
			//读取图像每一个波段的像素点
			poSrcDS_MUL->GetRasterBand(1)->RasterIO(GF_Read, 0, 256 * n, imgXlen, 256,
				buff_R, imgXlen, 256, GDT_Float32, 0, 0);

			poSrcDS_MUL->GetRasterBand(2)->RasterIO(GF_Read, 0, 256 * n, imgXlen, 256,
				buff_G, imgXlen, 256, GDT_Float32, 0, 0);

			poSrcDS_MUL->GetRasterBand(3)->RasterIO(GF_Read, 0, 256 * n, imgXlen, 256,
				buff_B, imgXlen, 256, GDT_Float32, 0, 0);

			//全色图只有一个通道
			poSrcDS_PAN->GetRasterBand(1)->RasterIO(GF_Read, 0, 256 * n, imgXlen, 256,
				buffTmp, imgXlen, 256, GDT_Float32, 0, 0);

			for (int i = 0; i < imgXlen*256; i++)
			{
				//进行 RGB ==> IHS 正变换
				buff_H[i] = -sqrt(2.0f) / 6.0f*buff_R[i] - sqrt(2.0f) / 6.0f*buff_G[i] + sqrt(2.0f) / 3.0f*buff_B[i];
				buff_S[i] = 1.0f / sqrt(2.0f)*buff_R[i] - 1 / sqrt(2.0f)*buff_G[i];
				/*添加bandP之后还原到RGB*/
				buff_R[i] = buffTmp[i] - 1.0f / sqrt(2.0f)*buff_H[i] + 1.0f / sqrt(2.0f)*buff_S[i];
				buff_G[i] = buffTmp[i] - 1.0f / sqrt(2.0f)*buff_H[i] - 1.0f / sqrt(2.0f)*buff_S[i];
				buff_B[i] = buffTmp[i] + sqrt(2.0f)*buff_H[i];
			}

			//输出图像的读取
			poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 256 * n, imgXlen, 256,
				buff_R, imgXlen, 256, GDT_Float32, 0, 0);
			poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 0, 256 * n, imgXlen, 256,
				buff_G, imgXlen, 256, GDT_Float32, 0, 0);
			poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 0, 256 * n, imgXlen, 256,
				buff_B, imgXlen, 256, GDT_Float32, 0, 0);
		}

	endTime = clock();//结束分块处理

	cout << "遥感图像的分块处理用时：" <<endl;
	cout << (double)(endTime - startTime) / CLOCKS_PER_SEC << " s " << endl;

	//清除内存
	CPLFree(buffTmp);
	CPLFree(buff_R);
	CPLFree(buff_G);
	CPLFree(buff_B);
	CPLFree(buff_H);
	CPLFree(buff_S);
	//关闭dataset
	GDALClose(poSrcDS_MUL);
	GDALClose(poSrcDS_PAN);
	GDALClose(poDstDS);
	system("PAUSE");
	return 0 ;
}
