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
	// имя картинки задаётся первым параметром
	const char* filename = "./assets/cat.jpg";
	// получаем картинку
	IplImage* image = cvLoadImage(filename, 1); // 0 - градации серого
	// клонируем картинку 
	IplImage* src = cvCloneImage(image);

	assert(src != 0);

	// окно для отображения картинки
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);

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
	// удаляем окно
	cvDestroyWindow("original");
}

void example_3() {
	// имя картинки задаётся первым параметром
	const char* filename = "./assets/cat.jpg";
	// получаем картинку
	IplImage* image = cvLoadImage(filename, 1);
	// клонируем картинку 
	IplImage* dst = cvCloneImage(image);

	printf("[i] image: %s\n", filename);
	assert(image != 0);

	// окно для отображения картинки
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("smooth", CV_WINDOW_AUTOSIZE);

	// сглаживаем исходную картинку
	cvSmooth(image, dst, CV_BLUR_NO_SCALE, 3, 3); // CV_BLUR_NO_SCALE

	// показываем картинку
	cvShowImage("original", image);
	cvShowImage("smooth", dst);

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&dst);
	// удаляем окно
	cvDestroyWindow("original");
	cvDestroyWindow("smooth");
}

void example_4() {
	// имя картинки задаётся первым параметром
	const char* filename = "./assets/cat.jpg";
	// получаем картинку
	IplImage* image = cvLoadImage(filename, 1);
	IplImage* dst;
	assert(image != 0);

	// создание уменьшенной картинки
	dst = cvCreateImage(cvSize(image->width / 3, image->height / 3), image->depth, image->nChannels);
	cvResize(image, dst, 1); // вариации интерполяции

	// окно для отображения картинки
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvShowImage("original", image);

	// показываем результат
	cvNamedWindow("Example#", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example#", dst);

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&dst);

	// удаляем окна
	cvDestroyAllWindows();
}

void example_5() {
	// имя картинки задаётся первым параметром
	const char* filename = "./assets/cat.jpg";
	// получаем картинку
	IplImage* image = cvLoadImage(filename, 1);	
	assert(image != 0);

	cvNamedWindow("origianl", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("ROI", CV_WINDOW_AUTOSIZE);

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
	cvDestroyAllWindows();
}


int main(){
	example_5();
    return 0;
}
