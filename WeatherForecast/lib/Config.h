#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

constexpr size_t kCountOfHours = 384;
constexpr size_t kCountOfDays = 16;

constexpr size_t kKeyboardCodeOfesc = 27;
constexpr size_t kKeyboardCodeOfq = 113;
constexpr size_t kKeyboardCodeOfn = 110;
constexpr size_t kKeyboardCodeOfp = 112;
constexpr size_t kKeyboardCodeOfPlus = 43;
constexpr size_t kKeyboardCodeOfMinus = 45;

constexpr size_t kTimeOfMorning = 6;
constexpr size_t kTimeOfDay = 12;
constexpr size_t kTimeOfAfternoon = 18;
constexpr size_t kTimeOfNight = 24;
constexpr size_t kHoursInDay = 24;

using Cords = std::pair<double, double>;
using json = nlohmann::json;

class Config {
public:

    Config()
    : cities_({"Penza"})
    , frequency_()
    , number_of_days_()
    {}

    explicit Config(const json& config)
    : cities_(config["cities"])
    , frequency_(config["frequency"])
    , number_of_days_(config["days"])
    {}

    Config(const std::filesystem::path& file_of_config);

    ~Config() = default;

    [[nodiscard]] size_t NumberOfCities() const;

    [[nodiscard]] size_t GetFrequency() const;

    [[nodiscard]] size_t GetNumberOfDays() const;

    [[nodiscard]] std::string GetCity(size_t index) const;

private:
  std::vector<std::string> cities_;
  size_t frequency_;
  size_t number_of_days_;
};

enum Command {
    escape = kKeyboardCodeOfesc,
    quit = kKeyboardCodeOfq,
    next = kKeyboardCodeOfn,
    previous = kKeyboardCodeOfp,
    increase_number_of_day = kKeyboardCodeOfPlus,
    decrease_number_of_day = kKeyboardCodeOfMinus
};

json GetJSONFromResponse(const std::string& request);

Cords GetCordsOfCity(const std::string& city);

json GetJSONWeather(const Cords& info_about_city);