#include <vector>   // std::vector
#include <iostream> // std::cout
#include <fstream>  // std::ifstream
#include <chrono>   // std::chrono
// #include <algorithm> // std::sort
#include <set> // std::set

/*
Author: Tevlen Naidoo (2429493) - @AwesomeTevv
*/

// ----------------------------------------------------
// Defining constants for convenience and clarity
// ----------------------------------------------------

#define delimiter ',' // The delimiter of the input files. By default is ','.

#define NUMBER 0 // The position in the array of the number of the rectangle.
#define X1 1     // The position in the array of the x-coordinate of the bottom-left corner of the rectangle.
#define Y1 2     // The position in the array of the y-coordinate of the bottom-left corner of the rectangle.
#define X2 3     // The position in the array of the x-coordinate of the top-right corner of the rectangle.
#define Y2 4     // The position in the array of the y-coordinate of the top-right corner of the rectangle.

// ----------------------------------------------------
// Variable To Change
// ----------------------------------------------------

// Path to the directory where the input files are found.
// Change as needed.
const std::string inPath = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Assignment/Input/";

// Path to the directory where the output files are to be saved.
// Change as needed.
const std::string outPath = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Assignment/Output/Improved Method Output/Average2/";

// Name of the generated Analysis .csv file.
// Change as needed.
const std::string analysisFilename = "AverageCase2.csv";

// Path to the directory where the analysis file is to be saved.
// Change as needed.
const std::string analysisPath = "C:/Users/tevle/Desktop/University/Third Year/Advanced Analysis of Algorithms/Advanced-Analysis-of-Algorithms-Assignment/Analysis/Improved Analysis/";

const int max_iter = 18000; // The maximum amount of iterations the program will make. Change as needed.
const int step_size = 1;    // The step size between iterations. Change as needed.

// ----------------------------------------------------
// Everything from now on should remain unchanged
// ----------------------------------------------------

std::string types[4] = {"Bottom Left", "Top Right"};

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

        std::string toString()
        {
            return '(' + std::to_string(this->x) + ',' + std::to_string(this->y) + ')';
        }
    };

    /*
        Custom Details class
        Stores the details of the adjacency
    */
    class Details
    {
    public:
        int rectangleNumber; // The rectangle number of the adjacent number
        int x;               // The x-coordinate where the rectangles meet
        int yt;              // The upper y-coordinate of the adjacency region
        int yb;              // The lower y-coordinate of the adjacency region

        Details(int rectangleNumber, int x, int yt, int yb)
            : rectangleNumber(rectangleNumber),
              x(x),
              yt(yt),
              yb(yb)
        {
        }

        // bool operator==(const Details &d) const
        // {
        //     return this->rectangleNumber == d.rectangleNumber;
        // }

        bool operator<(const Details &d) const
        {
            return this->rectangleNumber < d.rectangleNumber;
        }

        std::string toString()
        {
            return ',' + std::to_string(this->rectangleNumber) + ',' + std::to_string(this->x) + ',' + std::to_string(this->yb) + ',' + std::to_string(this->yt);
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

    std::set<Details> adjacencies; // Vector that stores the details of the adjacency of the adjacent rectangles

    Rectangle() {}

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

    bool operator<(const Rectangle &r) const
    {
        // return this->number < r.number;
        if (this->x1 != r.x1)
            return this->x1 < r.x1;
        else
        {
            return this->y1 < r.y1;
        }
    }

    /*
        Returns a string with all the details of the rectangle.
    */
    std::string toString()
    {
        return "Rectangle " + std::to_string(number) + " with anchor point at (" + std::to_string(bottom_left.x) + ", " + std::to_string(bottom_left.y) + ") with width: " + std::to_string(width) + " and height: " + std::to_string(height);
    }
};

enum
{
    BottomLeft,
    TopRight
};

/*
    Custom Event class
    Stores the details of the Event
*/
class Event
{
public:
    Rectangle r;        // The rectangle the event is referencing
    Rectangle::Point p; // The point the event is referencing
    int type;           // The type of event
    Event(Rectangle &r, Rectangle::Point p, int type)
    {
        this->r = r;
        this->p = p;
        this->type = type;
    }
    bool operator<(const Event &e) const
    {
        if (this->p.x != e.p.x)
        {
            return this->p.x < e.p.x;
        }
        else
        {
            if (this->p.y != e.p.y)
            {
                return this->p.y < e.p.y;
            }
            else
            {
                return this->type < e.type;
            }
        }
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
    Number,x1,y1,x2,y2
    */

    std::vector<int> numbers;

    size_t pos = 0;

    int number;
    pos = s.find(delimiter);
    number = stoi(s.substr(0, pos));
    numbers.push_back(number);
    s.erase(0, pos + 1);

    int x1;
    pos = s.find(delimiter);
    x1 = stoi(s.substr(0, pos));
    numbers.push_back(x1);
    s.erase(0, pos + 1);

    int y1;
    pos = s.find(delimiter);
    y1 = stoi(s.substr(0, pos));
    numbers.push_back(y1);
    s.erase(0, pos + 1);

    int x2;
    pos = s.find(delimiter);
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

            int number = numbers[NUMBER];
            int x1 = numbers[X1];
            int y1 = numbers[Y1];
            int x2 = numbers[X2];
            int y2 = numbers[Y2];

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

/*
    Function that prints to the console every Event
    in a given vector of Events.
*/
void printEvents(std::vector<Event> events)
{
    std::cout << "Number of events: " << events.size() << std::endl;
    for (auto event : events)
    {
        std::cout << types[event.type] << " of Rectangle " << event.r.number << " at Point " << event.p.toString() << std::endl;
    }
    std::cout << std::endl;
}

/*
    Function that prints to the console every Candidate
    in a given set of Rectangles.
*/
void printCandidates(std::set<Rectangle> candidates)
{
    if (candidates.empty())
        std::cout << "Candidates list is empty" << std::endl;
    else
    {
        std::string out = "";
        for (auto it = candidates.begin(); it != candidates.end(); it++)
        {
            out += std::to_string(it->number) + '\t';
        }
        std::cout << out << std::endl;
    }
}

/*
    Function that performs a line sweeping on the list of rectangles
    and returns the adjacencies of the rectangles
*/
std::vector<std::string> ImprovedMethod(std::vector<Rectangle> &rectangles)
{
    // std::vector<Event> events; // Stores a list of all the Events
    std::set<Event> events;

    for (Rectangle rectangle : rectangles)
    {
        // Event e1 = Event(rectangle, rectangle.bottom_left, BottomLeft);
        // Event e2 = Event(rectangle, rectangle.bottom_right, BottomRight);
        // Event e3 = Event(rectangle, rectangle.top_right, TopRight);
        // Event e4 = Event(rectangle, rectangle.top_left, TopLeft);
        // events.push_back(e1);
        // events.push_back(e2);
        // events.push_back(e3);
        // events.push_back(e4);
        events.insert(Event(rectangle, rectangle.bottom_left, BottomLeft));
        events.insert(Event(rectangle, rectangle.top_right, TopRight));
    }

    /*
        Ordering the events
    */
    // std::sort(events.begin(), events.end(), [](Event a, Event b)
    //           {
    //     if (a.p.x != b.p.x){
    //         return a.p.x < b.p.x;
    //     }
    //     else{
    //         if (a.p.y != b.p.y){
    //             return a.p.y < b.p.y;
    //         }
    //         else{
    //             return a.type < b.type;
    //         }
    //     } });

    // std::cout << "Event Schedule:" << std::endl;
    // printEvents(events);
    // std::cout << std::endl;

    std::set<Rectangle> candidates;

    std::set<Event>::iterator e;
    for (e = events.begin(); e != events.end(); e++)
    {
        Event event = *e;
        if (event.type == BottomLeft)
            candidates.insert(event.r);
        else
        {
            candidates.erase(event.r);
            // auto itr = candidates.begin();
            std::set<Rectangle>::iterator candidate;
            for (candidate = candidates.begin(); candidate != candidates.end(); candidate++)
            {
                Rectangle current = event.r;
                Rectangle other = *candidate;

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
                else if (current.bottom_right.y < other.bottom_left.y && other.bottom_left.y < current.top_right.y)
                {
                    yt = current.top_right.y;
                    yb = other.y1;
                    adjacent = true;
                }
                else if (current.bottom_right.y < other.top_left.y && other.top_left.y < current.top_right.y)
                {
                    yt = other.top_left.y;
                    yb = current.bottom_right.y;
                    adjacent = true;
                }

                if (adjacent)
                {
                    // std::cout << "Rectangle " << other.number << " is adjacent to Rectangle " << current.number << std::endl;
                    Rectangle::Details details = Rectangle::Details(other.number, other.x1, yt, yb);
                    rectangles[event.r.number].adjacencies.insert(details);
                }
            }
        }
    }

    std::vector<std::string> output;

    for (Rectangle rectangle : rectangles)
    {
        std::string out = std::to_string(rectangle.number) + ',' + std::to_string(rectangle.adjacencies.size());

        std::set<Rectangle::Details>::iterator it;
        for (it = rectangle.adjacencies.begin(); it != rectangle.adjacencies.end(); it++)
        {
            Rectangle::Details detail = *it;
            out += detail.toString();
        }
        output.push_back(out);
    }

    return output;
}

/*
    Ordering structure for comparing rectangles
*/
bool cmp(Rectangle a, Rectangle b)
{
    if (a.x1 != b.x1)
    {
        return a.x1 < b.x1;
    }
    else
    {
        return a.y1 < b.y1;
    }
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

        std::vector<std::string> output = ImprovedMethod(rectangles);

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

    auto programDuration = std::chrono::duration_cast<std::chrono::minutes>(programStop - programStart); // Getting the duration of the whole program (in seconds)

    std::cout << '\n'
              << "--- Program took " << programDuration.count() << " minutes ---" << std::endl;

    return 0;
}