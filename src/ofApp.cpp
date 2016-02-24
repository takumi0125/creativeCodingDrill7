#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
  ofBackground(255, 255, 255, 255);
  
	cam.setup(ofGetWidth(), ofGetHeight());
	thresh.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);
  
  setGridWidth(4);
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		convertColor(cam, thresh, CV_RGB2GRAY);
		
    if(ofGetMousePressed()) {
			autothreshold(thresh);
		} else {
			float thresholdValue = ofMap(mouseX, 0, ofGetWidth(), 0, 255);
			threshold(thresh, thresholdValue);
		}
		thresh.update();
	}
}

void ofApp::draw() {
  int index = 0;
  int gridSpan = 1;
  int colIndex, rowIndex;
  ofColor color;
  
  ofPixels pixels = thresh.getPixels();
  map = vector<int>(numAllGrids);

  while(index < numAllGrids) {
    if(map[index] == 1) {
      index++;
      continue;
    }
    
    colIndex = index % numCols;
    rowIndex = index / numCols;
    
    color = getColor(pixels, colIndex, rowIndex);
    
    while(combineColors(pixels, gridSpan, colIndex, rowIndex, color)) {
      gridSpan++;
    }
    
    ofSetColor(color);
    ofDrawRectangle(colIndex * gridWidth + squareOffsetX - 0.5, rowIndex * gridWidth + squareOffsetY - 0.5, gridWidth * gridSpan - 1, gridWidth * gridSpan - 1);
    
    map[index] = 1;
    index += gridSpan;
    gridSpan = 1;
  }
}

bool ofApp::combineColors(ofPixels& pixels, float gridSpan, int colIndex, int rowIndex, ofColor color) {
  if(colIndex + gridSpan >= numCols || rowIndex + gridSpan >= numRows) return false;
  
  int i, l;
  int index;
  int gridColIndex, gridRowIndex;
  ofColor color2;
  vector<int> indeces;
  
  for(i = 0; i < gridSpan; i++) {
    gridColIndex = colIndex + gridSpan;
    gridRowIndex = rowIndex + i;
    index = gridRowIndex * numCols + gridColIndex;
    
    color2 = getColor(pixels, gridColIndex, gridRowIndex);
    if(map[index] == 1 || color != color2) {
      return false;
    }
    indeces.push_back(index);
  }
  
  for(i = 0, l = gridSpan + 1; i < l; i++) {
    gridColIndex = colIndex + i;
    gridRowIndex = rowIndex + gridSpan;
    index = gridRowIndex * numCols + gridColIndex;
    
    color2 = getColor(pixels, gridColIndex, gridRowIndex);
    if(map[index] == 1 || color != color2) {
      return false;
    };
    indeces.push_back(index);
  }
  
  for(i = 0, l = indeces.size(); i < l; i++) {
    map[indeces[i]] = 1;
  }
  
  return true;
}

ofColor ofApp::getColor(ofPixels& pixels, int colIndex, int rowIndex) {
  int baseColIndex = colIndex * gridWidth + squareOffsetX;
  int baseRowIndex = rowIndex * gridWidth + squareOffsetY;
  int col = 0;
  int cnt = 0;

  for(int i = 0; i < gridWidth; i++) {
    int rIndex = baseRowIndex + i;
    for(int j = 0; j < gridWidth; j++) {
      int cIndex = ofGetWidth() - (baseColIndex + j);
    
      if(cIndex >= 0 && rIndex >= 0 && cIndex < ofGetWidth() && rIndex < ofGetHeight()) {
        col += int(pixels[rIndex * ofGetWidth() + cIndex]);
        cnt++;
      }
    }
  }
  
  
  if(cnt != 0) col /= cnt;
  
  return ofColor(col, col, col, 255);
}

void ofApp::setGridWidth(int w) {
  gridWidth = w;
  numCols = ceil(ofGetWidth() / gridWidth) + 1;
  numRows = ceil(ofGetHeight() / gridWidth) + 1;
  numAllGrids = numCols * numRows;
  
  squareOffsetX = (ofGetWidth() - numCols * gridWidth) / 2;
  squareOffsetY = (ofGetHeight() - numRows * gridWidth) / 2;
  
  map = vector<int>(numAllGrids);
}

void ofApp::mouseMoved(int x, int y ){
  setGridWidth(ofMap(y, 0, ofGetHeight(), 4, 64));
}
