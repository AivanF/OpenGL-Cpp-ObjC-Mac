//
//  ItemModel.hpp
//  OpenGL-Cpp-Mac
//
//  Created by Barashev Ivan on 21/01/2018.
//

#ifndef ItemModel_hpp
#define ItemModel_hpp

#include "Item.hpp"
#include "Model.hpp"


/// Box with the same texture on each side
class ItemModel : public Item {
    Model *_m;
    
public:
    ItemModel(Model *m);
    void draw(ShaderProgram *shp) override;
};

#endif /* ItemModel_hpp */
