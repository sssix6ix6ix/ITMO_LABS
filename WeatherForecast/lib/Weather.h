#pragma once

#include <chrono>
#include <iomanip>
#include <map>
#include <sstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum WeatherCode
{
    Clear = 0,
    MostlyClear = 1,
    PartlyCloudy = 2,
    Cloudy = 3,
    Fog = 45,
    Frost = 48,
    LightDrizzle = 51,
    ModerateDrizzle = 53,
    HeavyDrizzle = 55,
    LightFreezingRain = 56,
    HeavyFreezingRain = 57,
    LightRain = 61,
    ModerateRain = 63,
    HeavyRain = 65,
    IceRain = 66,
    HeavyIceRain = 67,
    LightSnow = 71,
    ModerateSnow = 73,
    HeavySnow = 75,
    SnowFlurry = 77,
    LightRainShower = 80,
    ModerateRainShower = 81,
    HeavyRainShower = 82,
    LightSnowShower = 85,
    HeavySnowShower = 86,
    Thunderstorm = 95,
    ThunderstormWithSmallHail = 96,
    ThunderstormWithLargeHail = 99,
    GoodWeather = 100
};

std::string WhatWeather(WeatherCode code);

class Weather {
public:
    Weather() = default;

    Weather(WeatherCode weather_code,
            double apparent_temperature,
            double rain_val,
            double relative_humidity,
            double temperature,
            double wind_speed,
            time_t date_val);

    Weather& operator=(const Weather& other) = default;

    ~Weather() = default;

    WeatherCode GetWeatherCode() const;

    double GetApparentTemperature() const;

    double GetRain() const;

    double GetRelativeHumidity() const;

    double GetTemperature() const;

    double GetWindSpeed() const;

    std::string GetDate() const;

private:
    WeatherCode weather_code_;
    double apparent_temperature_;
    double rain_;
    double relative_humidity_;
    double temperature_;
    double wind_speed_;
    time_t date_;
};

const std::vector<std::string_view> kCodeUnknown = {
        "    .-.      ",
        "     __)     ",
        "    (        ",
        "     `-'     ",
        "      •      "
};

const std::vector<std::string_view> kCodeCloudy = {
        "             ",
        "     .--.    ",
        "  .-(    ).  ",
        " (___.__)__) ",
        "             "
};

const std::vector<std::string_view> kCodeFog = {
        "             ",
        " _ - _ - _ - ",
        "  _ - _ - _  ",
        " _ - _ - _ - ",
        "             "
};

const std::vector<std::string_view> kCodeHeavyRain = {
        "     .-.     ",
        "    (   ).   ",
        "   (___(__)  ",
        "  ,',',','   ",
        "  ,',',','   "
};

const std::vector<std::string_view> kCodeHeavyShowers = {
        " _`/\"\".-.    ",
        "  ,\\_(   ).  ",
        "   /(___(__) ",
        "   ,',',','  ",
        "   ,',',','  "
};

const std::vector<std::string_view> kCodeHeavySnow = {
        "     .-.     ",
        "    (   ).   ",
        "   (___(__)  ",
        "   * * * *   ",
        "  * * * *    "
};

const std::vector<std::string_view> kCodeHeavySnowShowers = {
        " _`/\"\".-.    ",
        "  ,\\_(   ).  ",
        "   /(___(__) ",
        "    * * * *  ",
        "   * * * *   "
};

const std::vector<std::string_view> kCodeLightRain = {
        "     .-.     ",
        "    (   ).   ",
        "   (___(__)  ",
        "    ' ' ' '  ",
        "   ' ' ' '   "
};

const std::vector<std::string_view> kCodeLightShowers = {
        " _`/\"\".-.    ",
        "  ,\\_(   ).  ",
        "   /(___(__) ",
        "     ' ' ' ' ",
        "    ' ' ' '  "
};

const std::vector<std::string_view> kCodeLightSleet = {
        "     .-.     ",
        "    (   ).   ",
        "   (___(__)  ",
        "    ' * ' *  ",
        "   * ' * '   "
};

const std::vector<std::string_view> kCodeLightSleetShowers = {
        " _`/\"\".-.    ",
        "  ,\\_(   ).  ",
        "   /(___(__) ",
        "     ' * ' * ",
        "    * ' * '  "
};

const std::vector<std::string_view> kCodeLightSnow = {
        "     .-.     ",
        "    (   ).   ",
        "   (___(__)  ",
        "    *  *  *  ",
        "   *  *  *   "
};

const std::vector<std::string_view> kCodeLightSnowShowers = {
        " _`/\"\".-.    ",
        "  ,\\_(   ).  ",
        "   /(___(__) ",
        "     *  *  * ",
        "    *  *  *  "
};

const std::vector<std::string_view> kCodePartlyCloudy = {
        "   \\         ",
        " _ /\"\".-.    ",
        "   \\_(   ).  ",
        "   /(___(__) ",
        "             "
};

const std::vector<std::string_view> kCodeSunny = {
        "    \\   /    ",
        "     .-.     ",
        "  - (   ) -  ",
        "     `-'     ",
        "    /   \\    "
};

const std::vector<std::string_view> kCodeThunderyHeavyRain = {
        "     .-.     ",
        "    (   ).   ",
        "   (___(__)  ",
        "  ,'7',7,'   ",
        "  ,','7','   "
};

const std::vector<std::string_view> kCodeThunderyShowers = {
        " _`/\"\".-.    ",
        "  ,\\_(   ).  ",
        "   /(___(__) ",
        "    7' '7' ' ",
        "    ' ' ' '  "
};

const std::vector<std::string_view> kCodeThunderySnowShowers = {
        " _`/\"\".-.    ",
        "  ,\\_(   ).  ",
        "   /(___(__) ",
        "     *7 *7 * ",
        "    *  *  *  "
};

const std::vector<std::string_view> kCodeVeryCloudy = {
        "             ",
        "     .--.    ",
        "  .-(    ).  ",
        " (___.__)__) ",
        "             "
};

const std::vector<std::string_view>& GetWeatherPicture(WeatherCode weather_code);

time_t GetDateFromJSON(const std::string& json_weather);

std::string WhatWeather(WeatherCode code);
