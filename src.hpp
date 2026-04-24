
#ifndef SRC_HPP
#define SRC_HPP

#include <iostream>
#include <algorithm>
#include <string>
#include "base.hpp"

using namespace std;

struct date {
    int year, month, day;
    date() = default;
    date(int y, int m, int d) : year(y), month(m), day(d) {}

    bool operator<(const date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator==(const date& other) const {
        return year == other.year && month == other.month && day == other.day;
    }

    friend istream& operator>>(istream& is, date& d) {
        is >> d.year >> d.month >> d.day;
        return is;
    }

    int to_days() const {
        return year * 360 + month * 30 + day;
    }
};

class mail : public object {
protected:
    string postmark;
    date send_date;
    date arrive_date;

public:
    mail() = default;
    mail(string _contain_, string _postmark_, date send_d, date arrive_d)
        : object(_contain_), postmark(_postmark_), send_date(send_d), arrive_date(arrive_d) {}

    virtual string send_status(int y, int m, int d) override {
        return "not send";
    }

    virtual string type() override {
        return "no type";
    }

    virtual void print() override {
        object::print();
        cout << "[mail] postmark: " << postmark << endl;
    }

    virtual void copy(object* o) override {
        mail* other = reinterpret_cast<mail*>(o);
        contain = other->contain;
        postmark = other->postmark;
        send_date = other->send_date;
        arrive_date = other->arrive_date;
    }
};

class air_mail : public mail {
protected:
    string airlines;
    date take_off_date;
    date land_date;

public:
    air_mail() = default;
    air_mail(string _contain_, string _postmark_, date send_d, date arrive_d, date take_off, date land, string _airline)
        : mail(_contain_, _postmark_, send_d, arrive_d), airlines(_airline), take_off_date(take_off), land_date(land) {}

    virtual string send_status(int y, int m, int d) override {
        date query_date(y, m, d);
        if (query_date < send_date) return "mail not send";
        if (query_date < take_off_date) return "wait in airport";
        if (query_date < land_date) return "in flight";
        if (query_date < arrive_date) return "already land";
        return "already arrive";
    }

    virtual string type() override {
        return "air";
    }

    virtual void print() override {
        mail::print();
        cout << "[air] airlines: " << airlines << endl;
    }

    virtual void copy(object* o) override {
        air_mail* other = reinterpret_cast<air_mail*>(o);
        mail::copy(o);
        airlines = other->airlines;
        take_off_date = other->take_off_date;
        land_date = other->land_date;
    }
};

class train_mail : public mail {
protected:
    string* station_name;
    date* station_time;
    int len;

public:
    train_mail() : station_name(nullptr), station_time(nullptr), len(0) {}
    train_mail(string _contain_, string _postmark_, date send_d, date arrive_d, string* sname, date* stime, int station_num)
        : mail(_contain_, _postmark_, send_d, arrive_d), len(station_num) {
        station_name = new string[len];
        station_time = new date[len];
        for (int i = 0; i < len; ++i) {
            station_name[i] = sname[i];
            station_time[i] = stime[i];
        }
    }

    virtual ~train_mail() {
        delete[] station_name;
        delete[] station_time;
    }

    virtual string send_status(int y, int m, int d) override {
        date query_date(y, m, d);
        if (query_date < send_date) return "mail not send";
        for (int i = 0; i < len; ++i) {
            if (query_date == station_time[i]) {
                return "train arrive at " + station_name[i];
            }
            if (query_date < station_time[i]) {
                if (i == 0) return "train will arrive at " + station_name[0];
                return "train is between " + station_name[i - 1] + " and " + station_name[i];
            }
        }
        if (query_date < arrive_date) return "train already arrive at " + station_name[len - 1];
        return "already arrive";
    }

    virtual string type() override {
        return "train";
    }

    virtual void print() override {
        mail::print();
        cout << "[train] station_num: " << len << endl;
    }

    virtual void copy(object* o) override {
        train_mail* other = reinterpret_cast<train_mail*>(o);
        mail::copy(o);
        delete[] station_name;
        delete[] station_time;
        len = other->len;
        station_name = new string[len];
        station_time = new date[len];
        for (int i = 0; i < len; ++i) {
            station_name[i] = other->station_name[i];
            station_time[i] = other->station_time[i];
        }
    }
};

class car_mail : public mail {
protected:
    int total_mile;
    string driver;

public:
    car_mail() = default;
    car_mail(string _contain_, string _postmark_, date send_d, date arrive_d, int mile, string _driver)
        : mail(_contain_, _postmark_, send_d, arrive_d), total_mile(mile), driver(_driver) {}

    virtual string send_status(int y, int m, int d) override {
        date query_date(y, m, d);
        if (query_date < send_date) return "mail not send";
        if (!(query_date < arrive_date)) return "already arrive";
        
        double total_time = (double)arrive_date.to_days() - send_date.to_days();
        double used_time = (double)query_date.to_days() - send_date.to_days();
        double current_mile = (used_time / total_time) * total_mile;
        return to_string(current_mile);
    }

    virtual string type() override {
        return "car";
    }

    virtual void print() override {
        mail::print();
        cout << "[car] driver_name: " << driver << endl;
    }

    virtual void copy(object* o) override {
        car_mail* other = reinterpret_cast<car_mail*>(o);
        mail::copy(o);
        total_mile = other->total_mile;
        driver = other->driver;
    }
};

void obj_swap(object*& lhs, object*& rhs) {
    object* temp = lhs;
    lhs = rhs;
    rhs = temp;
}

#endif
