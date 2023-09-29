#include "Config.h"

json GetJSONFromResponse(const std::string& request) {
    std::string tmp = std::string(request.substr(1, request.size() - 2));

    if (tmp.empty()) {
        throw std::runtime_error("bad request");
    }

    return (json::parse(tmp));
}

Cords GetCordsOfCity(const std::string& city) {
    std::string cords_url = "https://api.api-ninjas.com/v1/city?name=" + city;
    cpr::Response cords = cpr::Get(cpr::Url{cords_url},
                                    cpr::Header{{"X-Api-Key", "W/4nRpsrMp7C5xuEydU25Q==3T5QfAb5c72hO8LS"}},
                                    cpr::Authentication{"SeDead70@gmail.com", "lab10password", cpr::AuthMode::BASIC});
    json info_about_city = GetJSONFromResponse(cords.text);

    double latitude = info_about_city["latitude"];
    double longitude = info_about_city["longitude"];

    return {latitude, longitude};
}

json GetJSONWeather(const Cords& cords) {
    std::string weather_url = "https://api.open-meteo.com/v1/forecast";

    cpr::Parameters parameters = {{"latitude", std::to_string(cords.first)},
                                  {"longitude", std::to_string(cords.second)},
                                  {"current_weather", "true"},
                                  {"timezone", "Europe/Moscow"},
                                  {"hourly", "temperature_2m,windspeed_10m,relativehumidity_2m,apparent_temperature,rain,weathercode"},
                                  {"forecast_days", "16"}};

    cpr::Response weather = cpr::Get(cpr::Url{weather_url}, parameters);

    return json::parse(weather.text);
}

size_t Config::NumberOfCities() const {
    return cities_.size();
}

size_t Config::GetNumberOfDays() const {
    return number_of_days_;
}

std::string Config::GetCity(size_t index) const {
    return cities_[index];
}

size_t Config::GetFrequency() const {
    return frequency_;
}

