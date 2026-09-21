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
    int workingWorkshops;
    int stationClass;
};

struct Data {
    Pipe pipe;
    CompressorStation station;
    bool pipeExists;
    bool stationExists;
};

int readInt(int minValue, int maxValue) {
    int number;

    while (true) {
        cin >> number;

        if (cin && number >= minValue && number <= maxValue)
            return number;

        if (cin.eof())
            return -1;

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Wrong number. Try again: ";
    }
}

double readLength() {
    double length;

    while (true) {
        cin >> length;

        if (cin && length > 0 && length < 1e100)
            return length;

        if (cin.eof())
            return -1;

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Wrong length. Try again: ";
    }
}

Pipe inputPipe() {
    Pipe pipe = {};

    cout << "\nPipe name: ";
    getline(cin >> ws, pipe.name);

    cout << "Length (km): ";
    pipe.length = readLength();

    cout << "Diameter (mm): ";
    pipe.diameter = readInt(1, 1000000);

    cout << "Under repair? (0-no, 1-yes): ";
    pipe.repair = (readInt(0, 1) == 1);

    return pipe;
}

CompressorStation inputStation() {
    CompressorStation station = {};

    cout << "\nStation name: ";
    getline(cin >> ws, station.name);

    cout << "Number of workshops: ";
    station.workshopCount = readInt(0, 1000000);

    cout << "Working workshops: ";
    station.workingWorkshops = readInt(0, station.workshopCount);

    cout << "Station class: ";
    station.stationClass = readInt(0, 1000000);

    return station;
}

void printPipe(Pipe pipe) {
    cout << "\nPipe: " << pipe.name << '\n';
    cout << "Length: " << pipe.length << " km\n";
    cout << "Diameter: " << pipe.diameter << " mm\n";
    cout << "Under repair: " << (pipe.repair ? "Yes" : "No") << '\n';
}

void printStation(CompressorStation station) {
    cout << "\nStation: " << station.name << '\n';
    cout << "Workshops: " << station.workshopCount << '\n';
    cout << "Working workshops: " << station.workingWorkshops << '\n';
    cout << "Class: " << station.stationClass << '\n';
}

Pipe editPipe(Pipe pipe) {
    pipe.repair = !pipe.repair;
    return pipe;
}

CompressorStation editStation(CompressorStation station) {
    cout << "1. Start workshop\n";
    cout << "2. Stop workshop\n";
    cout << "Choice: ";

    int choice = readInt(1, 2);

    if (choice == 1) {
        if (station.workingWorkshops < station.workshopCount)
            station.workingWorkshops++;
        else
            cout << "All workshops are working.\n";
    }
    else if (choice == 2) {
        if (station.workingWorkshops > 0)
            station.workingWorkshops--;
        else
            cout << "No working workshops.\n";
    }

    return station;
}

void savePipe(ofstream& file, Pipe pipe) {
    file << pipe.name << '\n';
    file << pipe.length << '\n';
    file << pipe.diameter << '\n';
    file << pipe.repair << '\n';
}

void saveStation(ofstream& file, CompressorStation station) {
    file << station.name << '\n';
    file << station.workshopCount << '\n';
    file << station.workingWorkshops << '\n';
    file << station.stationClass << '\n';
}

bool loadPipe(ifstream& file, Pipe& pipe) {
    int repair;

    if (!getline(file >> ws, pipe.name) ||
        !(file >> pipe.length >> pipe.diameter >> repair))
        return false;

    if (pipe.length <= 0 || pipe.length >= 1e100 ||
        pipe.diameter <= 0 || (repair != 0 && repair != 1))
        return false;

    pipe.repair = (repair == 1);
    return true;
}

bool loadStation(ifstream& file, CompressorStation& station) {
    if (!getline(file >> ws, station.name) ||
        !(file >> station.workshopCount
            >> station.workingWorkshops >> station.stationClass))
        return false;

    if (station.workshopCount < 0 ||
        station.workingWorkshops < 0 ||
        station.workingWorkshops > station.workshopCount ||
        station.stationClass < 0)
        return false;

    return true;
}

bool saveData(Data data) {
    ofstream file("data.txt");
    if (!file) return false;

    file << data.pipeExists << ' ' << data.stationExists << '\n';

    if (data.pipeExists)
        savePipe(file, data.pipe);

    if (data.stationExists)
        saveStation(file, data.station);

    file.close();
    return !file.fail();
}

bool loadData(Data& data) {
    ifstream file("data.txt");
    if (!file) return false;

    Data loaded = {};
    int hasPipe, hasStation;

    if (!(file >> hasPipe >> hasStation))
        return false;

    if ((hasPipe != 0 && hasPipe != 1) ||
        (hasStation != 0 && hasStation != 1))
        return false;

    loaded.pipeExists = (hasPipe == 1);
    loaded.stationExists = (hasStation == 1);

    if (loaded.pipeExists && !loadPipe(file, loaded.pipe))
        return false;

    if (loaded.stationExists && !loadStation(file, loaded.station))
        return false;

    data = loaded;
    return true;
}

int main() {
    Data data = {};

    while (true) {
        cout << "\n1. Add pipe\n";
        cout << "2. Add compressor station\n";
        cout << "3. View all objects\n";
        cout << "4. Edit pipe\n";
        cout << "5. Edit station\n";
        cout << "6. Save\n";
        cout << "7. Load\n";
        cout << "0. Exit\n";
        cout << "\nChoice: ";

        int command;

        if (!(cin >> command)) {
            if (cin.eof()) return 0;
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\nEnter a number from 0 to 7.\n";
            continue;
        }

        switch (command) {
        case 0:
            return 0;

        case 1:
            data.pipe = inputPipe();
            if (cin.eof()) return 0;
            data.pipeExists = true;
            break;

        case 2:
            data.station = inputStation();
            if (cin.eof()) return 0;
            data.stationExists = true;
            break;

        case 3:
            if (data.pipeExists) printPipe(data.pipe);
            else cout << "\nPipe not added.\n";

            if (data.stationExists) printStation(data.station);
            else cout << "\nStation not added.\n";
            break;

        case 4:
            if (data.pipeExists) data.pipe = editPipe(data.pipe);
            else cout << "\nPipe not added.\n";
            break;

        case 5:
            if (data.stationExists)
                data.station = editStation(data.station);
            else
                cout << "\nStation not added.\n";
            if (cin.eof()) return 0;
            break;

        case 6:
            if (!data.pipeExists && !data.stationExists)
                cout << "\nAdd a pipe or station first.\n";
            else if (saveData(data))
                cout << "\nData saved.\n";
            else
                cout << "\nSave error.\n";
            break;

        case 7:
            if (loadData(data))
                cout << "\nData loaded.\n";
            else
                cout << "\nFile not found or invalid.\n";
            break;

        default:
            cout << "\nEnter a number from 0 to 7.\n";
            cin.ignore(10000, '\n');
            break;
        }
    }
}