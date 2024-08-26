#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

const int Num_Denominations = 6;
const int Denominatios[] = { 100, 200, 500, 1000, 2000, 5000 };
const int maxNotes = 1000;

void refill(int notes[], int maxNotes) {
    int totalNotes = 0;
    for (int i = 0; i < Num_Denominations; ++i) {
        totalNotes += notes[i];
    }

    int currentNotes = maxNotes - totalNotes;

    while (currentNotes > 0) {
        int denominationIndex = std::rand() % Num_Denominations;
        notes[denominationIndex]++;
        totalNotes++;
        currentNotes--;
    }
}

void giveMyMoney(int notes[], int& summ) {
    for (int i = Num_Denominations - 1; i >= 0; --i) {
        while (Denominatios[i] <= summ && notes[i] > 0) {
            notes[i]--;
            summ -= Denominatios[i];
        }
    }
    if (summ > 0) {
        std::cout << "Not enough founds or equal nominal for complete operation" << std::endl;
    }
    else {
        std::cout << "Operation complete!" << std::endl;
    }
}

void printNotes(const int notes[]) {
    int totalAmount = 0;
    int totalNotes = 0;
    std::cout << "State of the bankomat: " << std::endl;
    for (int i = 0; i < Num_Denominations; i++) {
        int count = notes[i];
        if (count > 0) {
            std::cout << Denominatios[i] << " rub.: " << count << std::endl;
            totalAmount += Denominatios[i] * count;
            totalNotes += count;
        }
    }
    std::cout << "Total amount: " << totalAmount << " rub." << std::endl;
    std::cout << "Total notes: " << totalNotes << std::endl;
}

int main()
{
    int notes[Num_Denominations] = { 0 };
    std::string answer;
    std::cout << "Welcome to abobusBank!Please do the operation" << std::endl;
    std::ifstream bankomat("C:\\tutorial-data\\bankomat\\bank.bin", std::ios::binary);

    if (bankomat.is_open()) {
        bankomat.read(reinterpret_cast<char*>(notes), sizeof(notes));
        std::cout << "Bankomat data loaded from file" << std::endl;
        bankomat.close();
    }
    else {
        std::cout << "No prev. data found in bank.Started with empty bankomat" << std::endl;
    }

    printNotes(notes);


    while (answer != "q") {
        std::cout << "Please input operation you wonna do(+ to refill money, - to get money or q to exit)" << std::endl;
        std::cin >> answer;
        if (answer == "+") {
            refill(notes, maxNotes);
            std::ofstream refill_bankomat("C:\\tutorial-data\\bankomat\\bank.bin", std::ios::binary);
            if (refill_bankomat.is_open()) {
                refill_bankomat.write(reinterpret_cast<const char*>(notes), sizeof(notes));
                refill_bankomat.close();
                std::cout << "Bankomat refilled and data saved to file" << std::endl;
            }
            else {
                std::cerr << "Error: Could not open file for writing." << std::endl;
            }
            printNotes(notes);
        }
        else if (answer == "-") {
            int summ = 0;
            std::cout << "Please input a summ you wonna take: " << std::endl;
            std::cin >> summ;

            giveMyMoney(notes, summ);

            if (summ < 0) {
                std::cerr << "Error: summ cannot be less then 0" << std::endl;
            }
            std::ofstream withdraw_bankomat("C:\\tutorial-data\\bankomat\\bank.bin", std::ios::binary);
            if (withdraw_bankomat.is_open()) {
                withdraw_bankomat.write(reinterpret_cast<const char*>(notes), sizeof(notes));
                withdraw_bankomat.close();
            }

        }
        else {
            std::cout << "Invalid operation(+ to refill money, - to get money, q to quit)" << std::endl;
        }
    }

    printNotes(notes);

    return  0;
}

