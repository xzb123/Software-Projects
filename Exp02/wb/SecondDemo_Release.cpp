// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

// SecondDemo_Release.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main()
{
	//输入图像
	GDALDataset* poSrcDS;
	//输出图像
	GDALDataset* poDstDS;
	//开始的坐标
	int StartX, StartY;
	//需要改变的区域的大小
	int tmpXlen, tmpYlen;
	//图像的高度和宽度
	int imgXlen, imgYlen;
	//输入图像路径
	const char* srcPath = "MayForthSquare.jpg";
	//输出图像路径
	const char* dstPath = "WithoutRedChannel3.jpg";
	//图像内存存储
	GByte* buffTmp;
	int i, j, bandNum;
	//注册驱动
	GDALAllRegister();
	//打开图像,poSrcDS初始化
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	//获取图像的宽度、高度和通道数
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

		//起始位置坐标
		StartX = 184/*100*/;
		StartY = 485/*100*/;
		//区域宽度和高度
		tmpXlen = 200;
		tmpYlen = 150;

	//分配内存
	buffTmp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	//创建输出图像,poDstDS初始化
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create
	(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	//一个个波段的输入，然后一个个波段的输出
	for (i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read,
			0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write,
			0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	CPLFree(buffTmp);

		buffTmp = (GByte*)CPLMalloc(tmpXlen * tmpYlen * sizeof(GByte));
		//读取红色通道缓存在buffTmp中
		poSrcDS->GetRasterBand(1)->RasterIO(GF_Read,
			StartX, StartY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
		//遍历区域，逐像素置为255
		for (j = 0; j < tmpYlen; j++)
			for (i = 0; i < tmpXlen; i++)
				buffTmp[j * tmpXlen + i] = (GByte)255;
		//数据写入poDstDS
		poDstDS->GetRasterBand(1)->RasterIO(GF_Write,
			StartX, StartY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);

	//清除内存
	CPLFree(buffTmp);
	//关闭dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);
	system("PAUSE");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
