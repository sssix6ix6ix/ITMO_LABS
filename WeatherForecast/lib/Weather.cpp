#pragma once
#include "Weather.h"

time_t GetDateFromJSON(const std::string& date_string) {
    std::stringstream ss(date_string);
    tm date_time = {};
    ss >> std::get_time(&date_time, "%Y-%m-%dT%H:%M:%S");

    auto time_point = std::chrono::system_clock::from_time_t(mktime(&date_time));
    time_t time = std::chrono::system_clock::to_time_t(time_point);

    return mktime(&date_time);
}

std::string WhatWeather(WeatherCode code) {
    switch (code) {
        case Clear:
            return "Ясно";
        case MostlyClear:
            return "В основном ясно";
        case PartlyCloudy:
            return "Немного облачно";
        case Cloudy:
            return "Пасмурное небо";
        case Fog:
            return "Туман";
        case Frost:
            return "Иней";
        case LightDrizzle:
            return "Легкая морось";
        case ModerateDrizzle:
            return "Умеренная морось";
        case HeavyDrizzle:
            return "Интенсивная морось";
        case LightFreezingRain:
            return "Легкая гололедица";
        case HeavyFreezingRain:
            return "Сильная гололедица";
        case LightRain:
            return "Слабый дождь";
        case ModerateRain:
            return "Средний дождь";
        case HeavyRain:
            return "Сильный дождь";
        case IceRain:
            return "Ледяной дождь";
        case HeavyIceRain:
            return "Сильный ледяной дождь";
        case LightSnow:
            return "Слабый снег";
        case ModerateSnow:
            return "Умеренный снег";
        case HeavySnow:
            return "Сильный снег";
        case SnowFlurry:
            return "Снежные хлопья";
        case LightRainShower:
            return "Легкий ливень";
        case ModerateRainShower:
            return "Средний ливень";
        case HeavyRainShower:
            return "Сильный ливень";
        case LightSnowShower:
            return "Слабый снегопaд";
        case HeavySnowShower:
            return "Сильный снег";
        case Thunderstorm :
            return "Гроза";
        case ThunderstormWithSmallHail :
            return "Гроза с мелким градом";
        case ThunderstormWithLargeHail :
            return "Гроза с крупным градом";
        case GoodWeather :
            return "Хорошая погода";
    }

    throw std::runtime_error("bad weather code");
};



Weather::Weather(WeatherCode weather_code,
                double apparent_temperature, 
                double rain_val, 
                double relative_humidity, 
                double temperature,
                double wind_speed,
                time_t date_val)
{
    this->weather_code_ = weather_code;
    this->apparent_temperature_ = apparent_temperature;
    this->rain_ = rain_val;
    this->relative_humidity_ = relative_humidity;
    this->temperature_ = temperature;
    this->wind_speed_ = wind_speed;
    this->date_ = date_val;
}

WeatherCode Weather::GetWeatherCode() const {
    return weather_code_;
}   

double Weather::GetApparentTemperature() const {
    return apparent_temperature_;
}

double Weather::GetRain() const {
    return rain_;
}

double Weather::GetRelativeHumidity() const {
    return relative_humidity_;
}

double Weather::GetTemperature() const {
    return temperature_;
}

double Weather::GetWindSpeed() const {
    return wind_speed_;
}

std::string Weather::GetDate() const {
    char buf[64];
    strftime(buf, std::size(buf), "%d.%m.%Y", localtime(&date_));

    return buf;
}

const std::vector<std::string_view>& GetWeatherPicture(WeatherCode weather_code) {
    switch (weather_code) {
        case Clear:
            return kCodeSunny;
        case MostlyClear:
        case PartlyCloudy:
            return kCodeCloudy;
        case Cloudy:
            return kCodeVeryCloudy;
        case Fog:
        case Frost:
            return kCodeFog;
        case LightDrizzle:
        case ModerateDrizzle:
        case HeavyDrizzle:
        case LightFreezingRain:
        case HeavyFreezingRain:
        case LightRain:
            return kCodeLightRain;
        case ModerateRain:
        case HeavyRain:
        case IceRain:
        case HeavyIceRain:
            return kCodeHeavyRain;
        case LightSnow:
        case ModerateSnow:
            return kCodeLightSnow;
        case HeavySnow:
        case SnowFlurry:
            return kCodeHeavySnow;
        case LightRainShower:
            return kCodeLightShowers;
        case ModerateRainShower:
        case HeavyRainShower:
            return kCodeHeavySnow;
        case LightSnowShower:
            return kCodeLightSnowShowers;
        case HeavySnowShower:
            return kCodeHeavySnowShowers;
        case Thunderstorm:
            return kCodeThunderyHeavyRain;
        case ThunderstormWithSmallHail:
        case ThunderstormWithLargeHail:
            return kCodeThunderySnowShowers;
    }

    return kCodeUnknown;
}
