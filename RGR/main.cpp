#include "controller.h"
#include <locale>

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    Manager dbController;
    dbController.Run();
    return 0;
}
