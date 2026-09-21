#include <iostream>
#include <fstream>
#include <string>

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

bool readInt(const string& text, int min, int max, int& number) {
    string rest;

    while (true) {
        cout << text;

        if (cin >> number) {
            getline(cin, rest);

            if (rest.find_first_not_of(" \t\r") == string::npos &&
                number >= min && number <= max)
                return true;
        }
        else {
            if (cin.eof()) return false;
            cin.clear();
            getline(cin, rest);
        }

        cout << "Wrong number. Try again.\n";
    }
}

bool readLength(double& length) {
    string rest;

    while (true) {
        cout << "Enter pipe length (km): ";

        if (cin >> length) {
            getline(cin, rest);

            if (rest.find_first_not_of(" \t\r") == string::npos &&
                length > 0 && length < 1e100)
                return true;
        }
        else {
            if (cin.eof()) return false;
            cin.clear();
            getline(cin, rest);
        }

        cout << "Length must be a positive number.\n";
    }
}

bool readName(const string& text, string& name) {
    cout << text;
    return static_cast<bool>(getline(cin >> ws, name));
}

bool inputPipe(Pipe& pipe) {
    Pipe newPipe;
    int repair;

    if (!readName("\nEnter pipe name: ", newPipe.name) ||
        !readLength(newPipe.length) ||
        !readInt("Enter diameter (mm): ", 1, 1000000, newPipe.diameter) ||
        !readInt("Under repair? (0-no, 1-yes): ", 0, 1, repair))
        return false;

    newPipe.repair = (repair == 1);
    pipe = newPipe;
    return true;
}

bool inputStation(CompressorStation& station) {
    CompressorStation newStation;

    if (!readName("\nEnter station name: ", newStation.name) ||
        !readInt("Number of workshops: ",
            0, 1000000, newStation.workshopCount) ||
        !readInt("Working workshops: ",
            0, newStation.workshopCount,
            newStation.workshopsInOperation) ||
        !readInt("Station class: ",
            0, 1000000, newStation.stationClass))
        return false;

    station = newStation;
    return true;
}

void printPipe(const Pipe& pipe) {
    cout << "\n--- Pipe ---\n";
    cout << "Name: " << pipe.name << '\n';
    cout << "Length: " << pipe.length << " km\n";
    cout << "Diameter: " << pipe.diameter << " mm\n";
    cout << "Under repair: " << (pipe.repair ? "Yes" : "No") << '\n';
}

void printStation(const CompressorStation& station) {
    cout << "\n--- Compressor station ---\n";
    cout << "Name: " << station.name << '\n';
    cout << "Workshops: " << station.workshopCount << '\n';
    cout << "Working workshops: " << station.workshopsInOperation << '\n';
    cout << "Station class: " << station.stationClass << '\n';
}

void editPipe(Pipe& pipe) {
    pipe.repair = !pipe.repair;
    cout << "Repair status changed.\n";
}

bool editStation(CompressorStation& station) {
    int choice;

    if (!readInt("\n1. Start workshop\n2. Stop workshop\nChoice: ",
        1, 2, choice))
        return false;

    if (choice == 1) {
        if (station.workshopsInOperation < station.workshopCount) {
            station.workshopsInOperation++;
            cout << "Workshop started.\n";
        }
        else {
            cout << "All workshops are already working.\n";
        }
    }
    else {
        if (station.workshopsInOperation > 0) {
            station.workshopsInOperation--;
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

    file << pipe.name << '\n'
        << pipe.length << '\n'
        << pipe.diameter << '\n'
        << pipe.repair << '\n'
        << station.name << '\n'
        << station.workshopCount << '\n'
        << station.workshopsInOperation << '\n'
        << station.stationClass << '\n';

    file.close();
    return !file.fail();
}

bool loadData(Pipe& pipe, CompressorStation& station) {
    ifstream file("data.txt");
    if (!file) return false;

    Pipe newPipe;
    CompressorStation newStation;
    int repair;

    if (!getline(file, newPipe.name) ||
        !(file >> newPipe.length >> newPipe.diameter >> repair))
        return false;

    file.ignore(10000, '\n');

    if (!getline(file, newStation.name) ||
        !(file >> newStation.workshopCount
            >> newStation.workshopsInOperation
            >> newStation.stationClass))
        return false;

    if (newPipe.name.empty() ||
        newPipe.length <= 0 || newPipe.length >= 1e100 ||
        newPipe.diameter <= 0 ||
        (repair != 0 && repair != 1) ||
        newStation.name.empty() ||
        newStation.workshopCount < 0 ||
        newStation.workshopsInOperation < 0 ||
        newStation.workshopsInOperation > newStation.workshopCount ||
        newStation.stationClass < 0)
        return false;

    newPipe.repair = (repair == 1);
    pipe = newPipe;
    station = newStation;
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
            << "4. Edit pipe\n"
            << "5. Edit compressor station\n"
            << "6. Save\n"
            << "7. Load\n"
            << "0. Exit\n";

        int command;
        if (!readInt("\nCommand: ", 0, 7, command)) return 0;

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

            if (stationExists) printStation(station);
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
                    ? "\nData saved.\n" : "Save error.\n");
            break;

        case 7:
            if (loadData(pipe, station)) {
                pipeExists = true;
                stationExists = true;
                cout << "\nData loaded.\n";
            }
            else {
                cout << "\nFile not found or invalid.\n";
            }
            break;
        }
    }
}