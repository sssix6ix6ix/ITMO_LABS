#pragma once

#include <conio.h>
#include <ctime>
#include <thread>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "Weather.cpp"
#include "Config.h"
#include "lib/fort.hpp"

constexpr size_t kWidthOfCeil = 8;
constexpr size_t kCellMinWidth = 30;
constexpr size_t kRowTimesOfDay = 1;

constexpr size_t kSizeDate = 64;

const char* kRemoveLastRow = "                          \r";

class Forecast {
public:

    Forecast() = default;

    Forecast(const std::string& city, const json& json_of_weather);

    Forecast(const std::string& city);

    void UpdateDate();

    void UpdateDate(const std::string& weather);

    void UpdateDate(const json& weather);

    void PrintWeather(size_t number_of_day, size_t count);

    void PrintCurrentWeather();

    time_t GetLastUpdate() const;

private:
    std::string city_;
    std::vector<Weather> data_about_weather_;
    time_t last_update_;
};

template <typename json_type>
double FromJsonValueToDouble(const json_type& json_arg) {
    if (json_arg.is_null()) {
        return 0;
    }

    return json_arg;
}

void CheckKeyboard(Command& command, bool& flag);

void DoCommand(Command& c, bool& flag, std::vector<Forecast>& forecasts, const Config& config);

void WeatherForecast();

void NextCity(size_t& index_of_city, size_t count_of_cities);

void PreviousCity(size_t& index_of_city, size_t count_of_cities);

void IncreaseNumberOfDay(size_t& number_of_day);

void DecreaseNumberOfDay(size_t& number_of_day);