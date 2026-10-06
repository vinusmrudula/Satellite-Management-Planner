#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;


// ============================================================
// HELPER FUNCTIONS
// ============================================================

string trim(string value)
{
    while (!value.empty() && isspace(value.front()))
        value.erase(value.begin());

    while (!value.empty() && isspace(value.back()))
        value.pop_back();

    return value;
}


string cleanValue(string value)
{
    value = trim(value);

    if (value.size() >= 2 &&
        value.front() == '"' &&
        value.back() == '"')
    {
        value = value.substr(1, value.size() - 2);
    }

    return trim(value);
}


// ------------------------------------------------------------
// CSV LINE READER
// Handles commas inside quoted text
// ------------------------------------------------------------

vector<string> parseCSVLine(string line)
{
    vector<string> result;

    // Remove Windows newline
    if (!line.empty() && line.back() == '\r')
    {
        line.pop_back();
    }


    // --------------------------------------------------------
    // YOUR CSV HAS THE WHOLE ROW SURROUNDED BY QUOTES
    // --------------------------------------------------------

    if (
        line.size() >= 2 &&
        line.front() == '"' &&
        line.back() == '"'
    )
    {
        line = line.substr(
            1,
            line.size() - 2
        );
    }


    // --------------------------------------------------------
    // Convert escaped CSV quotes:
    //
    // ""550,94""
    //
    // becomes:
    //
    // "550,94"
    // --------------------------------------------------------

    string fixedLine;

    for (size_t i = 0; i < line.length(); i++)
    {
        if (
            line[i] == '"' &&
            i + 1 < line.length() &&
            line[i + 1] == '"'
        )
        {
            fixedLine += '"';
            i++;
        }
        else
        {
            fixedLine += line[i];
        }
    }


    // --------------------------------------------------------
    // NORMAL CSV PARSING
    // --------------------------------------------------------

    string current;

    bool insideQuotes = false;


    for (size_t i = 0;
         i < fixedLine.length();
         i++)
    {
        char c = fixedLine[i];


        // Toggle quote state
        if (c == '"')
        {
            insideQuotes = !insideQuotes;
        }


        // Comma outside quotes = next column
        else if (
            c == ',' &&
            !insideQuotes
        )
        {
            result.push_back(
                cleanValue(current)
            );

            current.clear();
        }


        // Normal character
        else
        {
            current += c;
        }
    }


    // Add final column
    result.push_back(
        cleanValue(current)
    );


    return result;
}


// ------------------------------------------------------------
// Convert string to double
// ------------------------------------------------------------

double toDouble(string value)
{
    value = cleanValue(value);

    try
    {
        return stod(value);
    }
    catch (...)
    {
        return 0;
    }
}


// ------------------------------------------------------------
// Get value from CSV using possible column names
// ------------------------------------------------------------

string getColumn(
    const vector<string>& row,
    const map<string, int>& columns,
    initializer_list<string> possibleNames
)
{
    for (string name : possibleNames)
    {
        auto it = columns.find(name);

        if (it != columns.end())
        {
            int index = it->second;

            if (index >= 0 &&
                index < (int)row.size())
            {
                return cleanValue(row[index]);
            }
        }
    }

    return "";
}


// ------------------------------------------------------------
// Convert header to standard form
// ------------------------------------------------------------

string normalizeHeader(string header)
{
    header = cleanValue(header);

    transform(
        header.begin(),
        header.end(),
        header.begin(),
        ::tolower
    );

    // Remove spaces
    header.erase(
        remove(
            header.begin(),
            header.end(),
            ' '
        ),
        header.end()
    );

    return header;
}


// ============================================================
// 1. ABSTRACT BASE CLASS - SATELLITE
// ============================================================

class Satellite
{
protected:

    string id;
    string name;
    string type;

    double battery;

    string description;
    string orbit;
    string launchDate;
    string operatorName;
    string missionPurpose;

    static int totalSatellites;

public:

    // --------------------------------------------------------
    // DEFAULT CONSTRUCTOR
    // --------------------------------------------------------

    Satellite()
    {
        id = "UNKNOWN";
        name = "UNKNOWN";
        type = "UNKNOWN";

        battery = 100;

        description = "No description available.";
        orbit = "UNKNOWN";
        launchDate = "UNKNOWN";
        operatorName = "UNKNOWN";
        missionPurpose = "UNKNOWN";

        totalSatellites++;
    }


    // --------------------------------------------------------
    // PARAMETERIZED CONSTRUCTOR
    // --------------------------------------------------------

    Satellite(
        string id,
        string name,
        string type,
        double battery,
        string description,
        string orbit,
        string launchDate,
        string operatorName,
        string missionPurpose
    )
    {
        this->id = id;
        this->name = name;
        this->type = type;

        this->battery = battery;

        this->description = description;
        this->orbit = orbit;
        this->launchDate = launchDate;
        this->operatorName = operatorName;
        this->missionPurpose = missionPurpose;

        totalSatellites++;
    }


    // --------------------------------------------------------
    // COPY CONSTRUCTOR
    // --------------------------------------------------------

    Satellite(const Satellite& other)
    {
        id = other.id;
        name = other.name;
        type = other.type;

        battery = other.battery;

        description = other.description;
        orbit = other.orbit;
        launchDate = other.launchDate;
        operatorName = other.operatorName;
        missionPurpose = other.missionPurpose;

        totalSatellites++;
    }


    // --------------------------------------------------------
    // ABSTRACTION
    // --------------------------------------------------------

    virtual void performMission() = 0;

    virtual void displayDetails() = 0;


    // --------------------------------------------------------
    // FUNCTION OVERLOADING
    // --------------------------------------------------------

    virtual double calculateResourceUsage()
    {
        return 0;
    }


    virtual double calculateResourceUsage(int missionDays)
    {
        return missionDays * 2.5;
    }


    // --------------------------------------------------------
    // GETTERS
    // --------------------------------------------------------

    string getId() const
    {
        return id;
    }


    string getName() const
    {
        return name;
    }


    string getType() const
    {
        return type;
    }


    double getBattery() const
    {
        return battery;
    }


    string getDescription() const
    {
        return description;
    }


    string getOrbit() const
    {
        return orbit;
    }


    string getLaunchDate() const
    {
        return launchDate;
    }


    string getOperator() const
    {
        return operatorName;
    }


    string getMissionPurpose() const
    {
        return missionPurpose;
    }


    // --------------------------------------------------------
    // STATIC FUNCTION
    // --------------------------------------------------------

    static int getTotalSatellites()
    {
        return totalSatellites;
    }


    // --------------------------------------------------------
    // FRIEND FUNCTION
    // --------------------------------------------------------

    friend void compareSatellites(
        const Satellite& s1,
        const Satellite& s2
    );


    // --------------------------------------------------------
    // VIRTUAL DESTRUCTOR
    // --------------------------------------------------------

    virtual ~Satellite()
    {
        // No output here.
        // This keeps CSV/API output clean.
    }
};


int Satellite::totalSatellites = 0;


// ============================================================
// 2. EARTH OBSERVATION SATELLITE
// ============================================================

class EarthObservationSatellite : public Satellite
{
private:

    string sensorType;
    double resolution;

public:

    EarthObservationSatellite(
        string id,
        string name,
        double battery,
        string description,
        string orbit,
        string launchDate,
        string operatorName,
        string missionPurpose,
        string sensor,
        double resolution
    )
        : Satellite(
            id,
            name,
            "Earth Observation",
            battery,
            description,
            orbit,
            launchDate,
            operatorName,
            missionPurpose
        )
    {
        sensorType = sensor;
        this->resolution = resolution;
    }


    void performMission() override
    {
        cout << "\nEarth Observation Mission Started!";
        cout << "\nCapturing satellite imagery...";
        cout << "\nSensor: " << sensorType << endl;
    }


    void displayDetails() override
    {
        cout << "\n--- Satellite ---";

        cout << "\nID: " << id;
        cout << "\nName: " << name;
        cout << "\nType: " << type;
        cout << "\nBattery: " << battery << "%";
        cout << "\nOrbit: " << orbit;
        cout << "\nLaunch Date: " << launchDate;
        cout << "\nOperator: " << operatorName;
        cout << "\nSensor: " << sensorType;
        cout << "\nResolution: " << resolution << " m";
        cout << "\nMission Purpose: " << missionPurpose;
        cout << "\nDescription: " << description << "\n";
    }


    double calculateResourceUsage() override
    {
        return 8.5;
    }
};


// ============================================================
// 3. WEATHER SATELLITE
// ============================================================

class WeatherSatellite : public Satellite
{
private:

    double temperature;
    double humidity;

public:

    WeatherSatellite(
        string id,
        string name,
        double battery,
        string description,
        string orbit,
        string launchDate,
        string operatorName,
        string missionPurpose,
        double temperature,
        double humidity
    )
        : Satellite(
            id,
            name,
            "Weather",
            battery,
            description,
            orbit,
            launchDate,
            operatorName,
            missionPurpose
        )
    {
        this->temperature = temperature;
        this->humidity = humidity;
    }


    void performMission() override
    {
        cout << "\nWeather Monitoring Mission Started!";
        cout << "\nCollecting weather data...";
        cout << "\nTemperature: "
             << temperature << " C";
        cout << "\nHumidity: "
             << humidity << "%\n";
    }


    void displayDetails() override
    {
        cout << "\n--- Satellite ---";

        cout << "\nID: " << id;
        cout << "\nName: " << name;
        cout << "\nType: " << type;
        cout << "\nBattery: " << battery << "%";
        cout << "\nOrbit: " << orbit;
        cout << "\nLaunch Date: " << launchDate;
        cout << "\nOperator: " << operatorName;
        cout << "\nTemperature: "
             << temperature << " C";
        cout << "\nHumidity: "
             << humidity << "%";
        cout << "\nMission Purpose: "
             << missionPurpose;
        cout << "\nDescription: "
             << description << "\n";
    }
};


// ============================================================
// 4. COMMUNICATION SATELLITE
// ============================================================

class CommunicationSatellite : public Satellite
{
private:

    double bandwidth;

public:

    CommunicationSatellite(
        string id,
        string name,
        double battery,
        string description,
        string orbit,
        string launchDate,
        string operatorName,
        string missionPurpose,
        double bandwidth
    )
        : Satellite(
            id,
            name,
            "Communication",
            battery,
            description,
            orbit,
            launchDate,
            operatorName,
            missionPurpose
        )
    {
        this->bandwidth = bandwidth;
    }


    void performMission() override
    {
        cout << "\nCommunication Mission Started!";
        cout << "\nTransmitting data...";
        cout << "\nBandwidth: "
             << bandwidth << " Mbps\n";
    }


    void displayDetails() override
    {
        cout << "\n--- Satellite ---";

        cout << "\nID: " << id;
        cout << "\nName: " << name;
        cout << "\nType: " << type;
        cout << "\nBattery: " << battery << "%";
        cout << "\nOrbit: " << orbit;
        cout << "\nLaunch Date: " << launchDate;
        cout << "\nOperator: " << operatorName;
        cout << "\nBandwidth: "
             << bandwidth << " Mbps";
        cout << "\nMission Purpose: "
             << missionPurpose;
        cout << "\nDescription: "
             << description << "\n";
    }
};


// ============================================================
// 5. AI CAPABILITY
// ============================================================

class AIEnabled
{
protected:

    string modelName;

public:

    AIEnabled(string model)
    {
        modelName = model;
    }


    void analyzeData()
    {
        cout << "\nAI Model: "
             << modelName;

        cout << "\nAnalyzing satellite data...";

        cout << "\nAnomaly detection completed.\n";
    }
};


// ============================================================
// 6. SMART SATELLITE
// MULTIPLE INHERITANCE
// ============================================================

class SmartSatellite :
    public Satellite,
    public AIEnabled
{
public:

    SmartSatellite(
        string id,
        string name,
        double battery,
        string description,
        string orbit,
        string launchDate,
        string operatorName,
        string missionPurpose,
        string model
    )
        : Satellite(
            id,
            name,
            "Smart Satellite",
            battery,
            description,
            orbit,
            launchDate,
            operatorName,
            missionPurpose
        ),
        AIEnabled(model)
    {
    }


    void performMission() override
    {
        cout << "\nSmart Satellite Mission Started!";

        analyzeData();
    }


    void displayDetails() override
    {
        cout << "\n--- Satellite ---";

        cout << "\nID: " << id;
        cout << "\nName: " << name;
        cout << "\nType: " << type;
        cout << "\nBattery: " << battery << "%";
        cout << "\nOrbit: " << orbit;
        cout << "\nLaunch Date: " << launchDate;
        cout << "\nOperator: " << operatorName;
        cout << "\nAI Model: " << modelName;
        cout << "\nMission Purpose: "
             << missionPurpose;
        cout << "\nDescription: "
             << description << "\n";
    }
};


// ============================================================
// 7. MISSION STATUS
// ============================================================

enum MissionStatus
{
    PLANNED,
    UPCOMING,
    ACTIVE,
    COMPLETED
};


string statusToString(MissionStatus status)
{
    switch (status)
    {
        case PLANNED:
            return "PLANNED";

        case UPCOMING:
            return "UPCOMING";

        case ACTIVE:
            return "ACTIVE";

        case COMPLETED:
            return "COMPLETED";
    }

    return "UNKNOWN";
}


MissionStatus stringToStatus(string status)
{
    status = cleanValue(status);

    transform(
        status.begin(),
        status.end(),
        status.begin(),
        ::toupper
    );


    if (status == "PLANNED")
        return PLANNED;

    if (status == "UPCOMING")
        return UPCOMING;

    if (status == "ACTIVE")
        return ACTIVE;

    if (status == "COMPLETED")
        return COMPLETED;

    return PLANNED;
}


// ============================================================
// 8. ABSTRACT MISSION CLASS
// ============================================================

class Mission
{
protected:

    string missionId;
    string missionName;
    string targetLocation;
    string objective;

    MissionStatus status;

    string classifiedInformation;

    string description;
    string launchDate;
    string estimatedDuration;

public:

    static int totalMissions;


    Mission(
        string id,
        string name,
        string target,
        string objective,
        string classifiedInfo,
        string description,
        string launchDate,
        string estimatedDuration
    )
    {
        missionId = id;
        missionName = name;
        targetLocation = target;
        this->objective = objective;

        classifiedInformation = classifiedInfo;

        this->description = description;
        this->launchDate = launchDate;
        this->estimatedDuration = estimatedDuration;

        status = PLANNED;

        totalMissions++;
    }


    // --------------------------------------------------------
    // PURE VIRTUAL FUNCTION
    // --------------------------------------------------------

    virtual void startMission() = 0;


    // --------------------------------------------------------
    // PUBLIC INFORMATION
    // --------------------------------------------------------

    virtual void displayMission()
    {
        cout << "\nMission ID: "
             << missionId;

        cout << "\nMission Name: "
             << missionName;

        cout << "\nTarget Location: "
             << targetLocation;

        cout << "\nObjective: "
             << objective;

        cout << "\nStatus: "
             << statusToString(status);

        cout << "\nLaunch Date: "
             << launchDate;

        cout << "\nEstimated Duration: "
             << estimatedDuration;

        cout << "\nDescription: "
             << description << endl;
    }


    // --------------------------------------------------------
    // ADMIN ONLY INFORMATION
    // --------------------------------------------------------

    void displayPrivateInformation()
    {
        cout << "\n========== CLASSIFIED INFORMATION ==========";

        cout << "\nMission ID: "
             << missionId;

        cout << "\nMission Name: "
             << missionName;

        cout << "\nTarget Location: "
             << targetLocation;

        cout << "\nObjective: "
             << objective;

        cout << "\nClassified Details: "
             << classifiedInformation;

        cout << "\nCurrent Status: "
             << statusToString(status);

        cout << "\n=============================================\n";
    }


    // --------------------------------------------------------
    // GETTERS
    // --------------------------------------------------------

    string getMissionId()
    {
        return missionId;
    }


    string getMissionName()
    {
        return missionName;
    }


    string getTargetLocation()
    {
        return targetLocation;
    }


    string getObjective()
    {
        return objective;
    }


    string getDescription()
    {
        return description;
    }


    string getLaunchDate()
    {
        return launchDate;
    }


    string getEstimatedDuration()
    {
        return estimatedDuration;
    }


    MissionStatus getStatus()
    {
        return status;
    }


    void setStatus(MissionStatus newStatus)
    {
        status = newStatus;
    }


    // --------------------------------------------------------
    // FRIEND FUNCTION
    // --------------------------------------------------------

    friend void compareMissions(
        const Mission& m1,
        const Mission& m2
    );


    virtual ~Mission()
    {
    }
};


int Mission::totalMissions = 0;


// ============================================================
// 9. OBSERVATION MISSION
// ============================================================

class ObservationMission : public Mission
{
private:

    string sensorType;

public:

    ObservationMission(
        string id,
        string name,
        string target,
        string objective,
        string sensor,
        string classifiedInfo,
        string description,
        string launchDate,
        string estimatedDuration
    )
        : Mission(
            id,
            name,
            target,
            objective,
            classifiedInfo,
            description,
            launchDate,
            estimatedDuration
        )
    {
        sensorType = sensor;
    }


    void startMission() override
    {
        status = ACTIVE;

        cout << "\nObservation Mission Started!";

        cout << "\nTarget: "
             << targetLocation;

        cout << "\nSensor: "
             << sensorType << endl;
    }


    void displayMission() override
    {
        Mission::displayMission();

        cout << "Sensor Type: "
             << sensorType << endl;
    }
};


// ============================================================
// 10. RESOURCE CLASS
// ============================================================

class Resource
{
private:

    string resourceId;
    string resourceName;
    int quantity;

public:

    Resource(
        string id,
        string name,
        int quantity
    )
    {
        resourceId = id;
        resourceName = name;
        this->quantity = quantity;
    }


    void displayResource()
    {
        cout << resourceName
             << " : "
             << quantity
             << endl;
    }
};


// ============================================================
// 11. ADMIN CLASS
// ============================================================

class Admin
{
private:

    string username;
    string password;

public:

    Admin(
        string user,
        string pass
    )
    {
        username = user;
        password = pass;
    }


    bool login()
    {
        string enteredUsername;
        string enteredPassword;


        cout << "\n========== ADMIN LOGIN ==========\n";


        cout << "Enter username: ";
        cin >> enteredUsername;


        cout << "Enter password: ";
        cin >> enteredPassword;


        if (
            enteredUsername == username &&
            enteredPassword == password
        )
        {
            cout << "\nLogin successful!\n";

            return true;
        }


        cout << "\nIncorrect username or password!\n";

        return false;
    }
};


// ============================================================
// 12. SATELLITE CSV LOADER
// ============================================================

class SatelliteCSVLoader
{
public:

    static vector<Satellite*> load(
        string filename
    )
    {
        vector<Satellite*> satellites;


        ifstream file(filename);


        if (!file.is_open())
        {
            cout << "\nERROR: Could not open "
                 << filename << endl;

            return satellites;
        }


        string line;


        // ----------------------------------------------------
        // READ HEADER
        // ----------------------------------------------------

        if (!getline(file, line))
        {
            return satellites;
        }


        vector<string> headers =
            parseCSVLine(line);


        map<string, int> columns;


        for (int i = 0;
             i < (int)headers.size();
             i++)
        {
            columns[
                normalizeHeader(headers[i])
            ] = i;
        }


        // ----------------------------------------------------
        // READ ALL SATELLITES
        // ----------------------------------------------------

        while (getline(file, line))
        {
            if (trim(line).empty())
                continue;


            vector<string> row =
                parseCSVLine(line);


            if (row.size() < 3)
                continue;


            string id =
                getColumn(
                    row,
                    columns,
                    {"id", "satelliteid"}
                );


            string name =
                getColumn(
                    row,
                    columns,
                    {"name", "satellitename"}
                );


            string type =
                getColumn(
                    row,
                    columns,
                    {"type", "satellitetype"}
                );


            // ------------------------------------------------
            // BATTERY
            // ------------------------------------------------

            string batteryText =
                getColumn(
                    row,
                    columns,
                    {"battery", "batterylevel"}
                );


            /*
                Your existing dataset has values like:

                550,94
                700,88
                36000,91

                If the battery cell contains two numbers,
                the SECOND number is treated as battery.
            */

            double battery = 0;


            size_t comma =
                batteryText.find(',');


            if (comma != string::npos)
            {
                string secondPart =
                    batteryText.substr(
                        comma + 1
                    );

                battery =
                    toDouble(secondPart);
            }
            else
            {
                battery =
                    toDouble(batteryText);
            }


            // ------------------------------------------------
            // OTHER COLUMNS
            // ------------------------------------------------

            string description =
                getColumn(
                    row,
                    columns,
                    {
                        "description",
                        "details",
                        "information"
                    }
                );


            string orbit =
                getColumn(
                    row,
                    columns,
                    {
                        "orbit",
                        "orbittype"
                    }
                );


            string launchDate =
                getColumn(
                    row,
                    columns,
                    {
                        "launchdate",
                        "launch_date",
                        "date"
                    }
                );


            string operatorName =
                getColumn(
                    row,
                    columns,
                    {
                        "operatorname",
                        "operator",
                        "opratorname",
                        "organisation",
                        "organization"
                    }
                );


            string missionPurpose =
                getColumn(
                    row,
                    columns,
                    {
                        "missionpurpose",
                        "purpose",
                        "objective"
                    }
                );


            string sensor =
                getColumn(
                    row,
                    columns,
                    {
                        "sensortype",
                        "sensor"
                    }
                );


            double resolution =
                toDouble(
                    getColumn(
                        row,
                        columns,
                        {
                            "resolution",
                            "resolutionm"
                        }
                    )
                );


            double temperature =
                toDouble(
                    getColumn(
                        row,
                        columns,
                        {
                            "temperature",
                            "temp"
                        }
                    )
                );


            double humidity =
                toDouble(
                    getColumn(
                        row,
                        columns,
                        {
                            "humidity"
                        }
                    )
                );


            double bandwidth =
                toDouble(
                    getColumn(
                        row,
                        columns,
                        {
                            "bandwidth"
                        }
                    )
                );


            string aiModel =
                getColumn(
                    row,
                    columns,
                    {
                        "aimodel",
                        "aimodelname",
                        "model"
                    }
                );


            // ------------------------------------------------
            // DEFAULT VALUES
            // ------------------------------------------------

            if (description.empty())
                description =
                    "Satellite used for space-based observation and monitoring.";


            if (orbit.empty())
                orbit = "LEO";


            if (launchDate.empty())
                launchDate = "Not Available";


            if (operatorName.empty())
                operatorName = "Not Available";


            if (missionPurpose.empty())
                missionPurpose =
                    "Satellite data collection and monitoring.";


            // ------------------------------------------------
            // CREATE OBJECT USING POLYMORPHISM
            // ------------------------------------------------

            Satellite* satellite = nullptr;


            if (
                type == "Earth Observation" ||
                type == "earth observation"
            )
            {
                satellite =
                    new EarthObservationSatellite(
                        id,
                        name,
                        battery,
                        description,
                        orbit,
                        launchDate,
                        operatorName,
                        missionPurpose,
                        sensor,
                        resolution
                    );
            }


            else if (
                type == "Weather" ||
                type == "weather"
            )
            {
                satellite =
                    new WeatherSatellite(
                        id,
                        name,
                        battery,
                        description,
                        orbit,
                        launchDate,
                        operatorName,
                        missionPurpose,
                        temperature,
                        humidity
                    );
            }


            else if (
                type == "Communication" ||
                type == "communication"
            )
            {
                satellite =
                    new CommunicationSatellite(
                        id,
                        name,
                        battery,
                        description,
                        orbit,
                        launchDate,
                        operatorName,
                        missionPurpose,
                        bandwidth
                    );
            }


            else if (
                type == "Smart Satellite" ||
                type == "smart satellite"
            )
            {
                satellite =
                    new SmartSatellite(
                        id,
                        name,
                        battery,
                        description,
                        orbit,
                        launchDate,
                        operatorName,
                        missionPurpose,
                        aiModel
                    );
            }


            // ------------------------------------------------
            // ADD VALID SATELLITE
            // ------------------------------------------------

            if (satellite != nullptr)
            {
                satellites.push_back(satellite);
            }
        }


        file.close();


        return satellites;
    }
};


// ============================================================
// 13. MISSION CSV LOADER
// ============================================================

class MissionCSVLoader
{
public:

    static vector<Mission*> load(
        string filename
    )
    {
        vector<Mission*> missions;


        ifstream file(filename);


        if (!file.is_open())
        {
            cout << "\nERROR: Could not open "
                 << filename << endl;

            return missions;
        }


        string line;


        if (!getline(file, line))
        {
            return missions;
        }


        vector<string> headers =
            parseCSVLine(line);


        map<string, int> columns;


        for (int i = 0;
             i < (int)headers.size();
             i++)
        {
            columns[
                normalizeHeader(headers[i])
            ] = i;
        }


        // ----------------------------------------------------
        // READ MISSIONS
        // ----------------------------------------------------

        while (getline(file, line))
        {
            if (trim(line).empty())
                continue;


            vector<string> row =
                parseCSVLine(line);


            if (row.size() < 3)
                continue;


            string id =
                getColumn(
                    row,
                    columns,
                    {
                        "id",
                        "missionid"
                    }
                );


            string name =
                getColumn(
                    row,
                    columns,
                    {
                        "name",
                        "missionname"
                    }
                );


            string target =
                getColumn(
                    row,
                    columns,
                    {
                        "target",
                        "targetlocation",
                        "location"
                    }
                );


            string objective =
                getColumn(
                    row,
                    columns,
                    {
                        "objective",
                        "purpose"
                    }
                );


            string sensor =
                getColumn(
                    row,
                    columns,
                    {
                        "sensor",
                        "sensortype"
                    }
                );


            string classifiedInfo =
                getColumn(
                    row,
                    columns,
                    {
                        "classifiedinfo",
                        "classifiedinformation",
                        "classifieddetails",
                        "classified"
                    }
                );


            string status =
                getColumn(
                    row,
                    columns,
                    {
                        "status",
                        "missionstatus"
                    }
                );


            string description =
                getColumn(
                    row,
                    columns,
                    {
                        "description",
                        "details",
                        "information"
                    }
                );


            string launchDate =
                getColumn(
                    row,
                    columns,
                    {
                        "launchdate",
                        "date"
                    }
                );


            string duration =
                getColumn(
                    row,
                    columns,
                    {
                        "estimatedduration",
                        "duration"
                    }
                );


            if (classifiedInfo.empty())
                classifiedInfo =
                    "Classified information restricted to authorized personnel.";


            if (description.empty())
                description =
                    "Satellite mission for observation and monitoring.";


            if (launchDate.empty())
                launchDate =
                    "Not Available";


            if (duration.empty())
                duration =
                    "Not Available";


            // ------------------------------------------------
            // CREATE MISSION
            // ------------------------------------------------

            ObservationMission* mission =
                new ObservationMission(
                    id,
                    name,
                    target,
                    objective,
                    sensor,
                    classifiedInfo,
                    description,
                    launchDate,
                    duration
                );


            mission->setStatus(
                stringToStatus(status)
            );


            missions.push_back(mission);
        }


        file.close();


        return missions;
    }
};


// ============================================================
// 14. MISSION PLANNER
// ============================================================

class MissionPlanner
{
private:

    vector<Satellite*> satellites;

    vector<Mission*> missions;

    vector<Resource*> resources;


public:

    // --------------------------------------------------------
    // ADD SATELLITE
    // --------------------------------------------------------

    void addSatellite(
        Satellite* satellite
    )
    {
        satellites.push_back(satellite);
    }


    // --------------------------------------------------------
    // ADD MISSION
    // --------------------------------------------------------

    void addMission(
        Mission* mission
    )
    {
        missions.push_back(mission);
    }


    // --------------------------------------------------------
    // ADD RESOURCE
    // --------------------------------------------------------

    void addResource(
        Resource* resource
    )
    {
        resources.push_back(resource);
    }


    // --------------------------------------------------------
    // DISPLAY SATELLITES
    // --------------------------------------------------------

    void displaySatellites()
    {
        cout << "\n========== SATELLITE FLEET ==========\n";


        for (
            Satellite* satellite :
            satellites
        )
        {
            satellite->displayDetails();
        }
    }


    // --------------------------------------------------------
    // PERFORM ACTIVE MISSIONS
    // --------------------------------------------------------

    void performActiveMissions()
    {
        cout << "\n========== ACTIVE MISSIONS ==========\n";


        for (
            Satellite* satellite :
            satellites
        )
        {
            satellite->performMission();
        }
    }


    // --------------------------------------------------------
    // PLANNED MISSIONS
    // --------------------------------------------------------

    void displayPlannedMissions()
    {
        cout << "\n========== PLANNED MISSIONS ==========\n";


        bool found = false;


        for (
            Mission* mission :
            missions
        )
        {
            if (
                mission->getStatus()
                == PLANNED
            )
            {
                mission->displayMission();

                found = true;
            }
        }


        if (!found)
            cout << "No planned missions.\n";
    }


    // --------------------------------------------------------
    // UPCOMING MISSIONS
    // --------------------------------------------------------

    void displayUpcomingMissions()
    {
        cout << "\n========== UPCOMING MISSIONS ==========\n";


        bool found = false;


        for (
            Mission* mission :
            missions
        )
        {
            if (
                mission->getStatus()
                == UPCOMING
            )
            {
                mission->displayMission();

                found = true;
            }
        }


        if (!found)
            cout << "No upcoming missions.\n";
    }


    // --------------------------------------------------------
    // ACTIVE MISSIONS
    // --------------------------------------------------------

    void displayActiveMissions()
    {
        cout << "\n========== ACTIVE MISSIONS ==========\n";


        bool found = false;


        for (
            Mission* mission :
            missions
        )
        {
            if (
                mission->getStatus()
                == ACTIVE
            )
            {
                mission->displayMission();

                found = true;
            }
        }


        if (!found)
            cout << "No active missions.\n";
    }


    // --------------------------------------------------------
    // COMPLETED MISSIONS
    // --------------------------------------------------------

    void displayCompletedMissions()
    {
        cout << "\n========== PREVIOUS MISSIONS ==========\n";


        bool found = false;


        for (
            Mission* mission :
            missions
        )
        {
            if (
                mission->getStatus()
                == COMPLETED
            )
            {
                mission->displayMission();

                found = true;
            }
        }


        if (!found)
            cout << "No completed missions.\n";
    }


    // --------------------------------------------------------
    // CLASSIFIED MISSIONS
    // --------------------------------------------------------

    void displayPrivateMissions()
    {
        cout << "\n========== CLASSIFIED MISSIONS ==========\n";


        for (
            Mission* mission :
            missions
        )
        {
            mission->displayPrivateInformation();
        }
    }


    // --------------------------------------------------------
    // UPDATE MISSION STATUS
    // --------------------------------------------------------

    void updateMissionStatus()
    {
        string id;


        cout << "\nEnter Mission ID: ";

        cin >> id;


        for (
            Mission* mission :
            missions
        )
        {
            if (
                mission->getMissionId()
                == id
            )
            {
                int choice;


                cout << "\n1. Planned";
                cout << "\n2. Upcoming";
                cout << "\n3. Active";
                cout << "\n4. Completed";


                cout << "\nEnter new status: ";

                cin >> choice;


                switch (choice)
                {
                    case 1:
                        mission->setStatus(PLANNED);
                        break;

                    case 2:
                        mission->setStatus(UPCOMING);
                        break;

                    case 3:
                        mission->setStatus(ACTIVE);
                        break;

                    case 4:
                        mission->setStatus(COMPLETED);
                        break;

                    default:
                        cout << "Invalid choice.\n";
                        return;
                }


                cout << "\nMission status updated successfully!\n";

                return;
            }
        }


        cout << "\nMission not found.\n";
    }


    // --------------------------------------------------------
    // ADMIN DASHBOARD
    // --------------------------------------------------------

    void adminDashboard()
    {
        int choice;


        do
        {
            cout << "\n\n========== ADMIN DASHBOARD ==========\n";

            cout << "1. View Planned Missions\n";
            cout << "2. View Upcoming Missions\n";
            cout << "3. View Active Missions\n";
            cout << "4. View Previous Missions\n";
            cout << "5. View Classified Information\n";
            cout << "6. Update Mission Status\n";
            cout << "7. Logout\n";


            cout << "\nEnter choice: ";

            cin >> choice;


            switch (choice)
            {
                case 1:
                    displayPlannedMissions();
                    break;

                case 2:
                    displayUpcomingMissions();
                    break;

                case 3:
                    displayActiveMissions();
                    break;

                case 4:
                    displayCompletedMissions();
                    break;

                case 5:
                    displayPrivateMissions();
                    break;

                case 6:
                    updateMissionStatus();
                    break;

                case 7:
                    cout << "\nLogged out successfully.\n";
                    break;

                default:
                    cout << "\nInvalid choice.\n";
            }

        }
        while (choice != 7);
    }


    // --------------------------------------------------------
    // DESTRUCTOR
    // --------------------------------------------------------

    ~MissionPlanner()
    {
        for (
            Satellite* satellite :
            satellites
        )
        {
            delete satellite;
        }


        for (
            Mission* mission :
            missions
        )
        {
            delete mission;
        }


        for (
            Resource* resource :
            resources
        )
        {
            delete resource;
        }
    }
};


// ============================================================
// 15. FRIEND FUNCTION - SATELLITE COMPARISON
// ============================================================

void compareSatellites(
    const Satellite& s1,
    const Satellite& s2
)
{
    cout << "\n========== SATELLITE COMPARISON ==========\n";


    if (s1.battery > s2.battery)
    {
        cout << s1.name
             << " has higher battery.\n";
    }
    else if (s2.battery > s1.battery)
    {
        cout << s2.name
             << " has higher battery.\n";
    }
    else
    {
        cout << "Both satellites have the same battery level.\n";
    }
}


// ============================================================
// 16. FRIEND FUNCTION - MISSION COMPARISON
// ============================================================

void compareMissions(
    const Mission& m1,
    const Mission& m2
)
{
    cout << "\n========== MISSION COMPARISON ==========\n";


    cout << "Mission 1: "
         << m1.missionName
         << endl;


    cout << "Mission 2: "
         << m2.missionName
         << endl;


    cout << "Status 1: "
         << statusToString(m1.status)
         << endl;


    cout << "Status 2: "
         << statusToString(m2.status)
         << endl;
}


// ============================================================
// 17. MAIN
// ============================================================

int main()
{
    MissionPlanner planner;


    // ========================================================
    // LOAD SATELLITES
    // ========================================================

    vector<Satellite*> satellites =
        SatelliteCSVLoader::load(
            "satellites.csv"
        );


    for (
        Satellite* satellite :
        satellites
    )
    {
        planner.addSatellite(satellite);
    }


    cout << "\nLoaded "
         << satellites.size()
         << " satellites from CSV.\n";


    // ========================================================
    // LOAD MISSIONS
    // ========================================================

    vector<Mission*> missions =
        MissionCSVLoader::load(
            "mission.csv"
        );


    for (
        Mission* mission :
        missions
    )
    {
        planner.addMission(mission);
    }


    cout << "\nLoaded "
         << missions.size()
         << " missions from CSV.\n";


    // ========================================================
    // PUBLIC VIEW
    // ========================================================

    planner.displaySatellites();

    planner.displayPlannedMissions();

    planner.displayUpcomingMissions();

    planner.displayActiveMissions();

    planner.displayCompletedMissions();


    // ========================================================
    // ADMIN LOGIN
    // ========================================================

    Admin admin(
        "admin",
        "isro_1234"
    );


    char loginChoice;


    cout << "\n\nDo you want to access Admin Panel? (y/n): ";

    cin >> loginChoice;


    if (
        loginChoice == 'y' ||
        loginChoice == 'Y'
    )
    {
        if (admin.login())
        {
            planner.adminDashboard();
        }
    }


    // ========================================================
    // TOTAL COUNTS
    // ========================================================

    cout << "\nTotal Satellites: "
         << Satellite::getTotalSatellites();


    cout << "\nTotal Missions: "
         << Mission::totalMissions
         << endl;


    return 0;
}

