#include "Application.h"

int main(int argc, char *argv[])
{
    PandaSpriteEditor::Application app(argc, argv);
    app.initialise();
    return app.run();
}
