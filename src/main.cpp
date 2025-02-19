#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <utility>
#pragma region imgui
#include "imgui.h"
#include "imgui-SFML.h"
#include "imguiThemes.h"
#pragma endregion


short totalRows, totalCols;
short scale = 25;

//static void update(std::vector<std::vector<int>>& board);
//static int countNeighbors(int r, int c, std::vector<std::vector<int>>& board);

static void update(std::vector<int>& board, int size);
static int countNeighbors(int r, int c, std::vector<int>& board);

//steps:
//1.create blank screen and simulation loop
//2.create the grid
//3.count live neighbors
//4.implement simulation rules
//5.starting and stopping the simulation
//6.create custom initial state

//rules:
//underpopulation: a live cell with fewer than 2 live neighbors dies.
//stasis: a live cell with 2 or 3 live neighbors lives on to the next generation
//overpopulation: a live cell with more than 3 live neighbors dies
//reproduction: a dead cell with exactly 3 live neighbors becomes a live cell

int main()
{
	sf::RenderWindow window(sf::VideoMode(750, 750), "Conway's game of life");
	window.setFramerateLimit(60);
#pragma region imgui
	ImGui::SFML::Init(window);
	//you can use whatever imgui theme you like!
	//ImGui::StyleColorsDark();				
	//imguiThemes::yellow();
	//imguiThemes::gray();
	imguiThemes::green();
	//imguiThemes::red();
	//imguiThemes::gray();
	//imguiThemes::embraceTheDarkness();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.FontGlobalScale = 2.f;
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg].w = 0.5f;
	//style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
#pragma endregion

	totalRows = window.getSize().x / scale; //30
	totalCols = window.getSize().y / scale; //30
	int size = totalCols * totalRows; //900

	/*std::vector<int> cols(totalCols, 0);
	cols.reserve(totalCols);
	std::vector<std::vector<int>> board(totalRows, cols);
	board.reserve(totalRows);*/


	//enhanced version
	std::vector<int> board(size);


	/*board[5][29] = 1;
	board[6][0] = 1;
	board[5][0] = 1;
	board[4][0] = 1;

	std::cout << countNeighbors(5, 29, board) << '\n';*/

	//for (size_t r = 0; r < totalRows; r++)
	//{
	//	for (size_t c = 0; c < totalCols; c++)
	//	{
	//		short randomValue = rand() % 2; //generate random number from 0-4, (set cell to 1 if random == 4 else set cell to 0)
	//		board[r][c] = randomValue; //result: 20% of cells are 1 and 80% of cells are 0
	//	}
	//}

	//enhanced version
	for (size_t i = 0; i < size; i++)
	{
		short randomValue = rand() % 2; //generate random number from 0-4, (set cell to 1 if random == 4 else set cell to 0)
		board[i] = randomValue; //result: 20% of cells are 1 and 80% of cells are 0
	}

	/*for (size_t n = 1; n <= size; n++)
	{
		std::cout << board[n - 1] << ' ';
		if (n % totalRows == 0)
		{
			std::cout << '\n';
		}
	}*/
	
	int row = 0;

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{

#pragma region imgui
			ImGui::SFML::ProcessEvent(window, event);
#pragma endregion


			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
			{
				// Adjust the viewport when the window is resized
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
		}

		//calculate the delta time
		sf::Time deltaTime = clock.restart();
		float deltaTimeSeconds = deltaTime.asSeconds();

		//make sure delta time stays within normal bounds, like between one FPS and zero FPS
		deltaTimeSeconds = std::min(deltaTimeSeconds, 1.f);
		deltaTimeSeconds = std::max(deltaTimeSeconds, 0.f);

#pragma region imgui
		ImGui::SFML::Update(window, deltaTime);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
#pragma endregion


		/*ImGui::Begin("Hello, world!");
		ImGui::Button("Look at this pretty button!");
		ImGui::Text("Hello!");
		ImGui::End();*/

		//update(board);
		update(board, size);

		//game code....
		window.clear();

		/*for (size_t r = 0; r < totalRows; r++)
		{
			for (size_t c = 0; c < totalCols; c++)
			{
				sf::RectangleShape rect;
				if (board[r][c] == 0)
					rect.setFillColor(sf::Color::Black);
				if (board[r][c] == 1)
					rect.setFillColor(sf::Color::White);
				rect.setSize(sf::Vector2f(scale, scale));
				rect.setPosition(sf::Vector2f(r * scale, c * scale));
				window.draw(rect);
			}
		}*/


		//for (size_t n = 1; n <= size; n++)
		//{
		//	sf::RectangleShape rect;
		//	if (board[n - 1] == 0)
		//		rect.setFillColor(sf::Color::Black);
		//	if (board[n - 1] == 1)
		//		rect.setFillColor(sf::Color::White);
		//	rect.setSize(sf::Vector2f(scale, scale));
		//	
		//	if (n % totalRows == 0)
		//	{
		//		//std::cout << "rows: " << row << '\n';
		//		row++;
		//	}
		//	int col = n - 1;
		//	//std::cout << col << '\n';
		//	rect.setPosition(sf::Vector2f(row * scale, col * scale));
		//	window.draw(rect);
		//}
		//row = 0;



		for (size_t r = 0; r < totalRows; r++)
		{
			for (size_t c = 0; c < totalCols; c++)
			{
				sf::RectangleShape rect;
				if (board[r * totalCols + c] == 0)
					rect.setFillColor(sf::Color::Black);
				if (board[r * totalCols + c] == 1)
					rect.setFillColor(sf::Color::White);
				rect.setSize(sf::Vector2f(scale, scale));
				rect.setPosition(sf::Vector2f(r * scale, c * scale));
				window.draw(rect);
			}
		}
#pragma region imgui
		ImGui::SFML::Render(window);
#pragma endregion

		window.display();

		sf::sleep(sf::milliseconds(100));
	}

#pragma region imgui
	ImGui::SFML::Shutdown();
#pragma endregion

	return 0;
}


//static void update(std::vector<std::vector<int>>& board)
//{
//	std::vector<std::vector<int>> tempBoard(totalRows, std::vector<int>(totalCols, 0));
//	for (size_t r = 0; r < totalRows; r++)
//	{
//		for (size_t c = 0; c < totalCols; c++)
//		{
//			int liveNeighbors = countNeighbors(r, c, board);
//			if (board[r][c] == 1)
//			{
//				if (liveNeighbors > 3 || liveNeighbors < 2)
//					tempBoard[r][c] = 0;	//overpopulation: a live cell with more than 3 live neighbors dies
//				
//				else
//					tempBoard[r][c] = 1;	//stasis: a live cell with 2 or 3 live neighbors lives on to the next generation
//			}
//			else
//			{
//				
//				if (liveNeighbors == 3)
//					tempBoard[r][c] = 1;	//reproduction: a dead cell with exactly 3 live neighbors becomes a live cell
//				else
//					tempBoard[r][c] = 0;	//underpopulation: a live cell with fewer than 2 live neighbors dies.
//			}
//		}
//	}
//	board = tempBoard;
//}
//
//static int countNeighbors(int r, int c, std::vector<std::vector<int>>& board)
//{
//	int liveNeighbors = 0;
//
//	std::vector<std::pair<int,int>> neighborOffset = {
//		{-1,-1},	//up-left
//		{-1, 0},	//above
//		{-1, 1},	//up-right
//		{0,  1},	//right
//		{1,  1},	//low-right
//		{1,  0},	//below
//		{1, -1},	//low-left
//		{0, -1},	//left
//	};
//
//	for (const auto& offset : neighborOffset)
//	{
//		//if (offset.first == 0 && offset.second == 0) continue;
//			//traverse all 8 offset
//			/*int neighborR = r + offset.first;
//			int neighborC = c + offset.second;*/
//
//			//implement: toroidal grid
//			//problem: the cell check are out of bounds
//			//solution: go back to the starting cell to be continuous
//
//		int new_row = ((r + offset.first) + totalRows) % totalRows;
//		int new_col = ((c + offset.second) + totalCols) % totalCols;
//
//
//		if(board[new_row][new_col] == 1) //check if the cell is alive(1)
//			liveNeighbors += 1;
//	}
//	return liveNeighbors;
//}



static void update(std::vector<int>& board, int size)
{
	std::vector<int> tempBoard(size);
	for (size_t r = 0; r < totalRows; r++)
	{
		for (size_t c = 0; c < totalCols; c++)
		{
			int liveNeighbors = countNeighbors(r, c, board);
			if (board[r * totalCols + c] == 1)
			{
				if (liveNeighbors > 3 || liveNeighbors < 2)
					tempBoard[r * totalCols + c] = 0;	//overpopulation: a live cell with more than 3 live neighbors dies
				
				else
					tempBoard[r * totalCols + c] = 1;	//stasis: a live cell with 2 or 3 live neighbors lives on to the next generation
			}
			else
			{
				
				if (liveNeighbors == 3)
					tempBoard[r * totalCols + c] = 1;	//reproduction: a dead cell with exactly 3 live neighbors becomes a live cell
				else
					tempBoard[r * totalCols + c] = 0;	//underpopulation: a live cell with fewer than 2 live neighbors dies.
			}
		}
	}
	board = tempBoard;
}

static int countNeighbors(int r, int c, std::vector<int>& board)
{
	int liveNeighbors = 0;

	std::vector<std::pair<int,int>> neighborOffset = {
		{-1,-1},	//up-left
		{-1, 0},	//above
		{-1, 1},	//up-right
		{0,  1},	//right
		{1,  1},	//low-right
		{1,  0},	//below
		{1, -1},	//low-left
		{0, -1},	//left
	};

	for (const auto& offset : neighborOffset)
	{
		//if (offset.first == 0 && offset.second == 0) continue;
			//traverse all 8 offset
			/*int neighborR = r + offset.first;
			int neighborC = c + offset.second;*/

			//implement: toroidal grid
			//problem: the cell check are out of bounds
			//solution: go back to the starting cell to be continuous

		int new_row = ((r + offset.first) + totalRows) % totalRows;
		int new_col = ((c + offset.second) + totalCols) % totalCols;

		if(board[new_row * totalCols + new_col] == 1) //check if the cell is alive(1)
			liveNeighbors += 1;
	}
	return liveNeighbors;
}
