//
//  PerlinNoise.hpp
//  Map Generator
//

#ifndef PerlinNoise_hpp
#define PerlinNoise_hpp

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
        ~PerlinNoiseGenerator();
        
        double noise(double x, double y);
        
    private:
        
        Vec2D<int> dimensions;
        
        unsigned int seed;
        
        Vec2D<double> *gradients;
        void generateGradients();
        
        double gridDotProduct(Vec2D<double> point, Vec2D<int> grid);
        
        double interpolate(double a0, double a1, double w);
    
        unsigned int randomInitialSeed();
    
};


#endif /* PerlinNoise_hpp */
