#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

#define CV_BLUR_NO_SCALE 0
#define CV_BLUR  1
#define CV_GAUSSIAN  2
#define CV_MEDIAN 3
#define CV_BILATERAL 4

using namespace cv;
using namespace std;

const char* catImage = "./assets/cat.jpg";
const char* eyeImage = "./assets/eye.jpg";

void example_1() {
	// задаём высоту и ширину картинки
	int height = 620;
	int width = 440;
	// задаём точку для вывода текста
	CvPoint pt = cvPoint(height / 4, width / 2);
	// Создаёи 8-битную, 3-канальную картинку
	IplImage* hw = cvCreateImage(cvSize(height, width), 8, 3);
	// заливаем картинку чёрным цветом
	cvSet(hw, cvScalar(0, 0, 0));
	// инициализация шрифта
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 1.0, 1.0, 0, 1, CV_AA);
	// используя шрифт выводим на картинку текст
	cvPutText(hw, "Hello, OpenCV", pt, &font, CV_RGB(255, 255, 255));
	// создаём окошко
	cvNamedWindow("Hello World", CV_WINDOW_AUTOSIZE);
	// показываем картинку в созданном окне
	cvShowImage("Hello World", hw);
	// ждём нажатия клавиши
	char c = cvWaitKey(0);
	if (c == 27) { // если нажали ESC 
		std::cout << "ESC pressed" << std::endl;
	}
	// освобождаем ресурсы
	cvReleaseImage(&hw);
	cvDestroyWindow("Hello World");
}

void example_2() {
	// получаем картинку
	IplImage* image = cvLoadImage(catImage, 1); // 0 - градации серого
	// клонируем картинку 
	IplImage* src = cvCloneImage(image);
	assert(src != 0);

	// показываем картинку
	cvShowImage("original", image);

	// выводим в консоль информацию о картинке
	printf("[i] channels:  %d\n", image->nChannels);
	printf("[i] pixel depth: %d bits\n", image->depth);
	printf("[i] width:       %d pixels\n", image->width);
	printf("[i] height:      %d pixels\n", image->height);
	printf("[i] image size:  %d bytes\n", image->imageSize);
	printf("[i] width step:  %d bytes\n", image->widthStep);

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&src);
}

void example_3() {
	// получаем картинку
	IplImage* image = cvLoadImage(catImage, 1);
	assert(image != 0);
	// клонируем картинку 
	IplImage* dst = cvCloneImage(image);

	// сглаживаем исходную картинку
	cvSmooth(image, dst, CV_BLUR, 3, 3); // CV_BLUR_NO_SCALE

	// показываем картинку
	cvShowImage("original", image);
	cvShowImage("smooth", dst);

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&dst);
}

void example_4() {
	// получаем картинку
	IplImage* image = cvLoadImage(catImage, 1);
	assert(image != 0);
	IplImage* dst;

	// создание уменьшенной картинки
	dst = cvCreateImage(cvSize(image->width / 3, image->height / 3), image->depth, image->nChannels);
	cvResize(image, dst, 1); // вариации интерполяции

	cvShowImage("original", image);
	cvShowImage("Example#", dst);

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&dst);
}

void example_5() {
	// получаем картинку
	IplImage* image = cvLoadImage(catImage, 1);
	assert(image != 0);

	// задаём ROI
	int x = 40;
	int y = 20;
	int width = 100;
	int height = 100;
	// добавочная величина 
	int add = 100;

	cvShowImage("origianl", image);
	// устанавливаем ROI
	cvSetImageROI(image, cvRect(x, y, width, height));
	cvAddS(image, cvScalar(add), image);
	// сбрасываем ROI
	cvResetImageROI(image);
	// показываем изображение
	cvShowImage("ROI", image);

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&image);
}

void matchTemplate() {
	IplImage* src = cvLoadImage(catImage, 1), 
		* templ = cvLoadImage(eyeImage, 1),
		* ftmp[6];
	assert(src != 0);
	assert(templ != 0);

	int patchx = templ->width;
	int patchy = templ->height;
	int iwidth = src->width - patchx + 1;
	int iheight = src->height - patchy + 1;

	for (int i = 0; i < 6; ++i) {
		ftmp[i] = cvCreateImage(cvSize(iwidth, iheight), 32, 1);
	}

	for (int i = 0; i < 6; ++i) {
		cvMatchTemplate(src, templ, ftmp[i], i);
		double minval, maxval;
		CvPoint minLoc, maxLoc;
		cvMinMaxLoc(ftmp[i], &minval, &maxval, &minLoc, &maxLoc, 0);
		cvNormalize(ftmp[i], ftmp[i], 1, 0, CV_MINMAX);
		cvRectangle(src, maxLoc, cvPoint(maxLoc.x + templ->width - 1, maxLoc.y + templ->height - 1), CV_RGB(255,0,0),1,8);
		cvRectangle(ftmp[i], maxLoc, cvPoint(maxLoc.x + templ->width - 1, maxLoc.y + templ->height - 1), CV_RGB(255, 0, 0), 1, 8);
	}

	cvShowImage("Template", templ);
	cvShowImage("Image", src);
	cvShowImage("0", ftmp[0]);
	cvShowImage("1", ftmp[1]);
	cvShowImage("2", ftmp[2]);
	cvShowImage("3", ftmp[3]);
	cvShowImage("4", ftmp[4]);
	cvShowImage("5", ftmp[5]);

	cvWaitKey(0);
}

void findTemp() {
	IplImage* src = cvLoadImage(catImage, 1),
		* templ = cvLoadImage(eyeImage, 1),
		* ftmp[6];
	assert(src != 0);
	assert(templ != 0);

	int patchx = templ->width;
	int patchy = templ->height;

	int iwidth = src->width - patchx + 1;
	int iheight = src->height - patchy + 1;

	cvShowImage("Template", templ);
	cvShowImage("Image", src);

	IplImage* res = cvCreateImage(cvSize(iwidth, iheight), 32, 1);
	cvMatchTemplate(src, templ, res, 0);

	double minval, maxval;
	CvPoint minLoc, maxLoc;
	cvMinMaxLoc(res, &minval, &maxval, &minLoc, &maxLoc, 0);
	cvNormalize(res, res, 1, 0, CV_MINMAX);

	cvRectangle(src, minLoc, cvPoint(minLoc.x + templ->width - 1, minLoc.y + templ->height - 1), CV_RGB(255, 0, 0), 1, 8);

	cvShowImage("Match", src);
	cvWaitKey(0);

	cvReleaseImage(&src);
	cvReleaseImage(&templ);
	cvReleaseImage(&res);

}

int main(){
	findTemp();

    return 0;
}
