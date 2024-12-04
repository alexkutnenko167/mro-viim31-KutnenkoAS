#include "L'addition.cpp"

int main() {
    setlocale(LC_ALL, "RU");
    srand((time(NULL)));

    O obj;
    obj.GenArray();

    std::vector<std::vector<int>> X;
    std::vector<int> y;
    obj.read_data(X, y);
    obj.ho_kashyap_algorithm(X, y);
    obj.Metrics();

    obj.k_means_algorithm(X, y, 2, 100);

    Perceptron perceptron(X[0].size());
    perceptron.train(X, y);
    perceptron.save_all_data(X, y);

    return 0;
}