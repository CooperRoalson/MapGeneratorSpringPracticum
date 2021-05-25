//
//  PerlinNoise.hpp
//  Map Generator
//

#ifndef PerlinNoise_hpp
#define PerlinNoise_hpp

#include <random>

class PerlinNoiseGenerator {
    
    template <class numClass> struct Vec2D {
        numClass x;
        numClass y;
        
        Vec2D() {
            x = 0.;
            y = 0.;
        }
        
        Vec2D(numClass _x, numClass _y) {
            x = _x;
            y = _y;
        }
    };
    
    public:
        
        PerlinNoiseGenerator(int width, int height);
        PerlinNoiseGenerator(unsigned int seed, int width, int height);
        PerlinNoiseGenerator(std::mt19937* generator, int width, int height);
        ~PerlinNoiseGenerator();
        
        double noise(double x, double y);
        
    private:
        PerlinNoiseGenerator(std::mt19937* generator, int width, int height, bool deleteRandWhenDone); // For constructor from seed
        
        Vec2D<int> dimensions;
                
        Vec2D<double> *gradients;
        void generateGradients(std::mt19937* gen);
        
        double gridDotProduct(Vec2D<double> point, Vec2D<int> grid);
        
        double interpolate(double a0, double a1, double w);
    
        unsigned int randomInitialSeed();
    
};


#endif /* PerlinNoise_hpp */
