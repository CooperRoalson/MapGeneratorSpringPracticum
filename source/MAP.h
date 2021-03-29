#include <string>
#include <vector>

using namespace std;

// Multidimensional Array Program

class MAP {
    private:
        struct Tile {
            vector<string> data;
        };

        vector<vector<Tile*> > tiles;
        vector<string> attribs;

        Tile Get_Tile (int x, int y);
    public:
        MAP () {
            // nothing *shrugs*
        }
        ~MAP () {
            // I literally have nothing to put here
        }
        void Create_Grid (int width, int height);
        string Get_Tile_Attribute (int x, int y, string attrib_name);
        bool Add_Attribute (string attrib_name, string default_value);
        bool Set_All_Attribute (string attrib_name, string new_value);
};