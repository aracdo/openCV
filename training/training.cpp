#include <iostream>
#include <opencv2/opencv.hpp>
#include <ViZDoom.h>
#include <vector>
#include <string>
#include "windows.h"



vizdoom::DoomGame* game = new vizdoom::DoomGame();
auto screenBuff = cv::Mat(480, 640, CV_8UC3);
unsigned int sleepTime = 10000;
std::string path = "C:\\practice\\vizdoom";

void RunTaskTest(int episodes) {

	try
	{
		game->loadConfig(path + "\\scenarios\\basic.cfg");
		game->init();
	}
	catch (std::exception& err)
	{
		std::cout << err.what() << std::endl;
	}

	std::vector<double> actions[3];
	actions[0] = { 1,0,1 };
	actions[1] = { 0,1,1 };
	actions[2] = { 0,0,1 };

	for (auto i = 0; i < episodes; i++)
	{
		game->newEpisode();
		std::cout << "episode " << i << std::endl;
		while (!game->isEpisodeFinished()) {

			auto gamestate = game->getState();
			auto screenBuf = gamestate->screenBuffer;
			for (int j = 0; j < 30; j++) {
				game->makeAction(actions[0]);
				Sleep(sleepTime);
			}
			for (int j = 0; j < 60; j++) {
				game->makeAction(actions[1]);
				Sleep(sleepTime);
			}
			
		}
	}
}

void RunTask1(int episodes) {


	try
	{
		game->setWindowVisible(true);
		game->setRenderWeapon(true);
		game->loadConfig(path + "\\scenarios\\task1.cfg");
		game->init();
	}
	catch (std::exception& err)
	{
		std::cout << err.what() << std::endl;
	}

	std::vector<double> actions[3];
	actions[0] = { 1,0,0 };
	actions[1] = { 0,1,0 };
	actions[2] = { 0,0,1 };

	int start, end, center;
	start = -100;

	auto greyscale = cv::Mat(480, 640, CV_8UC1);
	for (auto i = 0; i < episodes; i++)
	{
		game->newEpisode();
		//std::cout << "Episode #" << i + 1 << std::endl;

		while (!game->isEpisodeFinished())
		{
			const auto& gamestate = game->getState();
			std::memcpy(screenBuff.data, gamestate->screenBuffer->data(), gamestate->screenBuffer->size());


			cv::extractChannel(screenBuff, greyscale, 2);
			cv::threshold(greyscale, greyscale, 150, 255, cv::THRESH_BINARY);
			//cv::GaussianBlur(greyscale, greyscale, cv::Size(21, 21), 0, 0);
			cv::imshow("Output Window", greyscale);


			//std::cout << reward << " ";


			for (int x = 0; x < 640; x++) {
				if (greyscale.at<unsigned char>(225, x) > 130) {
					if (start < 0) start = x;
					end = x;
				}
			}
			std::cout << start + (end - start) / 2 << std::endl;
			center = start + (end - start) / 2;
			if (center < 269) double reward = game->makeAction({ 1,0,0 });
			else if (center > 290) double reward = game->makeAction({ 0,1,0 });
			else if (center >= 269 && center <= 290) double reward = game->makeAction({ 0,0,1 });

			cv::waitKey(sleepTime);
		}



	}
}


void RunTask2(int episodes) {

	try
	{
		game->setWindowVisible(true);
		game->setRenderWeapon(true);
		game->loadConfig(path + "\\scenarios\\task2.cfg");
		game->init();
	}
	catch (std::exception& err)
	{
		std::cout << err.what() << std::endl;
	}


	int start, end, center;
	start = -100;

	auto greyscale = cv::Mat(480, 640, CV_8UC1);
	for (auto i = 0; i < episodes; i++)
	{
		game->newEpisode();
		//std::cout << "Episode #" << i + 1 << std::endl;

		while (!game->isEpisodeFinished())
		{
			const auto& gamestate = game->getState();
			std::memcpy(screenBuff.data, gamestate->screenBuffer->data(), gamestate->screenBuffer->size());


			cv::extractChannel(screenBuff, greyscale, 2);
			cv::threshold(greyscale, greyscale, 150, 255, cv::THRESH_BINARY);
			//cv::GaussianBlur(greyscale, greyscale, cv::Size(21, 21), 0, 0);
			cv::imshow("Output Window", greyscale);


			//std::cout << reward << " ";


			for (int x = 0; x < 640; x++) {
				if (greyscale.at<unsigned char>(225, x) > 130) {
					if (start < 0) start = x;
					end = x;
				}
			}
			std::cout << start + (end - start) / 2 << std::endl;
			center = start + (end - start) / 2;
			
			
			double reward = game->makeAction({ 0,0,180,0 });

			//if (center < 269) double reward = game->makeAction({ 1,0,0 });
			//else if (center > 290) double reward = game->makeAction({ 0,1,0 });
			//else if (center >= 269 && center <= 290) double reward = game->makeAction({ 0,0,1 });

			cv::waitKey(sleepTime);
		}



	}
}

int main()
{
	game->setViZDoomPath(path + "\\vizdoom.exe");
	game->setDoomGamePath(path + "\\DOOM2.wad");
	game->setWindowVisible(true);
	game->setRenderWeapon(true);

	auto episodes = 10;
	RunTask2(episodes);
	

	game->close();
	delete game;
}

