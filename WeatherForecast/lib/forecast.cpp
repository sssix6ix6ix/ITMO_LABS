#include "forecast.h"

Forecast::Forecast(const std::string& city, const json& json_of_weather)
    : city_(city)
    , data_about_weather_(kCountOfHours)
    , last_update_(time(0))
{
    UpdateDate(json_of_weather);
}

Forecast::Forecast(const std::string& city)
    : city_(city)
    , data_about_weather_(kCountOfHours)
{
    UpdateDate();
}


void Forecast::UpdateDate() {
    UpdateDate(city_);
}

void Forecast::UpdateDate(const std::string& city) {
    Cords cords_of_city = GetCordsOfCity(city);
    json json_weather = GetJSONWeather(cords_of_city);

    UpdateDate(json_weather);
}


void Forecast::UpdateDate(const json& weather) {
    std::ofstream fout(city_ + ".json");
    fout << weather;

    last_update_ = time(NULL);

    for (int i = 0; i < kCountOfHours; i++) {
        auto weather_code_from_json = weather["hourly"]["weathercode"][i];
        WeatherCode weather_code;

        if (weather_code_from_json.is_null()) {
            weather_code = GoodWeather;
        } else {
            weather_code = weather_code_from_json;
        }

        data_about_weather_[i] = {
                weather_code,
                FromJsonValueToDouble(weather["hourly"]["apparent_temperature"][i]),
                FromJsonValueToDouble (weather["hourly"]["rain"][i]),
                FromJsonValueToDouble(weather["hourly"]["relativehumidity_2m"][i]),
                FromJsonValueToDouble(weather["hourly"]["temperature_2m"][i]),
                FromJsonValueToDouble(weather["hourly"]["windspeed_10m"][i]),
                GetDateFromJSON(weather["hourly"]["time"][i])
        };
    }
}

std::string make_ceil(const Weather& weather) {
    std::vector<std::string_view> picture = GetWeatherPicture(weather.GetWeatherCode());
    std::stringstream ss;

    ss << picture[0] << std::setw(kWidthOfCeil) << WhatWeather(weather.GetWeatherCode()) << '\n';
    ss << picture[1] << std::setw(kWidthOfCeil) << weather.GetTemperature() << " °C\n";
    ss << picture[2] << std::setw(kWidthOfCeil) << weather.GetWindSpeed() << " км/ч \n";
    ss << picture[3] << std::setw(kWidthOfCeil) << weather.GetRain() << " мм" << '\n';
    ss << picture[4] << std::setw(kWidthOfCeil) << weather.GetRelativeHumidity() << " %";

    return ss.str();
}

void Forecast::PrintCurrentWeather() {
    std::time_t time_in_seconds = time(NULL);
    std::tm* now_time = std::localtime(&time_in_seconds);
    size_t hour = now_time->tm_hour;

    std::cout << "Дата : " << std::put_time(now_time, "%d-%m-%Y") << '\n';
    std::cout << "Время : " << std::put_time(now_time, "%H:%M") << '\n';

    std::cout << "Погода в " << city_ << std::endl;

    Weather& weather = data_about_weather_[hour];
    std::vector<std::string_view> picture = GetWeatherPicture(weather.GetWeatherCode());

    std::cout << picture[0] << std::setw(kWidthOfCeil) << WhatWeather(weather.GetWeatherCode()) << std::endl;
    std::cout << picture[1] << std::setw(kWidthOfCeil) << "Температура: " << weather.GetTemperature() << " °C\n";
    std::cout << picture[2] << std::setw(kWidthOfCeil) << "Скорость ветра: " << weather.GetWindSpeed() << " км/ч \n";
    std::cout << picture[3] << std::setw(kWidthOfCeil) << "Осадки: " <<weather.GetRain() << " мм" << '\n';
    std::cout << picture[4] << std::setw(kWidthOfCeil) << "Влажность: " << weather.GetRelativeHumidity() << " %" << '\n';
}

void Forecast::PrintWeather(size_t number_of_day, size_t count) {
    system("cls");

    PrintCurrentWeather();

    for (size_t day = 0; day < count; day++) {
        fort::utf8_table table;
        table.set_border_style(FT_BOLD2_STYLE);
        table.row(kRowTimesOfDay).set_cell_text_align(fort::text_align::center);
        table.set_cell_min_width(kCellMinWidth);

        size_t now_day = (number_of_day + day) % kCountOfDays;

        table << fort::header << data_about_weather_[kHoursInDay * now_day].GetDate() << fort::endr
              << "Утро" << "День" << "Вечер" << "Ночь" << fort::endr
              << make_ceil(data_about_weather_[kTimeOfMorning + kHoursInDay * now_day])
              << make_ceil(data_about_weather_[kTimeOfDay + kHoursInDay * now_day])
              << make_ceil(data_about_weather_[kTimeOfAfternoon + kHoursInDay * now_day])
              << make_ceil(data_about_weather_[kTimeOfNight + kHoursInDay * now_day]);

        std::cout << table.to_string();
    }
}

time_t Forecast::GetLastUpdate() const {
    return last_update_;
}

void NextCity(size_t& index_of_city, size_t count_of_cities) {
    ++index_of_city;
    index_of_city %= count_of_cities;
}

void PreviousCity(size_t& index_of_city, size_t count_of_cities) {
    index_of_city += count_of_cities;
    --index_of_city;
    index_of_city %= count_of_cities;
}


void IncreaseNumberOfDay(size_t& number_of_day) {
    ++number_of_day;
    number_of_day %= kCountOfDays;
}

void DecreaseNumberOfDay(size_t& number_of_day) {
    number_of_day += kCountOfDays;
    --number_of_day;
    number_of_day %= kCountOfDays;
}


void CheckKeyboard(Command& command, bool& flag) {
    while (command != escape && command != quit) {
        if (_kbhit())  {
            printf("%s", kRemoveLastRow);
            command = static_cast<Command>(getch());
            flag = true;
        }
    }
}

void DoCommand(Command& command, bool& flag,
               std::vector<Forecast>& forecasts,
               const Config& config) {
    size_t index_of_city = 0;
    size_t number_of_day = 0;
    size_t count_of_cities = forecasts.size();
    size_t count_of_days = config.GetNumberOfDays();

    while (command != escape && command != quit) {
        time_t now_time = time(NULL);

        if (now_time - forecasts[index_of_city].GetLastUpdate() >= config.GetFrequency()) {
            forecasts[index_of_city].UpdateDate();
            forecasts[index_of_city].PrintWeather(number_of_day, count_of_days);
        }

        if (flag) {
            switch (command) {
                case quit: case escape:
                    printf("Завершение программы");
                    break;

                case next:
                    NextCity(index_of_city, count_of_cities);
                    forecasts[index_of_city].PrintWeather(number_of_day, count_of_days);
                    break;

                case previous:
                    PreviousCity(index_of_city, count_of_cities);
                    forecasts[index_of_city].PrintWeather(number_of_day, count_of_days);
                    break;

                case increase_number_of_day:
                    IncreaseNumberOfDay(number_of_day);
                    forecasts[index_of_city].PrintWeather(number_of_day, count_of_days);
                    break;

                case decrease_number_of_day:
                    DecreaseNumberOfDay(number_of_day);
                    forecasts[index_of_city].PrintWeather(number_of_day, count_of_days);
                    break;

                default :
                    printf("error key\n");
                    break;
            }

            flag = false;

            printf("press \"esc\" or \"q\" to exit \r");
        }
    }
}

void WeatherForecast() {

    std::ifstream fin("Config.txt");

    if (!fin.is_open()) {
        throw std::runtime_error("Problems opening config");
    }

    json json_of_config;
    fin >> json_of_config;

    Config config(json_of_config);

    std::vector<Forecast> forecasts(config.NumberOfCities());

    for (size_t i = 0; i < config.NumberOfCities(); i++) {
        forecasts[i] = Forecast(config.GetCity(i));
    }

    Command command = next;
    bool flag = true;

    std::thread checking_of_touches(CheckKeyboard, std::ref(command), std::ref(flag));
    std::thread do_command(DoCommand,
                    std::ref(command), std::ref(flag),
                    std::ref(forecasts), std::cref(config));

    checking_of_touches.join();
    do_command.join();
}
