#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
  bool combineColors(ofPixels& pixels, float gridSpan, int colIndex, int rowIndex, ofColor color);
  ofColor getColor(ofPixels& pixels, int colIndex, int rowIndex);
  void mouseMoved(int x, int y );
  void setGridWidth(int w);
	
	ofVideoGrabber cam;
	ofImage thresh;
  
  int gridWidth;
  float squareOffsetX;
  float squareOffsetY;
  int numCols;
  int numRows;
  int numAllGrids;
  vector<int> map;
};
