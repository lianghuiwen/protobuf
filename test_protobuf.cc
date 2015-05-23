#include <iostream>
#include <fstream>
#include <string>
#include "person.pb.h"
using namespace std;

void WriteFile(const string& file) {
    tutorial::Person* person = new tutorial::Person();

    cout << "Enter person ID number: ";
    int id;
    cin >> id;
    person->set_id(id);
    cin.ignore(256, '\n');

    cout << "Enter name: ";
    string name;
    getline(cin, name);
    person->set_name(name);

    cout << "Enter email address (blank for none): ";
    string email;
    getline(cin, email);
    if (!email.empty()) {
        person->set_email(email);
    }

    while (true) {
        cout << "Enter a phone number (or leave blank to finish): ";
        string number;
        getline(cin, number);
        if (number.empty()) {
            break;
        }

        tutorial::Person::PhoneNumber* phone_number = person->add_phone();
        phone_number->set_number(number);

        cout << "Is this a mobile, home, or work phone? ";
        string type;
        getline(cin, type);
        if (type == "mobile") {
            phone_number->set_type(tutorial::Person::MOBILE);
        } else if (type == "home") {
            phone_number->set_type(tutorial::Person::HOME);
        } else if (type == "work") {
            phone_number->set_type(tutorial::Person::WORK);
        } else {
            cout << "Unknown phone type.  Using default." << endl;
        }
    }

    fstream output(file.c_str(), ios::out | ios::trunc | ios::binary);
    if (!person->SerializeToOstream(&output)) {
        cerr << "Failed to write." << endl;
    }
    output.close();
}

void ReadFile(const string& file) {
    tutorial::Person person;
    fstream input(file.c_str(), ios::in | ios::binary);
    if (!person.ParseFromIstream(&input)) {
        cerr << "Failed to parse." << endl;
        return;
    }

    cout << "Person ID: " << person.id() << endl;
    cout << "  Name: " << person.name() << endl;
    if (person.has_email()) {
        cout << "  E-mail address: " << person.email() << endl;
    }

    for (int j = 0; j < person.phone_size(); j++) {
        const tutorial::Person::PhoneNumber& phone_number = person.phone(j);

        switch (phone_number.type()) {
        case tutorial::Person::MOBILE:
            cout << "  Mobile phone #: ";
            break;
        case tutorial::Person::HOME:
            cout << "  Home phone #: ";
            break;
        case tutorial::Person::WORK:
            cout << "  Work phone #: ";
            break;
        }
        cout << phone_number.number() << endl;
    }
}

void ReadFile(tutorial::Person& person, const string& file) {
    fstream input(file.c_str(), ios::in | ios::binary);
    if (!person.ParseFromIstream(&input)) {
        cerr << "Failed to parse." << endl;
        return;
    }

    cout << "Person ID: " << person.id() << endl;
    cout << "  Name: " << person.name() << endl;
    if (person.has_email()) {
        cout << "  E-mail address: " << person.email() << endl;
    }

    for (int j = 0; j < person.phone_size(); j++) {
        const tutorial::Person::PhoneNumber& phone_number = person.phone(j);

        switch (phone_number.type()) {
        case tutorial::Person::MOBILE:
            cout << "  Mobile phone #: ";
            break;
        case tutorial::Person::HOME:
            cout << "  Home phone #: ";
            break;
        case tutorial::Person::WORK:
            cout << "  Work phone #: ";
            break;
        }
        cout << phone_number.number() << endl;
    }
}

using namespace google::protobuf;
void Test() {
    cout << tutorial::Person::descriptor()->full_name() << endl;
    const Descriptor* descriptor = 
        DescriptorPool::generated_pool()->FindMessageTypeByName("tutorial.Person");
    if (descriptor) {
        const Message* prototype = 
            MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (prototype) {
            Message* msg = prototype->New();
            tutorial::Person* person = dynamic_cast<tutorial::Person*>(msg);
            ReadFile(*person, "1.txt");
        } else {
            cout << "GetPrototype() fail!" << endl;
        }
    } else {
        cout << "FindMessageTypeByName() fail!" << endl;
    }
}

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    cout << "Usage: " << argv[0] << " [r/w] [file]" << endl;
    cout << "Usage: " << argv[0] << endl;
    
    if (argc == 3) {
        if (argv[1][0] == 'w')
            WriteFile(argv[2]);

        if (argv[1][0] == 'r')
            ReadFile(argv[2]);
    }

    if (argc == 1) {
        Test();
    }

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
