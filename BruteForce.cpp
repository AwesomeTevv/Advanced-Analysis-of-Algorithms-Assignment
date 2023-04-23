#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/*
Author: Tevlen Naidoo (2429493) - @AwesomeTevv
*/

// ----------------------------------------------------
// Defining constants for convenience and clarity
// ----------------------------------------------------

#define delimiter ',' // The delimiter of the input files. By default is ','.

#define NUMBER 0 // The position in the array of the number of the rectangle.
#define X 1      // The position in the array of the x-coordinate of the bottom-left corner of the rectangle.
#define Y 2      // The position in the array of the y-coordinate of the bottom-left corner of the rectangle.
#define WIDTH 3  // The position in the array of the width of the rectangle.
#define HEIGHT 4 // The position in the array of the height of the rectangle.

// ----------------------------------------------------
// Variable To Change
// ----------------------------------------------------

// Path to the directory where the input files are found.
// Change as needed.
const std::string inPath = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Assignment/Input/";

// Path to the directory where the output files are to be saved.
// Change as needed.
const std::string outPath = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Assignment/Output/Brute Force Algorithm Output/";

// Name of the generated Analysis .csv file.
// Change as needed.
const std::string analysisFilename = "BruteForceAnalysis.csv";

// Path to the directory where the analysis file is to be saved.
// Change as needed.
const std::string analysisPath = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Advanced-Analysis-of-Algorithms-Assignment/Analysis/Brute Force Analysis/";

const int max_iter = 18000; // The maximum amount of iterations the program will make. Change as needed.
const int step_size = 1;    // The step size between iterations. Change as needed.

// ----------------------------------------------------
// Everything from now on should remain unchanged
// ----------------------------------------------------

/*
    Custom Rectangle class.
    Stored all the necessary details of the rectangle.
*/
class Rectangle
{
public:
    /*
        Custom Point class.
        Stores the x and y coordinate of a single point.
    */
    class Point
    {
    public:
        int x; // The x coordinate of the point.
        int y; // The y coordinate of the point.

        Point() {}

        Point(int x, int y)
            : x(x), y(y)
        {
        }
    };

    int number;         // The number of the rectangle.
    int x1;             // The x-coordinate of the bottom-left corner of the rectangle.
    int y1;             // The y-coordinate of the bottom-left corner of the rectangle.
    int x2;             // The x-coordinate of the top-right corner of the rectangle.
    int y2;             // The y-coordinate of the top-right corner of the rectangle.
    int width;          // The width of the rectangle.
    int height;         // The height of the rectangle.
    Point bottom_left;  // The bottom-left point (coordinates) of the rectangle.
    Point bottom_right; // The bottom-right point (coordinates) of the rectangle.
    Point top_left;     // The top-left point (coordinates) of the rectangle.
    Point top_right;    // The top-right point (coordinates) of the rectangle.

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

    /*
        Returns a string with all the details of the rectangle.
    */
    std::string toString()
    {
        return "Rectangle " + std::to_string(number) + " with anchor point at (" + std::to_string(bottom_left.x) + ", " + std::to_string(bottom_left.y) + ") with width: " + std::to_string(width) + " and height: " + std::to_string(height);
    }
};

/*
    Function that splits a given string into a vector of integers, by a given delimiter.
    By default, the delimiter is ','
*/
std::vector<int> split(std::string s)
{
    /*
    Structure of input
    Number,x,y,Width,Height
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

/*
    Function that prints to the console every Rectangle
    in a given vector of Rectangles.
*/
void printRectangles(std::vector<Rectangle> rectangles)
{
    for (Rectangle rectangle : rectangles)
        std::cout << rectangle.toString() << std::endl;
}

/*
    A function that, when given a vector of Rectangles, will determine
    the adjacencies, to the right, of every Rectangle and will return
    an adjacency list.
*/
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
    auto programStart = std::chrono::high_resolution_clock::now(); // Starting the timer that tracks the runtime of the whole program

    std::ofstream analysisFile(analysisPath + analysisFilename); // The Analysis file

    analysisFile << "Input,Time" << '\n';

    for (int numRectangles = 1; numRectangles <= max_iter; numRectangles += step_size)
    {
        std::string inFilename = "in" + std::to_string(numRectangles) + ".csv"; // The name of the input file
        std::vector<Rectangle> rectangles = getRectangles(inPath + inFilename);

        auto start = std::chrono::high_resolution_clock::now(); // Starting the timer that tracks the runtime of the brute force program

        std::vector<std::string> output = getAdjacencies(rectangles);

        auto stop = std::chrono::high_resolution_clock::now(); // Stopping the timer that tracks the runtime of the brute force program

        std::string outFilename = "out" + std::to_string(numRectangles) + ".csv";
        std::ofstream outFile(outPath + outFilename); // The generated output file
        for (std::string out : output)
        {
            // cout << out << endl;
            outFile << out << '\n'; // Adding the output to the output file
        }

        outFile.close();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); // Getting the duration of the brute force program (in microseconds)

        analysisFile << numRectangles << ',' << duration.count() / 1000.0 << '\n'; // Adding the data to the analysis file
        std::cout << numRectangles << " rectangles: " << duration.count() / 1000.0 << "ms" << std::endl;
    }

    analysisFile.close();

    auto programStop = std::chrono::high_resolution_clock::now(); // Starting the timer that tracks the runtime of the whole program

    auto programDuration = std::chrono::duration_cast<std::chrono::seconds>(programStop - programStart); // Getting the duration of the whole program (in seconds)

    std::cout << '\n'
              << "--- Program took " << programDuration.count() << " seconds ---" << std::endl;

    return 0;
}