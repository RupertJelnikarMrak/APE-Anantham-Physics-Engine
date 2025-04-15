#include "App/App.hpp"

namespace App
{

void App::run()
{
    engine.update();
    engine.render();
}

} // namespace App
