//
//  STLWriter.cpp
//  Map Generator
//

#include "STLWriter.hpp"

#include <math.h>
#include <chrono>

STLWriter::Point STLWriter::Point::crossProduct(Point u, Point v){
    return Point{u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x};
}

double STLWriter::Point::getMagnitude() const {
    return sqrt(x*x + y*y + z*z);
}

void STLWriter::Point::scale(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
}

STLWriter::Point STLWriter::Point::normalize() const {
    Point result{x,y,z};
    result.scale(1/getMagnitude());
    return result;
}

STLWriter::Point STLWriter::Point::operator-(const Point& other) const {
    return Point{x-other.x, y-other.y, z-other.z};
}

void STLWriter::Point::writeToStream(std::ostream& stream) const {
    float xF = x, yF = y, zF = z;
    STLWriter::writeFloatLittleEndian(stream, (float) x);
    STLWriter::writeFloatLittleEndian(stream, (float) y);
    STLWriter::writeFloatLittleEndian(stream, (float) z);
}

std::string STLWriter::Point::toString() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}




STLWriter::Point STLWriter::Facet::calculateNormal(const Point& point1, const Point& point2, const Point& point3) {
    return Point::crossProduct(point2 - point1, point3 - point1).normalize();
}

void STLWriter::Facet::writeToStream(std::ostream& stream) const {
    normal.writeToStream(stream);
    
    p1.writeToStream(stream);
    p2.writeToStream(stream);
    p3.writeToStream(stream);
    
    char attributeByteCount[] = {0,0}; // Unused by STL
    STLWriter::writeBytes(stream, attributeByteCount, 2);
}


                                       
void STLWriter::writeFloatLittleEndian(std::ostream& stream, float f) {
    STLWriter::writeUInt32LittleEndian(stream, *(reinterpret_cast<uint32_t*>(&f)));
}

void STLWriter::writeUInt32LittleEndian(std::ostream& stream, uint32_t num) {
    // Little Endian
    stream.put(num        & 0xFF);
    stream.put(num >> 8   & 0xFF);
    stream.put(num >> 16  & 0xFF);
    stream.put(num >> 24  & 0xFF);
    //std::cout << "[" << std::to_string(num & 0xFF) << ", " << std::to_string(num >> 8 & 0xFF) << ", " << std::to_string(num >> 16 & 0xFF) << ", " << std::to_string(num >> 24 & 0xFF) << "]\n";
}

void STLWriter::writeBytes(std::ostream& stream, const char* bytes, int length) {
    stream.write(bytes, length);
}

STLWriter::STLWriter(std::ostream& outputStream, Point* pts, int* triIndexes, int triCount) : stream(outputStream) {
    
    points = pts;
    numTris = triCount;
    tris = triIndexes;
}

STLWriter::STLWriter(std::ostream& outputStream, double* pointCoords, int pointCount, int* triIndexes, int triCount) : stream(outputStream) {
    
    points = new Point[pointCount];
    for (int i = 0; i < pointCount; i++) {
        points[i] = Point{pointCoords[3*i], pointCoords[3*i + 1], pointCoords[3*i + 2]};
    }
    
    numTris = triCount;
    tris = new int[triCount*3];
    for (int i = 0; i < 3*triCount; i++) {
        tris[i] = triIndexes[i];
    }
}

STLWriter::~STLWriter() {
    delete[] points;
    delete[] tris;
}

void STLWriter::write() {
    std::cout << "Writing STL\n";
    
    // Empty header
    char bytes[80] = {0};
    writeBytes(stream, bytes, 80);
    
    writeUInt32LittleEndian(stream, numTris);
    
    auto lastPrintTime = std::chrono::system_clock::now(), currentTime = lastPrintTime;
    std::chrono::duration<float> span;
    
    for (int i = 0; i < numTris; i++) {
        Facet facet = Facet(points[tris[3*i]], points[tris[3*i + 1]], points[tris[3*i + 2]]);
        //std::cout << "facet: [" << facet.p1.toString() << "; " << facet.p2.toString() << "; " << facet.p3.toString() << "] normal: " << facet.normal.toString() << "\n";
        facet.writeToStream(stream);
        
        currentTime = std::chrono::system_clock::now();
        span = currentTime - lastPrintTime;
        if (span.count() >= 3) {
            std::cout << "> Writing facets: " << std::to_string((100*i)/numTris) << "%\n";
            lastPrintTime = currentTime;
        }
    }
    std::cout << "Finished writing STL\n";
}

STLWriter STLWriter::fromGrid(std::ostream& stream, double* values, int width, int height, bool hasBack) {
        
    int numBackPoints = width*2 + height*2 - 4;
    Point* backPoints = new Point[numBackPoints];
    Point* points = new Point[width * height + (hasBack ? numBackPoints : 0)];
    
    int backTriCount = ((width-1)*2 + (height-1)*2) * 2;
    int* backTris = new int[backTriCount*3];
    
    int triCount = (width-1) * (height-1) * 2;
    int* tris = new int[triCount*3 + (hasBack ? backTriCount : 0)];
    
    int i,trii,j = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            i = y * width + x;
            points[i] = Point{(double)x, (double)y, values[y*width+x]};
            
            if (hasBack && (y == 0 || x == 0 || y == height-1 || x == width-1)) {
                backPoints[j++] = Point{(double)x, (double)y, 0};
            }
            
            if (y < height-1 && x < width-1) {
                trii = y * (width-1) + x;
                
                tris[(trii*2)*3] = i;
                tris[(trii*2)*3 + 1] = i + 1;
                tris[(trii*2)*3 + 2] = i + width + 1;
                
                tris[(trii*2+1)*3] = i;
                tris[(trii*2+1)*3 + 1] = i + width + 1;
                tris[(trii*2+1)*3 + 2] = i + width;
            }
        }
    }
    
    if (hasBack) {
        int backIndex = width * height;
        
        i = 0;
        for (int x = 0; x < width-1; x++) {
            tris[3*i] = x;
            tris[3*i+1] = x+backIndex;
            tris[3*i+2] = x+backIndex+1;
            i++;
            
            tris[3*i] = x;
            tris[3*i+1] = x+backIndex+1;
            tris[3*i+2] = x+1;
            i++;
            
            
            
            int oppX = x+(height-1)*width;
            int oppBackIndex = backIndex+width+2*height-4;
            
            tris[3*i] = oppX;
            tris[3*i+1] = x+oppBackIndex+1;
            tris[3*i+2] = x+oppBackIndex;
            i++;
            
            tris[3*i] = oppX;
            tris[3*i+1] = oppX+1;
            tris[3*i+2] = x+oppBackIndex+1;
            i++;
        }
        
        for (int y = 1; y < height-2; y++) {
            int yt = y*width;
            int yb = backIndex+width+2*(y-1);
            
            tris[3*i] = yt;
            tris[3*i+1] = yb+2;
            tris[3*i+2] = yb;
            i++;
            
            tris[3*i] = yt;
            tris[3*i+1] = yt+width;
            tris[3*i+2] = yb+2;
            i++;
            
            
            
            int oppYT = yt+(width-1);
            int oppYB = yb+1;
            
            tris[3*i] = oppYT;
            tris[3*i+1] = oppYB;
            tris[3*i+2] = oppYB+2;
            i++;
            
            tris[3*i] = oppYT;
            tris[3*i+1] = oppYB+2;
            tris[3*i+2] = oppYT+width;
            i++;
        }
        
        tris[3*i] = 0;
        tris[3*i+1] = backIndex+width;
        tris[3*i+2] = backIndex;
        i++;
        
        tris[3*i] = 0;
        tris[3*i+1] = width;
        tris[3*i+2] = backIndex+width;
        i++;
        
        tris[3*i] = width-1;
        tris[3*i+1] = backIndex+(width-1);
        tris[3*i+2] = backIndex+width+1;
        i++;
        
        tris[3*i] = width-1;
        tris[3*i+1] = backIndex+(width-1)+2;
        tris[3*i+2] = (width-1)+width;
        i++;

        

        int yt = width*(height-2);
        int yb = backIndex+width+2*(height-2-1);
        
        tris[3*i] = yt;
        tris[3*i+1] = yb+2;
        tris[3*i+2] = yb;
        i++;
        
        tris[3*i] = yt;
        tris[3*i+1] = yt+width;
        tris[3*i+2] = yb+2;
        i++;
        
        tris[3*i] = yt+(width-1);
        tris[3*i+1] = yb+1;
        tris[3*i+2] = yb+1+width;
        i++;
        
        tris[3*i] = yt+(width-1);
        tris[3*i+1] = yb+1+width;
        tris[3*i+2] = yt+(width-1)+width;
        i++;

        tris[3*i] = backIndex;
        tris[3*i+1] = backIndex+width+2*(height-2);
        tris[3*i+2] = backIndex+width+2*(height-2)+(width-1);
        i++;
        
        tris[3*i] = backIndex;
        tris[3*i+1] = backIndex+width+2*(height-2)+(width-1);
        tris[3*i+2] = backIndex+(width-1);
        i++;
    
        memcpy(points + width*height, backPoints, sizeof(Point)*numBackPoints);
        memcpy(tris + triCount*3, backTris, sizeof(int)*backTriCount*3);
    }
    
    delete[] backPoints;
    delete[] backTris;
    
    return STLWriter(stream, points, tris, triCount);
}

/*
 STLWriter STLWriter::fromGrid(std::ostream& stream, double* values, int width, int height, bool hasBase) {
         
     
     Point* points = new Point[width * height];
     
     int triCount = (width-1) * (height-1) * 2;
     int* tris = new int[triCount*3];
     
     int i,trii,j = 0;
     for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x++) {
             i = y * width + x;
             points[i] = Point{(double)x, (double)y, values[y*width+x]};
             
             if (y < height-1 && x < width-1) {
                 trii = y * (width-1) + x;
                 
                 tris[(trii*2)*3] = i;
                 tris[(trii*2)*3 + 1] = i + 1;
                 tris[(trii*2)*3 + 2] = i + width + 1;
                 
                 tris[(trii*2+1)*3] = i;
                 tris[(trii*2+1)*3 + 1] = i + width + 1;
                 tris[(trii*2+1)*3 + 2] = i + width;
             }
         }
     }
     
     return STLWriter(stream, points, tris, triCount);
 }
 */
