#include <iostream>

#include "Engine.h"

#include "../FDGL/test/GLFWImpl.h"
#include "../FDGL/test/GLUtils.h"

#include "../FDGL/test/Renderer.h"

using namespace std;

int main(int argc, char *argv[])
{
    Context ctx;
    Renderer renderer(ctx);
    Window w;

    Engine engine(ctx, w, renderer);
    engine.init(argc, argv);

    return engine.run();
}
