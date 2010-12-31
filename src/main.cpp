#include <SFML/Window.hpp>
#include <renderer.h>
#include <input.h>
#include <octree.h>
#include <iostream>

using namespace std;

int main()
{
    // Create the main window
    sf::Window App(sf::VideoMode(800, 600, 32), "SFML OpenGL");
    
    // Create a renderer and input handler
    Renderer renderer;
    InputHandler input_handler(&App);
    
//    App.UseVerticalSync(true);

    // Create a clock for measuring time elapsed
    sf::Clock Clock;

    const float Speed = 5.f;
    float Left = 0.f;
    float Top  = 0.f;

    Octree<float> myfloats[] = {Octree<float>(1.0), Octree<float>(2.0), Octree<float>(50), Octree<float>(3.5), Octree<float>(1.5), Octree<float>(56.0), Octree<float>(3.0), Octree<float>(1.0)};
    vector<Octree<float> > treechildren;
    for(int i = 0; i < 8; i++)
        treechildren.push_back(myfloats[i]);

    Octree<float> tree = Octree<float>(treechildren);

    for(int i = 0; i < 8; i++)
        cout << i << ": " << tree.children[i].value << endl;
    // Start game loop
    while (App.IsOpened())
    {
        float ElapsedTime = Clock.GetElapsedTime();
        Clock.Reset();
        
        if (App.GetInput().IsKeyDown(sf::Key::Left))  Left -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Right)) Left += Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Up))    Top  -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Down))  Top  += Speed * ElapsedTime;
        
        input_handler.handleEvents();

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();

        renderer.render(Left, Top);

        // Finally, display rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}

