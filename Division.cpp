#include <iostream>
#include <cmath>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")


int main()
{
	char* mulPath = "Image6/Mul_large.tif";
	char* panPath = "Image6/Pan_large.tif";
	char* fusPath = "Image6/American_Fus.tif";

	GDALAllRegister();

	// basic parameters
	GDALDataset *poMulDS, *poPanDS, *poFusDS;
	int imgXlen, divYlen,imgYlen;
	int i,j;
	float *bandR, *bandG, *bandB;
	float *bandI, *bandH, *bandS;
	float *bandP;

	// open datasets
	poMulDS = (GDALDataset*)GDALOpenShared(mulPath, GA_ReadOnly);
	poPanDS = (GDALDataset*)GDALOpenShared(panPath, GA_ReadOnly);
	imgXlen = poMulDS->GetRasterXSize();
	imgYlen = poMulDS->GetRasterYSize();
	divYlen = 256;
	poFusDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		fusPath, imgXlen, imgYlen, 3, GDT_Byte, NULL);


	printf(" ... ... image xlen : %d pixels ... ...\n", imgXlen);
	printf(" ... ... image ylen : %d pixles ... ...\n", divYlen);
	printf(" ... ... Mul image band number : %d ...\n", poMulDS->GetRasterCount());
	printf(" ... ... Pan image band number : %d ...\n", poPanDS->GetRasterCount());

	// allocating memory
	bandR = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	bandG = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	bandB = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	bandP = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	bandI = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	bandH = (float*)CPLMalloc(imgXlen*256 * sizeof(float));
	bandS = (float*)CPLMalloc(imgXlen*256 * sizeof(float));

	//分块，以imgXlen * 256为分块大小进行纵向的分块
	for (j = 0; j < imgYlen / divYlen;j++)
	{
		//读取每一分块的像素点信息存储到数据集
		poMulDS->GetRasterBand(1)->RasterIO(GF_Read, 0, divYlen*j, imgXlen, divYlen,
			bandR, imgXlen, divYlen, GDT_Float32, 0, 0);
		poMulDS->GetRasterBand(2)->RasterIO(GF_Read, 0, divYlen*j, imgXlen, divYlen,
			bandG, imgXlen, divYlen, GDT_Float32, 0, 0);
		poMulDS->GetRasterBand(3)->RasterIO(GF_Read, 0, divYlen*j, imgXlen, divYlen,
			bandB, imgXlen, divYlen, GDT_Float32, 0, 0);
		poPanDS->GetRasterBand(1)->RasterIO(GF_Read, 0, divYlen*j, imgXlen, divYlen,
			bandP, imgXlen, divYlen, GDT_Float32, 0, 0);

		//图像融合
		for (i = 0; i < imgXlen*divYlen; i++)
		{
			bandH[i] = -sqrt(2.0f) / 6.0f*bandR[i] - sqrt(2.0f) / 6.0f*bandG[i] + sqrt(2.0f) / 3.0f*bandB[i];
			bandS[i] = 1.0f / sqrt(2.0f)*bandR[i] - 1 / sqrt(2.0f)*bandG[i];

			bandR[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] + 1.0f / sqrt(2.0f)*bandS[i];
			bandG[i] = bandP[i] - 1.0f / sqrt(2.0f)*bandH[i] - 1.0f / sqrt(2.0f)*bandS[i];
			bandB[i] = bandP[i] + sqrt(2.0f)*bandH[i];
		}

		//将融合后的分块数据写入到最终的融合文件中
		poFusDS->GetRasterBand(1)->RasterIO(GF_Write, 0, divYlen*j, imgXlen, divYlen,
			bandR, imgXlen, divYlen, GDT_Float32, 0, 0);
		poFusDS->GetRasterBand(2)->RasterIO(GF_Write, 0, divYlen*j, imgXlen, divYlen,
			bandG, imgXlen, divYlen, GDT_Float32, 0, 0);
		poFusDS->GetRasterBand(3)->RasterIO(GF_Write, 0, divYlen*j, imgXlen, divYlen,
			bandB, imgXlen, divYlen, GDT_Float32, 0, 0);
		printf("Division %d is completed.\n", j + 1);
	}
	CPLFree(bandR);
	CPLFree(bandG);
	CPLFree(bandB);
	CPLFree(bandI);
	CPLFree(bandH);
	CPLFree(bandS);
	CPLFree(bandP);

	GDALClose(poMulDS);
	GDALClose(poPanDS);
	GDALClose(poFusDS);


	return 0;
}
