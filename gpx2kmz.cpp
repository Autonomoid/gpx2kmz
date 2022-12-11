/*
 * =====================================================================================
 *
 *       Filename:  gpx2kmz.cpp
 *
 *    Description:  Convert GPX files to KMZ files.
 *
 *        Version:  1.0
 *        Created:  2013-01-22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Autonomoid
 *   Organization:  
 *
 * =====================================================================================
 */

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

namespace GPX
{
    class Point
    {
        public:
            Point(double lat, double lon, double ele, std::string time, std::string name, std::string
            desc);
            ~Point();
            double getLat();
            double getLon();
            double getEle();
            std::string getTime();
            std::string getName();
            std::string getDesc();
            std::string printGPX();

        private:
            double lon;
            double lat;
            double ele;
            std::string time;
            std::string name;
            std::string desc;
    };

    Point::Point(double lat, double lon, double ele, std::string time, std::string name, std::string
            desc)
    {
        this->lat = lat;
        this->lon = lon;
        this->ele = ele;
        this->time = time;
        this->name = name;
        this->desc = desc;
    };
    
    Point::~Point(){}
    double Point::getLat(){return this->lat;}
    double Point::getLon(){return this->lon;}
    double Point::getEle(){return this->ele;}
    std::string Point::getTime(){return this->time;}
    std::string Point::getName(){return this->name;}
    std::string Point::getDesc(){return this->desc;}

    std::string Point::printGPX()
    {
        std::stringstream ss;

        ss << "<trkpt lat=\"" << this->lat << "\" lon=\"" << this->lon << "\">"
           << "<ele>" << this->ele << "</ele>"
           << "<time>" << this->time << "</time>"
           << "<name>" << this->name << "</name>"
           << "<desc>" << this->desc << "</desc>"
           << "</trkpt>";

        return ss.str();
    }

    ///////////////////////////////////////////////////////////////////////////

    class Segment
    {
        public:
            Segment();
            ~Segment();
            std::string printGPX();
            void addPoint(Point p);

        private:
            std::vector<Point> Points;
    };

    Segment::Segment(){};
    Segment::~Segment(){};

    void Segment::addPoint(Point p)
    {
        this->Points.push_back(p);
    }

    std::string Segment::printGPX()
    {
        std::stringstream ss;
        ss << "<trkseg>";

        for(std::vector<Point>::iterator it = Points.begin(); it != Points.end(); ++it)
        {
            ss << it->printGPX();
        }

        ss << "</trkseg>";

        return ss.str();
    }

    ///////////////////////////////////////////////////////////////////////////

    class Track
    {
        public:
            Track(std::string name);
            ~Track();
            void addSegment(Segment S);
            std::string printGPX();

        private:
            std::vector<Segment> Segments;
            std::string name;
    };

    Track::Track(std::string name){this->name = name;}
    Track::~Track(){}

    void Track::addSegment(Segment s)
    {
        this->Segments.push_back(s);
    }

    std::string Track::printGPX()
    {
        std::stringstream ss;
        ss << "<trk><name>" << this->name << "</name>";

        for(std::vector<Segment>::iterator it = Segments.begin(); it != Segments.end(); ++it)
        {
            ss << it->printGPX();
        }

        ss << "</trk>";

        return ss.str();
    }

    ///////////////////////////////////////////////////////////////////////////

    class Log
    {
        public:
            Log();
            ~Log();
            void addTrack(Track t);
            std::string printGPX();

        private:
            std::vector<Track> Tracks;
    };

    void Log::addTrack(Track t)
    {
      this->Tracks.push_back(t);
    }

    Log::Log(){}
    Log::~Log(){}

    std::string Log::printGPX()
    {
        std::stringstream ss;
        ss << "<gpx>";
         
        for(std::vector<Track>::iterator it = Tracks.begin(); it != Tracks.end(); ++it)
        {
            ss << it->printGPX();
        }

        ss << "</gpx>";

        return ss.str();
    }
}

    ///////////////////////////////////////////////////////////////////////////

void parser(char* filename)
{
    std::string file_contents;
    std::ifstream file(filename);
    if(file.is_open())
    {
        while(!file.eof())
        {
            std::string temp;
            file >> temp;
            file_contents += temp;
        }
        file.close();
        std::cout << file_contents << std::endl;
    }    
}

    ///////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
  GPX::Point myPoint1(-0.5, 53.2, 200.0, "midnight", "point 1", "no descr");

  GPX::Segment mySegment1;
  mySegment1.addPoint(myPoint1);

  GPX::Track myTrack1("Track 1");
  myTrack1.addSegment(mySegment1);

  GPX::Log myLog1;
  myLog1.addTrack(myTrack1);

  std::cout << myLog1.printGPX() << std::endl;

  parser(argv[1]);

  return 0;
}

