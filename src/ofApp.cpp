/*
Kelia Murata
Randomly generated terrain noise program 
*/

#include "ofApp.h"

//-----------------------------------------------------------------------------------------
//
void ofApp::setup()
{
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
	
	// Give us a starting point for the camera
	camera.setNearClip(0.01f);
	camera.setPosition( 0, 70, 70 );
	camera.lookAt(ofVec3f(0,0,0));
	camera.setMovementMaxSpeed( 0.1f );
    
	//sets deminsions for the mesh 
	meshWidth = 100;
	meshHeight = 100; 

	//forloops to go through each point in the mesh 
	for(int i = 0; i < meshWidth; i++)
	{
		for (int j = 0; j < meshHeight; j++)
		{
			//adds a vertices to each point 
			mesh.addVertex(ofPoint((i - meshWidth/2), (j - meshHeight/2), 0));
			//gives an inital color 
			mesh.addColor(ofColor(255));
		}
	}

	//go through each point again 
	for(int y = 0; y < meshHeight -1; y++)
	{
		for(int x = 0; x < meshWidth -1; x++)
		{
			//save our points
			//grabs the point, the point next to it, the one below it, and the one above it in the mesh
			int p1 = x + meshWidth * y; 
			int p2 = x + 1 + meshWidth * y;
			int p3 = x + meshWidth * (y + 1); 
			int p4 = x + 1 + meshWidth * (y + 1);
			//create our triangles (counter clockwise) 
			mesh.addTriangle(p1, p2, p3);
			mesh.addTriangle(p2, p4, p3);	
		}
	}
	setNormals(mesh);

}

//-----------------------------------------------------------------------------------------
//
void ofApp::update()
{
	//grab the random time 
	float timer = ofGetElapsedTimef();
	//forloops to go through each image 
	for(int y = 0; y < meshHeight; y++)
	{
		for(int x = 0; x < meshWidth; x++)
		{
			//look at the points and grab their vertex 
			int p = x + meshWidth * y;
			ofPoint point = mesh.getVertex(p);

			//get our noise value samples 
			float noiseValue = ofSignedNoise(x * .15, y * .15, timer * .15);
			float noiseValue2 = ofSignedNoise(x * .05, y * .05, timer * .05);
			noiseValue += noiseValue2 * 2;

			//move the z value according to our noise 
			point.z = noiseValue * 5;
			//set the mesh vertext 
			mesh.setVertex( p, point);

			//check the high of this points z position (which was just reset) 
			//color the mesh accroding to its z hight to create a topography map 
			if(point.z < - 5)
			{
				mesh.setColor(p, ofColor::blue);
			}
			if( (point.z >= -4) && (point.z < 2))
			{
				mesh.setColor(p, ofColor::darkGreen);
			}
			if( (point.z >= 2) && (point.z < 6))
			{
				mesh.setColor(p, ofColor::gray);
			}
			if(point.z >= 6)
			{
				mesh.setColor(p, ofColor::white);
			}
		}
	}
	//calls the added function set normals on the mesh 
	setNormals(mesh);
}

//-----------------------------------------------------------------------------------------
//
void ofApp::draw()
{
	//color the background gradiant in a circle 
	ofBackgroundGradient( ofColor(40,40,40), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);	
	
	ofEnableDepthTest();
	camera.begin();
	
		// draw a grid on the floor
		ofSetColor( ofColor(60) );
		ofPushMatrix();
			ofRotate(90, 0, 0, -1);
			ofDrawGridPlane( 50, 10, false );
		ofPopMatrix();

		//rotate our mesh and draw it 
		ofPushMatrix();
			ofRotate(90, -1, 0,0);
			ofTranslate(0, 0, -2.5);
			mesh.draw();
		ofPopMatrix();
	
	camera.end();
	ofDisableDepthTest();
	ofSetColor( ofColor::white );

	fontSmall.drawStringShadowed(ofToString(ofGetFrameRate(),2), ofGetWidth()-35, ofGetHeight() - 6, ofColor::whiteSmoke, ofColor::black );
}

//-----------------------------------------------------------------------------------------//
//Universal function which sets normals for the triangle mesh
void ofApp::setNormals( ofMesh &mesh ){
    
    //The number of the vertices
    int nV = mesh.getNumVertices();
    
    //The number of the triangles
    int nT = mesh.getNumIndices() / 3;
    
    vector<ofPoint> norm( nV ); //Array for the normals
    
    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t=0; t<nT; t++) {
        
        //Get indices of the triangle t
        int i1 = mesh.getIndex( 3 * t );
        int i2 = mesh.getIndex( 3 * t + 1 );
        int i3 = mesh.getIndex( 3 * t + 2 );
        
        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex( i1 );
        const ofPoint &v2 = mesh.getVertex( i2 );
        const ofPoint &v3 = mesh.getVertex( i3 );
        
        //Compute the triangle's normal
        ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();
        
        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }
    
    //Normalize the normal's length
    for (int i=0; i<nV; i++) {
        norm[i].normalize();
    }
    
    //Set the normals to mesh
    mesh.clearNormals();
    mesh.addNormals( norm );
}

//allows user to toggle fullscreen by pressing f key 
void ofApp::keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
}
