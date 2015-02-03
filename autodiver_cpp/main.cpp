//
//  main.cpp
//  autodiver_cpp
//
//  Created by oisee on 02.02.15.
//  Copyright (c) 2015 oisee. All rights reserved.
//

#define CELLX 8
#define CELLY 8

#include <iostream>
#include <string>
#include <Magick++.h>

using namespace std;
using namespace Magick;

int eval_image( string image_path );
int eval_cell( Image img, int x, int y);
int main(int argc, const char * argv[]) {
    // insert code here...

    int err = 0;
    
    err = eval_image("/Users/oisee/dev/imt/eval/geometry.png_r0_s256_x2_y4.png");
    
    return 0;
}

int eval_image( string image_path ){
    int err = 0;
    Image img;
    img.read(image_path);
    cout << "File: "<< image_path << " X:" << img.columns() << " Y:" << img.rows() << endl;
    int tox,toy = 0;
    
    toy = img.rows() / CELLY ;
    tox = img.columns() / CELLX  ;
    
    for (int y=0; y<toy; y++) {
        for (int x=0; x<tox; x++) {
            err += eval_cell(img, x* CELLX, y* CELLY);
        }
    }
    
    return err;
}

int eval_cell( Image img, int x, int y){
    int err = 0;
    
    PixelPacket *pp = img.getPixels(x, y, CELLX, CELLY );
    
    typedef map<Color, int> ColorStatistic;
    ColorStatistic cs;
    for (int i; i< CELLX * CELLY; i++) {
        Color pixel = pp[i];
        cs[pixel]++;
    }
    
    cout << "Cell size:" << cs.size() << endl;
    
    if (cs.size() < 0 ) {
        return 0;
    } else {
        //-----------------------
        for (auto &i : cs) {
            cout << "Cell: " << i.second << "" << endl;
        }
        
        return err;
    }
}