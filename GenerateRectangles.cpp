#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <chrono>

const int max_iter = 20000;
const int step_size = 1;

const std::string path = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Advanced-Analysis-of-Algorithms-Assignment/Input/";

/*
Function that takes in a minimum number
and a maximum number and produces a random
number from the minimum number to the maximum
number
*/
int randomIntInRange(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

/*
Rectangle class that stores the co-ordinates of the bottom-left
and top-right corners
*/
class Rectangle
{
public:
    int x1; // x-value for the bottom-left corner of the rectangle
    int y1; // y-value for the bottom-left corner of the rectangle
    int x2; // x-value for the top-right corner of the rectangle
    int y2; // y-value for the top-right corner of the rectangle
    Rectangle(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2)
    {
    }
    std::string toString()
    {
        return "Rectangle with bottom-left corner at (" + std::to_string(x1) + ", " + std::to_string(y1) + " and top-right corner at (" + std::to_string(x2) + ", " + std::to_string(y2) + ")";
    }
};

int main()
{
    auto programStart = std::chrono::high_resolution_clock::now();

    for (int numRectangles = 1; numRectangles <= max_iter; numRectangles += step_size)
    {
        std::cout << "Generating a list with " << numRectangles << " rectangles" << std::endl;

        // Defining our rectangle
        int x1 = 0;      // x-value for the bottom-left corner of the rectangle
        int y1 = 0;      // y-value for the bottom-left corner of the rectangle
        int x2 = 100000; // x-value for the top-right corner of the rectangle
        int y2 = 100000; // y-value for the top-right corner of the rectangle

        std::vector<Rectangle> rectangles;

        Rectangle outerRectangle = Rectangle(x1, y1, x2, y2);

        rectangles.push_back(outerRectangle);

        /*
        Breaking up our initial rectangle
        into four smaller rectangles
        */
        for (int k = 0; k < 5; k++)
        {
            Rectangle newRectangle = rectangles.back();
            rectangles.pop_back();

            x1 = newRectangle.x1;
            y1 = newRectangle.y1;
            x2 = newRectangle.x2;
            y2 = newRectangle.y2;

            if (((x2 - x1) > 1000) && ((y2 - y1) > 1000))
            {
                int midX = (int)((x2 - x1) / 2) + x1;
                int midY = (int)((y2 - y1) / 2) + y1;

                int rx = randomIntInRange(midX - 400, midX + 400);
                int ry = randomIntInRange(midY - 400, midY + 400);

                rectangles.push_back(Rectangle(x1, y1, rx, ry));
                rectangles.push_back(Rectangle(x1, ry, rx, y2));
                rectangles.push_back(Rectangle(rx, y1, x2, ry));
                rectangles.push_back(Rectangle(rx, ry, x2, y2));
            }
        }

        /*
        Choosing a random rectangle and breaking it up into
        smaller rectangles
        */
        while (rectangles.size() <= numRectangles)
        {
            int randomRectangle = randomIntInRange(0, rectangles.size() - 1);

            Rectangle r = rectangles[randomRectangle];
            rectangles.erase(rectangles.begin() + randomRectangle);

            x1 = r.x1;
            y1 = r.y1;
            x2 = r.x2;
            y2 = r.y2;

            if (((x2 - x1) > 1000) && ((y2 - y1) > 1000))
            {
                int midX = (x2 - x1) / 2 + x1;
                int midY = (y2 - y1) / 2 + y1;

                int rx = randomIntInRange(midX - 400, midX + 400);
                int ry = randomIntInRange(midY - 400, midY + 400);

                rectangles.push_back(Rectangle(x1, y1, rx, ry));
                rectangles.push_back(Rectangle(x1, ry, rx, y2));
                rectangles.push_back(Rectangle(rx, y1, x2, ry));
                rectangles.push_back(Rectangle(rx, ry, x2, y2));
            }
            else
            {
                rectangles.push_back(r);
            }
        }

        /*
        Removing excess rectangles
        */
        while (rectangles.size() != numRectangles)
        {
            int randomRectangle = randomIntInRange(0, rectangles.size() - 1);
            rectangles.erase(rectangles.begin() + randomRectangle);
        }

        /*
        Outputting the list to a .csv file
        */
        std::string filename = "in" + std::to_string(numRectangles) + ".csv";
        std::ofstream outputFile(path + filename);

        outputFile << "Number,x1,y1,x2,y2" << '\n';

        for (int j = 0; j < rectangles.size(); j++)
        {
            Rectangle r = rectangles[j];
            outputFile << j << ',' << r.x1 << ',' << r.y1 << ',' << r.x2 << ',' << r.y2 << '\n';
            // std::cout << r.toString() << std::endl;
        }

        outputFile.close();
    }

    auto programStop = std::chrono::high_resolution_clock::now();

    auto programDuration = std::chrono::duration_cast<std::chrono::microseconds>(programStop - programStart);

    std::cout << '\n'
              << "--- Program took " << (float)(programDuration.count() / 1000) << " milliseconds ---" << std::endl;

    return 0;
}