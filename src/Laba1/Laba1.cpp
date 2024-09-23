#include <iostream>
#include <fstream>

void J_m_en_fous() {

}
int main() {
    setlocale(LC_ALL, "RU");
    srand((time(NULL)));

    std::ifstream file("config.txt");

    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла! Создайте файл config.txt" << std::endl;
        return 1;
    }

    int N; // количество классов
    file >> N; // читаем с файла первую строку

    int* arr = new int[N]; // массив классов

    int sum = 0; // сумма чисел элементов класса

    for (int i = 0; i < N; i++) { 
    // считываем поэлементно строку с количеством элементов в каждом классе
        file >> arr[i];
        sum += arr[i]; // наращиваем сумму
    }
    int dimensions; // число измерений
    file >> dimensions; // читаем третью строку файла

    sum *= dimensions;

    int* arr2 = new int[sum];

    std::ofstream file2("data.txt");
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
    file.close();
    
    std::cout << "\nОбщее количество элементов: " << sum << std::endl;

    delete[] arr;
    delete[] arr2;
    return 0;
}
