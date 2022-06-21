#include "MarchingSquares.h"

int main()
{
	/*
	for (int i = 0; i < field.rows; i++)
		for (int j = 0; j < field.columns; j++) 
			field.set_value(i , j, (float)(rand()) / RAND_MAX);*/
			
	MarchingSquares ms;

	while (ms.running())
	{
		ms.update();
		ms.render();

		/*for (int i = 0; i < field.rows - 1; i++)
			for (int j = 0; j < field.columns - 1; j++)
			{
				float tl = field[i][j];			// top-left vertex
				float tr = field[i + 1][j];		// top-right vertex
				float bl = field[i][j + 1];		// bottom-left vertex
				float br = field[i + 1][j + 1];	// bottom-right vertex
				if ((tl >= 0.5 && tr < 0.5) || (tl < 0.5 && tr >= 0.5))
				{
					
				}
				if ((tl >= 0.5 && bl < 0.5) || (tl < 0.5 && bl >= 0.5))
				{

				}
				if ((br >= 0.5 && tr < 0.5) || (br < 0.5 && tr >= 0.5))
				{

				}
				if ((br >= 0.5 && bl < 0.5) || (br < 0.5 && bl >= 0.5))
				{

				}
			}

		/*sf::CircleShape circ(2.0f);
		circ.setFillColor(sf::Color::White);
		for (int i = 0; i < field.rows; i++)
			for (int j = 0; j < field.columns; j++)
			{
				sf::Color col((int)(field[i][j] * 255), (int)(field[i][j] * 255), (int)(field[i][j] * 255));
				/*float tl = field[i][j];			// top-left vertex
				float tr = field[i + 1][j];		// top-right vertex
				float bl = field[i][j + 1];		// bottom-left vertex
				float br = field[i + 1][j + 1];	// bottom-right vertex
				sf::Vertex line[2];
				line[0].position = sf::Vector2f(i * cellsize + 10, j * cellsize + 10);
				line[0].color = col;
				line[1].position = sf::Vector2f(i * cellsize, j * cellsize);
				line[1].color = col;
				window.draw(line,2, sf::Lines);
				circ.setFillColor(col);
				circ.setPosition(i * cellsize + cellsize / 2, j * cellsize + cellsize / 2);
				if (field[i][j] > 0.5)
					window.draw(circ);
				
			}*/
	}
	return 0;
}