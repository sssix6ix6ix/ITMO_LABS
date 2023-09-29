#include "lib/forecast.cpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    try {
        WeatherForecast();
    }
    catch (std::runtime_error& ex) {
        std::cout << ex.what();
    }
    catch (nlohmann::detail::parse_error& err) {
        std::cout << "Wrong config format";
    }

    return 0;
}
