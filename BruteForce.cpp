#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
using namespace std::chrono;

#define filename "sample_input.csv"
#define delimiter ','

#define NUMBER 0
#define X 1
#define Y 2
#define WIDTH 3
#define HEIGHT 4

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

    string toString()
    {
        return "Rectangle " + to_string(number) + " with anchor point at (" + to_string(bottom_left.x) + ", " + to_string(bottom_left.y) + ") with width: " + to_string(width) + " and height: " + to_string(height);
    }
};

vector<int> split(string s)
{
    /*
    # Structure of input
    * Number,x,y,Width,Height
    */

    vector<int> numbers;

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

vector<Rectangle> rectangles;

void getRectangles()
{
    string headers;

    ifstream myFile(filename);
    string s;

    int count = 0;

    while (getline(myFile, s))
    {
        if (count == 0)
        {
            headers = s;
        }
        else
        {
            vector<int> numbers = split(s);

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
}

void printRectangles()
{
    for (Rectangle rectangle : rectangles)
        cout << rectangle.toString() << endl;
}

vector<string> getAdjacencies()
{
    vector<string> output;
    for (Rectangle current : rectangles)
    {
        string out = to_string(current.number) + ",";

        string detail = "";
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
                        detail += "," + to_string(other.number) + "," + to_string(other.x1) + "," + to_string(yb) + "," + to_string(yt);
                        count++;
                    }
                }
            }
        }

        out += to_string(count) + detail;
        output.push_back(out);
    }

    return output;
}

int main()
{
    auto start = high_resolution_clock::now(); // * Starting the timer

    getRectangles();
    vector<string> output = getAdjacencies();

    auto stop = high_resolution_clock::now(); // * Ending the timer

    /*
    ! Outputting to file
    */

    ofstream fout("output.csv");

    for (string out : output)
    {
        cout << out << endl;
        fout << out << '\n';
    }

    fout.close();

    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "\n--- " << duration.count() << " milliseconds ---" << endl;

    return 0;
}