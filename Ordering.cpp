#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

class Rectangle
{
public:
    int number;
    int x1;
    int y1;
    int x2;
    int y2;

    Rectangle(int number, int x1, int y1, int x2, int y2)
        : number(number),
          x1(x1),
          y1(y1),
          x2(x2),
          y2(y2)
    {
    }

    std::string toString()
    {
        return std::to_string(this->number) + ',' + std::to_string(this->x1) + ',' + std::to_string(this->y1) + ',' + std::to_string(this->x2) + ',' + std::to_string(this->y2);
    }
};

bool Compare1(Rectangle a, Rectangle b)
{
    if (a.x1 != b.x1)
        return a.x1 < b.x1;
    else
        return a.y1 < b.y1;
}

bool Compare2(Rectangle a, Rectangle b)
{
    if (a.x1 != b.x1)
        return a.x1 > b.x1;
    else
        return a.y1 > b.y1;
}

std::vector<int> split(std::string s)
{
    /*
    Structure of input
    Number,x1,y1,x2,y2
    */

    std::vector<int> numbers;

    size_t pos = 0;

    int number;
    pos = s.find(',');
    number = stoi(s.substr(0, pos));
    numbers.push_back(number);
    s.erase(0, pos + 1);

    int x1;
    pos = s.find(',');
    x1 = stoi(s.substr(0, pos));
    numbers.push_back(x1);
    s.erase(0, pos + 1);

    int y1;
    pos = s.find(',');
    y1 = stoi(s.substr(0, pos));
    numbers.push_back(y1);
    s.erase(0, pos + 1);

    int x2;
    pos = s.find(',');
    x2 = stoi(s.substr(0, pos));
    numbers.push_back(x2);
    s.erase(0, pos + 1);

    int y2 = stoi(s);
    numbers.push_back(y2);

    return numbers;
}

/*
    Function that reads the input rectangles from a given input file
    and stores the rectangles in a vector of rectangles.
*/
std::vector<Rectangle> getRectangles(std::string filename)
{
    std::vector<Rectangle> rectangles;

    std::string headers;

    std::ifstream myFile(filename); // The input data .csv file
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

            int number = numbers[0];
            int x1 = numbers[1];
            int y1 = numbers[2];
            int x2 = numbers[3];
            int y2 = numbers[4];

            Rectangle rectangle = Rectangle(number, x1, y1, x2, y2);
            rectangles.push_back(rectangle);
        }
        count++;
    }

    myFile.close();

    return rectangles;
}

/*
    Function that prints to the console every Rectangle
    in a given vector of Rectangles.
*/
void printRectangles(std::vector<Rectangle> rectangles)
{
    for (Rectangle rectangle : rectangles)
        std::cout << rectangle.toString() << std::endl;
}

const std::string inPath = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Assignment/Input/";

const std::string outPathBest = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Assignment/Input/Best/";
const std::string outPathWorst = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Assignment/Input/Worst/";

int main()
{
    auto programStart = std::chrono::high_resolution_clock::now();

    for (int numRectangles = 1; numRectangles <= 18000; numRectangles++)
    {
        std::string filename = "in" + std::to_string(numRectangles) + ".csv";
        std::vector<Rectangle> rectangles = getRectangles(inPath + filename);

        std::vector<Rectangle> forward = rectangles;
        std::vector<Rectangle> reverse = rectangles;

        auto start = std::chrono::high_resolution_clock::now();

        std::sort(forward.begin(), forward.end(), Compare1);
        std::sort(reverse.begin(), reverse.end(), Compare2);

        auto stop = std::chrono::high_resolution_clock::now();

        std::string outFilename = "in" + std::to_string(numRectangles) + ".csv";

        std::ofstream bestFile(outPathBest + outFilename);
        std::ofstream worstFile(outPathWorst + outFilename);

        std::string headers = "Numbers,x1,y1,x2,y2";

        bestFile << headers << '\n';
        worstFile << headers << '\n';

        for (int i = 0; i < rectangles.size(); i++)
        {
            bestFile << forward[i].toString() << '\n';
            worstFile << reverse[i].toString() << '\n';
        }

        bestFile.close();
        worstFile.close();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << numRectangles << " rectangles: " << duration.count() / 1000.0 << "ms" << std::endl;
    }

    auto programStop = std::chrono::high_resolution_clock::now(); // Starting the timer that tracks the runtime of the whole program

    auto programDuration = std::chrono::duration_cast<std::chrono::seconds>(programStop - programStart); // Getting the duration of the whole program (in seconds)

    std::cout << '\n'
              << "--- Program took " << programDuration.count() << " seconds ---" << std::endl;

    return 0;
}
