#include "sandpile.h"
#include "BMPImage.cpp"

#include <deque>
#include <queue>

void SandPile::init(const uint32_t& height_by_parser, const uint32_t& width_by_parser) {
    height = height_by_parser;
    width = width_by_parser;
};

void SandPile::SandPileSimulation(BMPImage& image, const std::string& input, const std::string& out,
                                  const uint32_t& max_iter, const uint32_t& freq) {
    std::deque<std::deque<uint16_t>> used(height, std::deque<uint16_t>(width, 0));
    std::deque<std::deque<uint64_t>> graph(height, std::deque<uint64_t>(width, 0));

    std::ifstream file_in(input);
    uint64_t first_x;
    uint64_t first_y;
    uint64_t sand_number;
    std::queue<Cord> iterations;

    while (file_in >> first_x >> first_y >> sand_number) {
        graph[first_y][first_x] = sand_number;

        if (sand_number >= kOverSand) {
            iterations.emplace(std::make_pair(first_x, first_y));
        }
    }

    uint64_t number_iteration = 0;
    uint64_t plus_down = 0;
    uint64_t plus_left = 0;

    while (!iterations.empty() && number_iteration != max_iter) {
        std::cout << "Compiling iteration " << number_iteration << '\n';
        number_iteration++;
        std::queue<Cord> temp;

        while (!iterations.empty()) {
            Cord cell = iterations.front();
            iterations.pop();

            uint16_t X = cell.x + plus_left;
            uint16_t Y = cell.y + plus_down;

            if (X + 1 == width) {
                width++;

                for (int i = 0; i < height; i++) {
                    used[i].push_back(0);
                    graph[i].push_back(0);
                }
            }

            if (X == 0) {
                plus_left++;
                X++;
                width++;

                for (size_t i = 0; i < height; i++) {
                    used[i].push_front(0);
                    graph[i].push_front(0);
                }
            }

            if (Y + 1 == height) {
                height++;
                used.emplace_back(std::deque<uint16_t>(width, 0));
                graph.emplace_back(std::deque<uint64_t>(width, 0));
            }

            if (Y == 0) {
                Y++;
                plus_down++;
                height++;
                used.emplace_front(std::deque<uint16_t>(width, 0));
                graph.emplace_front(std::deque<uint64_t>(width, 0));
            }

            uint64_t plus_des = graph[Y][X] / 4;

            graph[Y][X] %= kOverSand;
            graph[Y + 1][X] += plus_des;
            graph[Y - 1][X] += plus_des;
            graph[Y][X - 1] += plus_des;
            graph[Y][X + 1] += plus_des;


            if ((graph[Y + 1][X] >= kOverSand) && (used[Y + 1][X] != number_iteration)) {
                temp.emplace(std::make_pair(cell.x, cell.y + 1));
                used[Y + 1][X] = number_iteration;
            }

            if ((graph[Y - 1][X] >= kOverSand) && (used[Y - 1][X] != number_iteration)) {
                temp.emplace(std::make_pair(cell.x, cell.y - 1));
                used[Y - 1][X] = number_iteration;
            }

            if ((graph[Y][X + 1] >= kOverSand) && (used[Y][X + 1] != number_iteration)) {
                temp.emplace(std::make_pair(cell.x + 1, cell.y));
                used[Y][X + 1] = number_iteration;
            }

            if (graph[Y][X - 1] >= kOverSand && used[Y][X - 1] != number_iteration) {
                temp.emplace(std::make_pair(cell.x - 1, cell.y));
                used[Y][X - 1] = number_iteration;
            }
        }

        iterations = temp;

        if ((freq == 0 && temp.empty()) || (freq != 0 && (number_iteration % freq == 0))) {
            std::string name_of_image = out + "\\frame" + std::to_string(number_iteration) + ".bmp";
            DrawImage(image, graph, name_of_image);
        }
    }
}
