//
//  main.cpp
//  autodiver_cpp
//
//  Created by oisee on 02.02.15.
//  Copyright (c) 2015 oisee. All rights reserved.
//

#define CELLX 8
#define CELLY 8
#define DIR_EVAL "./eval/"
#define DIR_BEST "./eval/"
#define FILE_RESULT "result.csv"
#define FILE_BEST "best.csv"

#include <iostream>
#include <fstream>
#include <string>
#include <Magick++.h>
#include <dirent.h>

using namespace std;
using namespace Magick;

typedef pair<int,Color> ColorRating;
typedef tuple<int, string, string> ResultRow;

int eval_image( string image_path );
int eval_cell( Image img, int x, int y);
bool rating_reverse_compare( ColorRating first, ColorRating second );
int main(int argc, const char * argv[]) {
    // insert code here...
    
    int err = 0;
    
//  err = eval_image("/Users/oisee/dev/imt/eval/geometry.png_r0_s256_x2_y4.png");
//  err = eval_image("/Users/oisee/dev/imt/in.jpg");
    err = eval_image("/Users/oisee/dev/imt/peep.png");
    cout << "File Err: " << err << endl;
    
    
    DIR *dd;
    struct dirent *fd;
    dd = opendir(DIR_EVAL);
    if ( dd == NULL ) {
        perror("opendir: Path './eval' does not exist or could not be read.");
        return -1;
    }
    
    //------------------------------------------------------------------
    
    ofstream result_file;
    result_file.open(FILE_RESULT);
    
    ofstream best_file;
    best_file.open(FILE_BEST);
    
    vector<ResultRow> results;
    
    int counter = 0;
    while ((fd = readdir(dd))){
        counter ++;
        if (counter > 6) {
            continue;
        }
        if (fd->d_name[0] == '.') {
            continue;
        }
        string image_path = DIR_EVAL;
        image_path.append(fd->d_name);
        
        err = eval_image(image_path);
        
        cout        << image_path << ", " << err << endl;
        result_file << image_path << ", " << err << endl;
        
    }
    closedir(dd);
    
    result_file.close();
    
    best_file.close();
    
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
    for (int i = 0 ; i< CELLX * CELLY; i++) {
        cs[pp[i]]++;
    }
    
    if (cs.size() <= 2 ) {
        return 0;
    } else {
        
        vector<ColorRating> ss;
        
        for (auto &i : cs) {
            ss.push_back(make_pair(i.second, i.first));
        }
        
        sort(ss.begin(),ss.end(), rating_reverse_compare );
        
        int counter = 0;
        for (auto &p : ss) {
            counter ++;
            if (counter > 2) {
                err += p.first;
            }
        }
    
        return err;
    }
}

bool rating_reverse_compare( ColorRating first, ColorRating second ){
    return first.first > second.first;
}
