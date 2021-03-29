#include "MAP.h"

void MAP::Create_Grid (int width, int height) {
    for (int x = 0; x < width; x++) {
        vector<Tile*> column;

        for (int y = 0; y < height; y++) {
            Tile * new_tile = new Tile;
            column.push_back ( new_tile );
        }

        tiles.push_back ( column );
    }
    return;
}

bool MAP::Add_Attribute (string attrib_name, string default_value) {
    for (string attrib : attribs) {
        if ( attrib.compare( attrib_name ) == 0 ) return false;
    }
    attribs.push_back ( attrib_name );
    for (vector<Tile*> column : tiles) {
        for (Tile* tile : column) {
            tile->data.push_back ( default_value );
        }
    }
    return true;
}

bool MAP::Set_All_Attribute (string attrib_name, string new_value) {
    int attrib_id = 0;
    for (string attrib : attribs) {
        if ( attrib.compare( attrib_name ) == 0 ) {
            for (vector<Tile*> column : tiles) {
                for (Tile* tile : column) {
                    tile->data.at ( attrib_id ) = new_value;
                }
            }
        }
        attrib_id++;
    }
    return false;
}

string MAP::Get_Tile_Attribute (int x, int y, string attrib_name) {
    int attrib_id = 0;
    for (string attrib : attribs) {
        if ( attrib.compare( attrib_name ) == 0 ) {
            string attrib_value = tiles.at( x ).at( y )->data.at( attrib_id );
            return attrib_value;
        }
        attrib_id++;
    }
    // returns nullptr for no attribute of that name
    return nullptr;
}