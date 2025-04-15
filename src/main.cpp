#include "Core/first_app.hpp"

#include <exception>
#include <iostream>

int main()
{
    Core::FirstApp app{};

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error occurred!" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
