#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define delimiter ','

#define NUMBER 0
#define X 1
#define Y 2
#define WIDTH 3
#define HEIGHT 4

const std::string inPath = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Advanced-Analysis-of-Algorithms-Assignment/Input/";
const std::string outPath = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Advanced-Analysis-of-Algorithms-Assignment/Output/Brute Force Algorithm Output/";
const std::string analysisPath = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Advanced-Analysis-of-Algorithms-Assignment/Analysis/Brute Force Analysis/";

const int max_iter = 5000;
const int step_size = 1;

class Rectangle
{
public:
    class Point
    {
    public:
        int x;
        int y;

        Point() {}

        Point(int x, int y)
            : x(x), y(y)
        {
        }
    };

    int number;
    int x1;
    int y1;
    int x2;
    int y2;
    int width;
    int height;
    Point bottom_left;
    Point bottom_right;
    Point top_left;
    Point top_right;

    Rectangle(int number, int x1, int y1, int x2, int y2)
        : number(number),
          x1(x1),
          y1(y1),
          x2(x2),
          y2(y2),
          width(x2 - x1),
          height(y2 - y1),
          bottom_left(x1, y1),
          bottom_right(x2, y1),
          top_left(x1, y2),
          top_right(x2, y2)
    {
    }

    std::string toString()
    {
        return "Rectangle " + std::to_string(number) + " with anchor point at (" + std::to_string(bottom_left.x) + ", " + std::to_string(bottom_left.y) + ") with width: " + std::to_string(width) + " and height: " + std::to_string(height);
    }
};

std::vector<int> split(std::string s)
{
    /*
    # Structure of input
    * Number,x,y,Width,Height
    */

    std::vector<int> numbers;

    size_t pos = 0;

    int number;
    pos = s.find(delimiter);
    number = stoi(s.substr(0, pos));
    numbers.push_back(number);
    s.erase(0, pos + 1);

    int x;
    pos = s.find(delimiter);
    x = stoi(s.substr(0, pos));
    numbers.push_back(x);
    s.erase(0, pos + 1);

    int y;
    pos = s.find(delimiter);
    y = stoi(s.substr(0, pos));
    numbers.push_back(y);
    s.erase(0, pos + 1);

    int width;
    pos = s.find(delimiter);
    width = stoi(s.substr(0, pos));
    numbers.push_back(width);
    s.erase(0, pos + 1);

    int height = stoi(s);
    numbers.push_back(height);

    return numbers;
}

std::vector<Rectangle> getRectangles(std::string filename)
{
    std::vector<Rectangle> rectangles;

    std::string headers;

    std::ifstream myFile(filename);
    std::string s;

    int count = 0;

    while (getline(myFile, s))
    {
        if (count == 0)
        {
            headers = s;
        }
        else
        {
            std::vector<int> numbers = split(s);

            int number = numbers[NUMBER];
            int x = numbers[X];
            int y = numbers[Y];
            int width = numbers[WIDTH];
            int height = numbers[HEIGHT];

            Rectangle rectangle = Rectangle(number, x, y, width, height);
            rectangles.push_back(rectangle);
        }
        count++;
    }

    myFile.close();

    return rectangles;
}

void printRectangles(std::vector<Rectangle> rectangles)
{
    for (Rectangle rectangle : rectangles)
        std::cout << rectangle.toString() << std::endl;
}

std::vector<std::string> getAdjacencies(std::vector<Rectangle> rectangles)
{
    std::vector<std::string> output;
    for (Rectangle current : rectangles)
    {
        std::string out = std::to_string(current.number) + ',';

        std::string detail = "";
        int count = 0;

        for (Rectangle other : rectangles)
        {
            if (current.number != other.number)
            {
                if (current.bottom_right.x == other.x1)
                {
                    bool adjacent = false;
                    int yt = 0, yb = 0;

                    if (current.bottom_right.y <= other.bottom_left.y && other.bottom_left.y <= current.top_right.y &&
                        current.bottom_right.y <= other.top_left.y && other.top_left.y <= current.top_right.y)
                    {
                        yt = other.top_left.y;
                        yb = other.y1;
                        adjacent = true;
                    }
                    else if (other.bottom_left.y <= current.bottom_right.y && current.bottom_right.y <= other.top_left.y &&
                             other.bottom_left.y <= current.top_right.y && current.top_right.y <= other.top_left.y)
                    {
                        yt = current.top_right.y;
                        yb = current.bottom_right.y;
                        adjacent = true;
                    }
                    else if (current.bottom_right.y <= other.bottom_left.y && other.bottom_left.y <= current.top_right.y)
                    {
                        yt = current.top_right.y;
                        yb = other.y1;
                        adjacent = true;
                    }
                    else if (current.bottom_right.y <= other.top_left.y && other.top_left.y <= current.top_right.y)
                    {
                        yt = other.top_left.y;
                        yb = current.bottom_right.y;
                        adjacent = true;
                    }

                    if (adjacent)
                    {
                        detail += ',' + std::to_string(other.number) + ',' + std::to_string(other.x1) + ',' + std::to_string(yb) + ',' + std::to_string(yt);
                        count++;
                    }
                }
            }
        }

        out += std::to_string(count) + detail;
        output.push_back(out);
    }

    return output;
}

int main()
{
    auto programStart = std::chrono::high_resolution_clock::now();

    std::string analysisFilename = "BruteForceAnalysis.csv";
    std::ofstream analysisFile(analysisPath + analysisFilename);

    analysisFile << "Input,Time" << '\n';

    for (int numRectangles = 1; numRectangles <= max_iter; numRectangles += step_size)
    {
        std::string inFilename = "in" + std::to_string(numRectangles) + ".csv";
        std::vector<Rectangle> rectangles = getRectangles(inPath + inFilename);

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::string> output = getAdjacencies(rectangles);

        auto stop = std::chrono::high_resolution_clock::now();

        std::string outFilename = "out" + std::to_string(numRectangles) + ".csv";
        std::ofstream outFile(outPath + outFilename);
        for (std::string out : output)
        {
            // cout << out << endl;
            outFile << out << '\n';
        }

        outFile.close();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        analysisFile << numRectangles << ',' << (float)(duration.count() / 1000) << '\n';
        std::cout << numRectangles << " rectangles: " << (float)(duration.count() / 1000) << "ms" << std::endl;
    }

    analysisFile.close();

    auto programStop = std::chrono::high_resolution_clock::now();

    auto programDuration = std::chrono::duration_cast<std::chrono::microseconds>(programStop - programStart);

    std::cout << '\n'
              << "--- Program took " << (float)(programDuration.count() / 1000) << " milliseconds ---" << std::endl;

    return 0;
}