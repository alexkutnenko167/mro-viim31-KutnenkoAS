#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <cmath>
#include <random>
#include <algorithm>
#include <limits>

using namespace std;

class O {
private:
    int N = 0;
    int* arr = nullptr;
    int dimensions = 0;
    int* arr2 = nullptr;
    int sum = 0;

public:
    O() {
        std::ifstream file("config.txt");

        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла! Создайте файл config.txt" << std::endl;
            return;
        }
        file >> N;
        //std::cout << file.tellg() << std::endl;
        arr = new int[N];

        for (int i = 0; i < N; i++) {
            // Поэлементно строку с количеством элементов в каждом классе
            file >> arr[i];
            sum += arr[i];
            //std::cout << file.tellg() << std::endl;
        }
        file >> dimensions;
        //std::cout << file.tellg() << std::endl;

        sum *= dimensions;
        file.close();
    }

    int Propush(int* arr) {
        std::stringstream ss;
        ss << N << "\n";
        for (int i = 0; i < N; i++) {
            ss << arr[i];
            if (i < N - 1) { // Добавляем пробел между числами
                ss << " ";
            }
        }
        ss << " " << dimensions << "\n";

        std::string numStr = ss.str();
        //std::cout << numStr << std::endl;
        return numStr.length() + 1;
    }

    void GenArray() {
        std::ifstream file("config.txt");
        std::ofstream file2("data.txt");

        file.seekg(Propush(arr), std::ios::beg);
        arr2 = new int[sum];
        int min; int max;
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < dimensions; j++) {
                file >> min;
                file >> max;
                std::cout << '\t' << min << '\n';
                std::cout << '\t' << max << '\n';

                for (int i = 0; i < arr[k]; i++) {
                    arr2[i] = min + rand() % (max - min + 1);
                    std::cout << arr2[i] << ' ';
                    file2 << arr2[i] << ' ';
                }
                std::cout << std::endl;
                file2 << std::endl;
            }
            file2 << std::endl;
        }
        std::cout << "\nОбщее количество элементов: " << sum << std::endl;

        file2.close();
    }

    ~O() {
        delete[] arr;
        delete[] arr2;
    }

    void read_data(std::vector<std::vector<int>>& X, std::vector<int>& y) {
        std::ifstream file("data.txt");
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла data.txt!" << std::endl;
            return;
        }

        std::string line;
        std::vector<int> x_coords, y_coords;
        int count = 0;
        while (std::getline(file, line)) {
            if (line.empty()) continue; // Пропускаем пустые строки
            x_coords.clear();
            y_coords.clear();

            // Считываем координаты X
            std::istringstream x_stream(line);
            int x_val;
            while (x_stream >> x_val) {
                x_coords.push_back(x_val);
            }

            // Считываем координаты Y
            if (std::getline(file, line)) {
                std::istringstream y_stream(line);
                int y_val;
                while (y_stream >> y_val) {
                    y_coords.push_back(y_val);
                }
            }

            // Пропускаем одну строку
            std::getline(file, line);

            // Добавляем данные в X и y
            for (int i = 0; i < x_coords.size(); ++i) {
                X.push_back({ x_coords[i], y_coords[i] });
                y.push_back((count != 0) ? 1 : -1);

            }
            count++;
        }

        file.close();
    }

    void save_data(const std::vector<std::vector<int>>& X, const std::vector<int>& y) {
        std::ofstream datafile("data2.txt");
        if (datafile.is_open()) {
            for (size_t i = 0; i < X.size(); ++i) {
                for (const auto& x : X[i]) {
                    datafile << x << " ";
                    std::cout << x << std::endl;
                }
                datafile << y[i] << "\n"; // Записываем метку y
                std::cout << "\t" << y[i] << std::endl;
            }
            datafile.close();
        }
        else {
            std::cerr << "Не удалось открыть файл для записи данных.\n";
        }
    }

    void ho_kashyap_algorithm(const std::vector<std::vector<int>>& X, const std::vector<int>& y,
        double learning_rate = 0.01, int max_iterations = 100) {

        int d = X[0].size(); // Размерность
        std::vector<double> weights(d, 0.0); // Инициализация весов
        double bias = 0.0; // Инициализация смещения

        save_data(X, y);

        for (int iter = 0; iter < max_iterations; ++iter) {
            bool misclassified = false;

            for (size_t i = 0; i < X.size(); ++i) {
                double activation = bias;

                // Вычисляем активацию
                for (int j = 0; j < d; ++j) {
                    activation += weights[j] * X[i][j];
                }

                // Проверяем условие классификации
                if (y[i] * activation <= 0) {
                    // Обновляем веса и смещение
                    for (int j = 0; j < d; ++j) {
                        weights[j] += learning_rate * y[i] * X[i][j];
                    }
                    bias += learning_rate * 1000 * y[i];
                    misclassified = true;
                }
            }

            // Если нет ошибок классификации, выходим из цикла
            if (!misclassified) {
                break;
            }
        }

        // Выводим результаты
        std::cout << "Обучение завершено. Итерации: " << max_iterations << "\n";
        std::cout << "Веса: ";
        std::ofstream datafile("weights_bias.txt");

        for (const auto& w : weights) {
            std::cout << w << " ";
            datafile << w << " ";
        }
        std::cout << "\nСмещение: " << bias << "\n";
        datafile << "\n" << bias << "\n";
    }

    // Функция для вычисления манхэттенского расстояния
    double manhattan_distance(double x1, double y1, double x2, double y2) {
        return abs(x1 - x2) + abs(y1 - y2);
    }

    // Функция для вычисления эвклидового расстояния
    double euclidean_distance(double x1, double y1, double x2, double y2) {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

    void Metrics(){
        std::ifstream file("data.txt");
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла data.txt!" << std::endl;
            return;
        }

        std::string line;
        std::vector<int> x_coords, y_coords;
        while (std::getline(file, line)) {
            if (line.empty()) continue; // Пропускаем пустые строки
            x_coords.clear();
            y_coords.clear();

            // Считываем координаты X
            std::istringstream x_stream(line);
            int x_val;
            while (x_stream >> x_val) {
                x_coords.push_back(x_val);
            }

            // Считываем координаты Y
            if (std::getline(file, line)) {
                std::istringstream y_stream(line);
                int y_val;
                while (y_stream >> y_val) {
                    y_coords.push_back(y_val);
                }
            }

            // Пропускаем одну строку
            std::getline(file, line);
        }
        std::cout << "Манхэттенское расстояние: " << 
            manhattan_distance(x_coords[0], y_coords[0], x_coords[1], y_coords[1]) << std::endl;
        std::cout << "Эвклидово расстояние: " << 
            euclidean_distance(x_coords[0], y_coords[0], x_coords[1], y_coords[1]) << std::endl;
    }

    // ЛР 3 ----------------------------------------------------------------------------------------------------------------------------------------------
    // Евклидово расстояние между двумя точками
    double euclideanDistance(const std::vector<int>& point1, const std::vector<int>& point2) {
        double distance = 0.0;
        for (size_t i = 0; i < point1.size(); ++i) {
            distance += pow(point1[i] - point2[i], 2);
        }
        return sqrt(distance);
    }

    // Функция нахождения ближайшего центроида
    int findClosestCentroid(const std::vector<int>& point, const std::vector<std::vector<int>>& centroids) {
        double minDistance = std::numeric_limits<double>::max();
        int closestCentroidIndex = -1;
        for (size_t i = 0; i < centroids.size(); ++i) {
            double distance = euclideanDistance(point, centroids[i]);
            if (distance < minDistance) {
                minDistance = distance;
                closestCentroidIndex = i;
            }
        }
        return closestCentroidIndex;
    }

    // Функция обновления центроидов
    std::vector<std::vector<int>> updateCentroids(const std::vector<std::vector<int>>& X, const std::vector<int>& clusters, int k, int dimensions) {
        std::vector<std::vector<int>> newCentroids(k, std::vector<int>(dimensions, 0));
        std::vector<int> clusterCounts(k, 0);

        for (size_t i = 0; i < X.size(); ++i) {
            int clusterIndex = clusters[i];
            clusterCounts[clusterIndex]++;
            for (int j = 0; j < dimensions; ++j) {
                newCentroids[clusterIndex][j] += X[i][j];
            }
        }

        for (int i = 0; i < k; ++i) {
            if (clusterCounts[i] > 0) {
                for (int j = 0; j < dimensions; ++j) {
                    newCentroids[i][j] /= clusterCounts[i];
                }
            }
        }
        return newCentroids;
    }


    void k_means_algorithm(const std::vector<std::vector<int>>& X, const std::vector<int>& y, int k, int maxIterations) {
        if (X.empty() || X[0].empty() || k <= 0 || maxIterations <= 0) {
            std::cerr << "Invalid input parameters." << std::endl;
            return;
        }

        int n = X.size();
        int dimensions = X[0].size();

        // Инициализация центроидов случайными точками из данных
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, n - 1);
        std::vector<std::vector<int>> centroids;
        std::vector<int> centroidIndices;

        for (int i = 0; i < k; ++i) {
            int index;
            do {
                index = distrib(gen);
            } while (std::find(centroidIndices.begin(), centroidIndices.end(), index) != centroidIndices.end());
            centroids.push_back(X[index]);
            centroidIndices.push_back(index);
        }


        for (int iteration = 0; iteration < maxIterations; ++iteration) {
            std::vector<int> clusters(n);
            // Присваивание каждой точки ближайшему центроиду
            for (int i = 0; i < n; ++i) {
                clusters[i] = findClosestCentroid(X[i], centroids);
            }

            // Обновление центроидов
            std::vector<std::vector<int>> newCentroids = updateCentroids(X, clusters, k, dimensions);

            // Проверка на сходимость (если центроиды не изменились)
            bool converged = true;
            for (int i = 0; i < k; ++i) {
                if (centroids[i] != newCentroids[i]) {
                    converged = false;
                    break;
                }
            }

            centroids = newCentroids;
            if (converged) break;
        }

        std::ofstream file3("data3.txt");
        // Вывод результатов (можно модифицировать для других выводов)
        std::cout << "Final Centroids:\n";
        for (const auto& centroid : centroids) {
            for (int val : centroid) {
                std::cout << val << " ";
                file3 << val << " ";
            }
            std::cout << "\n";
        }
        //std::cout << "Cluster assignments:\n";
        //for (int i = 0; i < y.size(); ++i) {
        //    std::cout << "Point " << i << ": Cluster " << y[i] << " (Actual class: " << y[i] << ")" << std::endl;
        //}
    }
  
};

class Perceptron {
private:
    vector<double> weights;
    double bias;

public:
    Perceptron(int num_feat) : weights(num_feat, 0.0), bias(0.0) {}

    int predict(const vector<int>& feat) {
        double sum = bias;
        for (size_t i = 0; i < feat.size(); ++i) {
            sum += weights[i] * feat[i];
        }
        return (sum >= 0) ? 1 : -1;
    }

    void train(const vector<vector<int>>& X, const vector<int>& y, double LR = 0.1, int max_iters = 100) {
        int num_feat = X[0].size();
        for (int iter = 0; iter < max_iters; ++iter) {
            bool updated = false;
            for (size_t i = 0; i < X.size(); ++i) {
                int predskaz = predict(X[i]);
                if (predskaz != y[i]) {
                    for (int j = 0; j < num_feat; ++j) {
                        weights[j] += LR * y[i] * X[i][j];
                    }
                    bias += LR * y[i];
                    updated = true;
                }
            }
            if (!updated) break; // Если ни один пример не был ошибочно классифицирован, то обучение завершено.
        }
        cout << "\tЧисло эпох: " << max_iters << endl;
    }

    void save_all_data(const vector<vector<int>>& X, const vector<int>& y) {
        for (size_t i = 0; i < X.size(); ++i) {
            cout << "Тчк " << i + 1 << ": ";
            cout << "Пред: " << predict(X[i]) << ", ";
            cout << "Факт: " << y[i] << endl;
        }

        ofstream outfile("data_perceptron.txt");
        if (outfile.is_open()) {
            for (size_t i = 0; i < X.size(); ++i) {
                for (int j = 0; j < X[i].size(); ++j) {
                    outfile << X[i][j] << " ";
                }
                outfile << y[i] << endl;
            }
            outfile.close();
        }
        else {
            cerr << "Ошибка открытия файла для записи данных." << endl;
        }
    }
};
