#include <bits/stdc++.h>
using namespace std;

class Observation {
public:
    string plateNumber;
    string date;
    string carType;
    int speed;
    bool seatbeltFastened;
};

class Violation {
public:
    string description;
    int fee;
};

class Rule {
public:
    virtual optional<Violation> check(
        const Observation &observation
    ) = 0;

    virtual string getName() const = 0;

    virtual ~Rule() = default;
};

class SeatbeltRule : public Rule {
public:
    optional<Violation> check(
        const Observation &observation
    ) override {
        if (!observation.seatbeltFastened) {
            Violation violation;

            violation.description =
                    "Seatbelt not fastned";

            violation.fee = 100;

            return violation;
        }

        return nullopt;
    }

    string getName() const override {
        return "Seatbelt Rule";
    }
};

class SpeedRule : public Rule {
public:
    optional<Violation> check(
        const Observation &observation
    ) override {
        if (observation.carType == "Truck" &&
            observation.speed > 60) {
            Violation violation;

            violation.description =
                    "speed of " +
                    to_string(observation.speed) +
                    " exceeded max allowed 60";

            violation.fee = 300;

            return violation;
        }

        if (observation.carType == "Private" &&
            observation.speed > 80) {
            Violation violation;

            violation.description =
                    "speed of " +
                    to_string(observation.speed) +
                    " exceeded max allowed 80";

            violation.fee = 300;

            return violation;
        }

        return nullopt;
    }

    string getName() const override {
        return "Speed Rule";
    }
};

class Fine {
private:
    string plateNumber;
    vector<Violation> violations;

public:
    Fine(
        const string &plateNum,
        const vector<Violation> &violation
    )
        : plateNumber(plateNum),
          violations(violation) {
    }

    string getPlateNumber() const {
        return plateNumber;
    }

    int getTotalAmount() const {
        int total = 0;

        for (const auto &violation: violations) {
            total += violation.fee;
        }

        return total;
    }

    string generateReport() const {
        string report;

        report +=
                "Traffic fine for car " +
                plateNumber + "\n";

        report +=
                "Total amount: " +
                to_string(getTotalAmount()) +
                " EGP\n";

        report += "Violations:\n";

        for (const auto &[description, fee]: violations) {
            report +=
                    "- " + description +
                    " : " +
                    to_string(fee) +
                    " EGP\n";
        }

        return report;
    }
};

class Radar {
private:
    vector<Rule *> rules;
    vector<Fine> fines;

    map<string, int> violatedRulesCount;

public:
    void addRule(Rule *rule) {
        rules.push_back(rule);
    }

    void processObservation(
        const Observation &observation
    ) {
        vector<Violation> violations;

        for (Rule *rule: rules) {
            optional<Violation> result =
                    rule->check(observation);

            if (result.has_value()) {
                violations.push_back(
                    result.value()
                );

                violatedRulesCount[
                    rule->getName()
                ]++;
            }
        }

        if (!violations.empty()) {
            fines.emplace_back(
                observation.plateNumber,
                violations
            );
        }
    }

    void AllFines() const {
        for (const Fine &fine: fines) {
            cout
                    << fine.getPlateNumber()
                    << " : "
                    << fine.getTotalAmount()
                    << " EGP\n";
        }
    }

    void ViolatedRules() const {
        for (const auto &[ruleName, count]
             : violatedRulesCount) {
            cout
                    << ruleName
                    << " : "
                    << count
                    << '\n';
        }
    }

    void printAllFines() const {
        for (const Fine &fine: fines) {
            cout << fine.generateReport();
            cout << '\n';
        }
    }
};

class Main {
public:
    void run() {
        Radar radar;

        SpeedRule speedRule;
        SeatbeltRule seatbeltRule;

        radar.addRule(&seatbeltRule);
        radar.addRule(&speedRule);

        Observation car1;

        car1.plateNumber = "ABC1234";
        car1.date = "2026-07-24";
        car1.carType = "Private";
        car1.speed = 94;
        car1.seatbeltFastened = false;

        Observation car2;

        car2.plateNumber = "XYZ5678";
        car2.date = "2026-07-24";
        car2.carType = "Truck";
        car2.speed = 75;
        car2.seatbeltFastened = true;

        Observation car3;

        car3.plateNumber = "CAR9999";
        car3.date = "2026-07-24";
        car3.carType = "Private";
        car3.speed = 70;
        car3.seatbeltFastened = true;

        radar.processObservation(car1);
        radar.processObservation(car2);
        radar.processObservation(car3);


        radar.printAllFines();


        radar.AllFines();


        cout << endl;

        radar.ViolatedRules();
    }
};

int main() {

    Main test;

    test.run();
    return 0;
}
