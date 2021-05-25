//
//  StlWriter.hpp
//  Map Generator
//

#ifndef STLWriter_hpp
#define STLWriter_hpp

#include <iostream>
#include <fstream>

class STLWriter {
    
    private:
        
        struct Point {
            double x, y, z;
            
            double getMagnitude() const;
            void scale(double scalar);
            Point normalize() const;
            
            Point operator-(const Point& other) const;
            
            static Point crossProduct(Point u, Point v);
            
            void writeToStream(std::ostream& stream) const;
            
            std::string toString() const;
        };
    
        struct Facet {
            const Point p1, p2, p3;
            const Point normal;
            
            static Point calculateNormal(const Point& point1, const Point& point2, const Point& point3);
            
            void writeToStream(std::ostream& stream) const;
            
            Facet(const Point point1, const Point point2, const Point point3) : p1(point1), p2(point2), p3(point3), normal(calculateNormal(point1, point2, point3)) {}
        };
    
        
        std::ostream& stream;
        Point* points;
        int* tris;
        int numTris;
    
        static void writeFloatLittleEndian(std::ostream& stream, float f);
        static void writeUInt32LittleEndian(std::ostream& stream, uint32_t num);
        static void writeBytes(std::ostream& stream, const char* bytes, int length);
    
        STLWriter(std::ostream& outputStream, Point* pts, int* triIndexes, int triCount);

    public:
        ~STLWriter();
    
        void write();
    
        STLWriter(std::ostream& outputStream, double* pointCoords, int pointCount, int* triIndexes, int triCount);
    
        static STLWriter fromGrid(std::ostream& outputSteam, double* values, int width, int height, bool hasBack=false);
    
};


#endif /* STLWriter_hpp */
