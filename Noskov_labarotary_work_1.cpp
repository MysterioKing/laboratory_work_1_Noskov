#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;

struct Pipe {
    string name;
    double length;
    int diameter;
    bool repair;
};

struct CompressorStation {
    string name;
    int workshopCount;
    int workshopsInOperation;
    int stationClass;
};

template <typename T>
bool readNumber(const string& prompt, T minValue, T maxValue, T& value) {
    string line;

    while (true) {
        cout << prompt;
        if (!getline(cin, line)) return false;

        istringstream input(line);
        char extra;

        if ((input >> value) && !(input >> extra) &&
            isfinite(value) && value >= minValue && value <= maxValue)
            return true;

        cout << "Wrong value. Try again.\n";
    }
}

bool readName(const string& prompt, string& name) {
    while (true) {
        cout << prompt;
        if (!getline(cin, name)) return false;
        if (name.find_first_not_of(" \t") != string::npos) return true;
        cout << "Name cannot be empty.\n";
    }
}

bool inputPipe(Pipe& pipe) {
    Pipe p;
    int repair;

    if (!readName("\nEnter pipe name: ", p.name)) return false;

    do {
        if (!readNumber("Enter pipe length (km): ", 0.0,
            numeric_limits<double>::max(), p.length))
            return false;
    } while (p.length == 0);

    if (!readNumber("Enter pipe diameter (mm): ",
        1, numeric_limits<int>::max(), p.diameter) ||
        !readNumber("Is pipe under repair? (1-yes/0-no): ",
            0, 1, repair))
        return false;

    p.repair = (repair == 1);
    pipe = p;
    return true;
}

bool inputStation(CompressorStation& station) {
    CompressorStation s;

    if (!readName("\nEnter station name: ", s.name) ||
        !readNumber("Enter number of workshops: ",
            0, numeric_limits<int>::max(), s.workshopCount) ||
        !readNumber("Enter number of workshops in operation: ",
            0, s.workshopCount, s.workshopsInOperation) ||
        !readNumber("Enter station class: ",
            0, numeric_limits<int>::max(), s.stationClass))
        return false;

    station = s;
    return true;
}

void printPipe(const Pipe& pipe) {
    cout << "\n--- Pipe ---\n"
        << "Name: " << pipe.name << '\n'
        << "Length: " << pipe.length << " km\n"
        << "Diameter: " << pipe.diameter << " mm\n"
        << "Repair: " << (pipe.repair ? "Yes" : "No") << '\n';
}

void printCompressorStation(const CompressorStation& station) {
    cout << "\n--- Compressor Station ---\n"
        << "Name: " << station.name << '\n'
        << "Workshops: " << station.workshopCount << '\n'
        << "Workshops in operation: " << station.workshopsInOperation << '\n'
        << "Station class: " << station.stationClass << '\n';
}

void editPipe(Pipe& pipe) {
    pipe.repair = !pipe.repair;
    cout << "\nPipe repair status changed.\n";
}

bool editStation(CompressorStation& station) {
    int choice;

    if (!readNumber("\n1. Start workshop\n2. Stop workshop\nEnter choice: ",
        1, 2, choice))
        return false;

    if (choice == 1) {
        if (station.workshopsInOperation < station.workshopCount) {
            ++station.workshopsInOperation;
            cout << "Workshop started.\n";
        }
        else {
            cout << "All workshops are already working.\n";
        }
    }
    else {
        if (station.workshopsInOperation > 0) {
            --station.workshopsInOperation;
            cout << "Workshop stopped.\n";
        }
        else {
            cout << "No working workshops.\n";
        }
    }

    return true;
}

bool saveData(const Pipe& pipe, const CompressorStation& station) {
    ofstream file("data.txt");
    if (!file) return false;

    file.precision(numeric_limits<double>::max_digits10);
    file << pipe.name << '\n'
        << pipe.length << '\n'
        << pipe.diameter << '\n'
        << pipe.repair << '\n'
        << station.name << '\n'
        << station.workshopCount << '\n'
        << station.workshopsInOperation << '\n'
        << station.stationClass << '\n';

    file.close();
    return static_cast<bool>(file);
}

bool loadData(Pipe& pipe, CompressorStation& station) {
    ifstream file("data.txt");
    if (!file) return false;

    Pipe p;
    CompressorStation s;
    int repair;

    if (!getline(file, p.name) ||
        !(file >> p.length >> p.diameter >> repair))
        return false;

    file.ignore(numeric_limits<streamsize>::max(), '\n');

    if (!getline(file, s.name) ||
        !(file >> s.workshopCount >> s.workshopsInOperation >> s.stationClass))
        return false;

    if (p.name.find_first_not_of(" \t") == string::npos ||
        !isfinite(p.length) || p.length <= 0 || p.diameter <= 0 ||
        (repair != 0 && repair != 1) ||
        s.name.find_first_not_of(" \t") == string::npos ||
        s.workshopCount < 0 ||
        s.workshopsInOperation < 0 ||
        s.workshopsInOperation > s.workshopCount ||
        s.stationClass < 0)
        return false;

    p.repair = (repair == 1);
    pipe = p;
    station = s;
    return true;
}

int main() {
    Pipe pipe;
    CompressorStation station;
    bool pipeExists = false;
    bool stationExists = false;

    while (true) {
        cout << "\n1. Add pipe\n"
            << "2. Add compressor station\n"
            << "3. View all objects\n"
            << "4. Edit pipe status\n"
            << "5. Edit compressor station\n"
            << "6. Save\n"
            << "7. Load\n"
            << "0. Exit\n";

        int command;
        if (!readNumber("\nEnter command: ", 0, 7, command)) return 0;

        switch (command) {
        case 0:
            return 0;

        case 1:
            if (!inputPipe(pipe)) return 0;
            pipeExists = true;
            break;

        case 2:
            if (!inputStation(station)) return 0;
            stationExists = true;
            break;

        case 3:
            if (pipeExists) printPipe(pipe);
            else cout << "\nPipe not added.\n";

            if (stationExists) printCompressorStation(station);
            else cout << "\nStation not added.\n";
            break;

        case 4:
            if (pipeExists) editPipe(pipe);
            else cout << "\nPipe not added.\n";
            break;

        case 5:
            if (stationExists) {
                if (!editStation(station)) return 0;
            }
            else {
                cout << "\nStation not added.\n";
            }
            break;

        case 6:
            if (!pipeExists || !stationExists)
                cout << "\nAdd pipe and station first.\n";
            else
                cout << (saveData(pipe, station)
                    ? "\nData saved.\n" : "\nCould not save data.\n");
            break;

        case 7:
            if (loadData(pipe, station)) {
                pipeExists = stationExists = true;
                cout << "\nData loaded.\n";
            }
            else {
                cout << "\nFile not found or contains invalid data.\n";
            }
            break;
        }
    }
}