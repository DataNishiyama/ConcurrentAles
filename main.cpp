#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

std::atomic<int> totalAlesDrunkGroup1(0);
std::atomic<int> totalAlesDrunkGroup2(0);
std::mutex tavernMutex;

void drinkAle(int id, int& totalAlesDrunk, std::string groupName) {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(tavernMutex);
            if (totalAlesDrunk > 0) {
                std::cout << "Dwarf " << id << " from Group " << groupName << " drinks ale... ";
                totalAlesDrunk--;
                std::cout << "Total ales drunk by Group " << groupName << ": " << totalAlesDrunk << "\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } else {
                std::cout << "Dwarf " << id << " found no ale left!\n";
                break;
            }
        }

        //Thread Delay
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

//Refill Ales
void refillAle(int& totalAlesDrunk, std::string groupName) {
    std::lock_guard<std::mutex> lock(tavernMutex);
    totalAlesDrunk = 5;
    std::cout << "Group " << groupName << " refills ale...\n";
}

//Main
int main() {
    int totalAlesGroup1 = 5;
    int totalAlesGroup2 = 5;

    std::thread dwarves[10];

    //Threads Group 1
    for (int i = 0; i < 5; ++i) {
        dwarves[i] = std::thread(drinkAle, i + 1, std::ref(totalAlesGroup1), "1");
    }

    //Threads Group 2
    for (int i = 5; i < 10; ++i) {
        dwarves[i] = std::thread(drinkAle, i + 1, std::ref(totalAlesGroup2), "2");
    }

    //Join all
    for (int i = 0; i < 10; ++i) {
        dwarves[i].join();
    }

    //Refill Group 1
    refillAle(totalAlesGroup1, "1");

    //Refill Group 2
    refillAle(totalAlesGroup2, "2");

    return 0;
}
