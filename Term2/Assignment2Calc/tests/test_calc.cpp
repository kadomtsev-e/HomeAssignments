#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "calc.hpp"

namespace {

constexpr double kTolerance = 1e-9;

void require(const bool condition, const std::string& message)
{
    if (!condition) {
        std::cerr << "Test failed: " << message << '\n';
        std::exit(1);
    }
}

void require_near(const double actual, const double expected, const std::string& message)
{
    require(
        std::fabs(actual - expected) <= kTolerance,
        message + ": expected " + std::to_string(expected) + ", got " + std::to_string(actual));
}

void require_contains(const std::string& text, const std::string& needle, const std::string& message)
{
    require(text.find(needle) != std::string::npos, message + ": '" + needle + "' not found in '" + text + "'");
}

class CerrCapture
{
public:
    CerrCapture()
        : old_(std::cerr.rdbuf(stream_.rdbuf()))
    {
    }

    ~CerrCapture()
    {
        std::cerr.rdbuf(old_);
    }

    std::string str() const
    {
        return stream_.str();
    }

private:
    std::ostringstream stream_;
    std::streambuf* old_;
};

} // namespace

int main()
{
    {
        bool rad_on = true;
        double current = 0;

        current = process_line(current, rad_on, "15");
        require_near(current, 15, "set literal works");

        current = process_line(current, rad_on, "+ 2.5");
        require_near(current, 17.5, "addition works");

        current = process_line(current, rad_on, "* -2");
        require_near(current, -35, "multiplication works with signed arguments");

        current = process_line(current, rad_on, "_");
        require_near(current, 35, "negation works");

        current = process_line(current, rad_on, "/ 5");
        require_near(current, 7, "division works");

        current = process_line(current, rad_on, "% 2");
        require_near(current, 1, "remainder works");

        current = process_line(current, rad_on, "^ 3");
        require_near(current, 1, "power works");

        current = process_line(current, rad_on, "SQRT");
        require_near(current, 1, "sqrt works");
    }

    {
        bool rad_on = true;
        double current = 90;

        current = process_line(current, rad_on, "DEG");
        require(!rad_on, "DEG switches calculator to degree mode");
        require_near(current, 90, "mode switch keeps register");

        current = process_line(current, rad_on, "SIN");
        require_near(current, 1, "sin in degree mode works");

        current = process_line(current, rad_on, "1");
        current = process_line(current, rad_on, "ASIN");
        require_near(current, 90, "asin returns degrees in degree mode");

        current = process_line(current, rad_on, "RAD");
        require(rad_on, "RAD switches calculator back to radians");
        require_near(current, 90, "RAD keeps register");

        current = process_line(current, rad_on, "1");
        current = process_line(current, rad_on, "ATAN");
        require_near(current, std::atan(1.0), "atan returns radians in radian mode");
    }

    {
        bool rad_on = false;
        double current = 10;

        current = process_line(current, rad_on, "(+) 1 2 3");
        require_near(current, 16, "left fold for addition works");

        current = process_line(current, rad_on, "(*) 2 3");
        require_near(current, 96, "left fold for multiplication works");

        current = process_line(current, rad_on, "2");
        current = process_line(current, rad_on, "(^) 2 3");
        require_near(current, 64, "left fold for power is left-associative");
    }

    {
        bool rad_on = false;
        const double current = 90;
        CerrCapture capture;

        const double updated = process_line(current, rad_on, "TAN");
        require_near(updated, current, "bad tangent argument keeps register");
        require_contains(capture.str(), "Bad argument for TAN: 90", "tangent domain error is reported");
    }

    {
        bool rad_on = true;
        const double current = -1;
        CerrCapture capture;

        const double updated = process_line(current, rad_on, "SQRT");
        require_near(updated, current, "bad sqrt argument keeps register");
        require_contains(capture.str(), "Bad argument for SQRT: -1", "sqrt domain error is reported");
    }

    {
        bool rad_on = true;
        const double current = 4;
        CerrCapture capture;

        const double updated = process_line(current, rad_on, "(/) 2 0 5");
        require_near(updated, current, "bad fold keeps original register");
        require_contains(capture.str(), "Bad right argument for division: 0", "fold binary errors are reported");
    }

    {
        double current = 0;

        current = process_line(current, "90");
        current = process_line(current, "DEG");
        current = process_line(current, "SIN");
        require_near(current, 1, "legacy process_line overload keeps trig mode");

        current = process_line(current, "RAD");
        require_near(current, 1, "legacy mode switch keeps register");
    }

    {
        bool rad_on = true;
        const double current = 0;
        CerrCapture capture;

        const double updated = process_line(current, rad_on, "12345678901");
        require_near(updated, current, "oversized literal keeps register");
        require_contains(
            capture.str(),
            "Too many decimal digits in argument: 12345678901",
            "oversized literal reports a numeric parsing error");
    }

    {
        bool rad_on = true;
        const double current = 0;
        CerrCapture capture;

        const double updated = process_line(current, rad_on, "12x");
        require_near(updated, current, "malformed literal keeps register");
        require_contains(
            capture.str(),
            "Bad numeric argument: 12x",
            "malformed literal reports a numeric parsing error");
    }

    {
        bool rad_on = true;
        const double current = -2;
        CerrCapture capture;

        const double updated = process_line(current, rad_on, "^ 0.5");
        require_near(updated, current, "undefined power keeps register");
        require_contains(capture.str(), "Bad arguments for power", "undefined power is reported");
    }

    {
        bool rad_on = false;
        double current = 0;

        current = process_line(current, rad_on, "1");
        current = process_line(current, rad_on, "ACOS");
        require_near(current, 0, "acos returns degrees in degree mode");

        current = process_line(current, rad_on, "0");
        current = process_line(current, rad_on, "ACTN");
        require_near(current, 90, "actn returns degrees in degree mode");

        current = process_line(current, rad_on, "45");
        current = process_line(current, rad_on, "CTN");
        require_near(current, 1, "ctn works in degree mode");
    }

    std::cout << "All calculator tests passed.\n";
    return 0;
}
