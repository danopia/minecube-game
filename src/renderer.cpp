#include "renderer.h"
#include <SFML/System.hpp>

Renderer::Renderer(Terrain initterrain) : terrain(initterrain) {
    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);


    glEnable(GL_CULL_FACE);
    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);


    // Lighting test junk
    GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 				// Ambient Light Values ( NEW )
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				 // Diffuse Light Values ( NEW )
    GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };				 // Light Position ( NEW )
    glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
    glEnable(GL_LIGHT1);							// Enable Light One
    glEnable(GL_LIGHTING);
}

void drawCube(float x, float y, float z, float length) {
    float sublength = length / 2;

    // Apply some transformations
	  glPushMatrix();
    glTranslatef(x + sublength, y + sublength, z + sublength);
    glScalef(sublength, sublength, sublength);

    glBegin(GL_QUADS);
        float randred = sf::Randomizer::Random(0.0f, 1.0f);
        float randblue = sf::Randomizer::Random(0.0f, 1.0f);
        float randgreen = sf::Randomizer::Random(0.0f, 1.0f);

        glColor3f(1.0f, 0.0f, 0.0f);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
		/*glTexCoord2f(0.0f, 0.0f);*/ glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 1 (Front)
		/*glTexCoord2f(1.0f, 0.0f);*/ glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 2 (Front)
		/*glTexCoord2f(1.0f, 1.0f);*/ glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Front)
		/*glTexCoord2f(0.0f, 1.0f);*/ glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 4 (Front)
		// Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);					// Normal Pointing Away From Viewer
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Back)
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 2 (Back)
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 3 (Back)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 4 (Back)
		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);					// Normal Pointing Up
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 1 (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 2 (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Top)
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 4 (Top)
		// Bottom Face
		glNormal3f( 0.0f,-1.0f, 0.0f);					// Normal Pointing Down
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Bottom)
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 2 (Bottom)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 3 (Bottom)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 4 (Bottom)
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);					// Normal Pointing Right
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 1 (Right)
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 2 (Right)
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Right)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 4 (Right)
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);					// Normal Pointing Left
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Left)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 2 (Left)
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 3 (Left)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 4 (Left)
	glEnd();								// Done Drawing Quads
	
	  glPopMatrix();
}

void renderNode(Octree<bool> terrain, float x, float y, float z, float size) {
    if (terrain.hasChildren) {
        float subsize = size / 2;
        renderNode(terrain.children[0], x,         y,         z,         subsize);
        renderNode(terrain.children[1], x+subsize, y,         z,         subsize);
        renderNode(terrain.children[2], x,         y+subsize, z,         subsize);
        renderNode(terrain.children[3], x+subsize, y+subsize, z,         subsize);
        renderNode(terrain.children[4], x,         y,         z+subsize, subsize);
        renderNode(terrain.children[5], x+subsize, y,         z+subsize, subsize);
        renderNode(terrain.children[6], x,         y+subsize, z+subsize, subsize);
        renderNode(terrain.children[7], x+subsize, y+subsize, z+subsize, subsize);
    } else if (terrain.value) {
        drawCube(x, y, z, size);
    }
}

void Renderer::render(float Left, float Top, float Up, float rotation, float zRotation) {

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    
    // Apply some transformations
    glLoadIdentity();
            // Rotate the view first 
        glRotatef(-90 + rotation, 1.f, 0.f, 0.f); 
        glRotatef(zRotation, 0.f, 0.f, 1.f); 
	  glTranslatef(-Left, -Top, -Up);				// Translate The Scene Based On Player Position
	  
    renderNode(terrain.GeneratedTerrain, 0.f, 0.f, 0.f, 50.f);
}
