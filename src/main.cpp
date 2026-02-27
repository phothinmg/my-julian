#include <node.h>

#include <string>
#include <vector>
#include <cmath>
#include <optional>

using namespace std;

namespace G2J
{
    using v8::Exception;
    using v8::Array;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::String;
    using v8::Value;

    struct Jd2Dt
    {
        int year;
        int month;
        int day;
    };

    double g2jd(int year, int month, int day, int hour = 12, int minute = 0, int second = 0, float tz = 0.00)
    {
        double tzos = tz / 24.0;
        double def = (hour - 12) / 24.0 + minute / 1440.0 + second / 86400.0;

        int a = (month < 3) ? -1 : 0;
        int x4 = year + a;
        int x3 = x4 / 100;
        int x2 = x4 % 100;
        int x1 = month + (a ? 9 : -3);
        int _jdn = (146097 * x3) / 4 +
                   (36525 * x2) / 100 +
                   (153 * x1 + 2) / 5 +
                   day +
                   1721119;

        return _jdn + tzos + def;
    }
    Jd2Dt jd2dt(double jd, optional<float> tz_offset = nullopt)
    {
        float tz = tz_offset.value_or(0.0);
        double jdd = jd + tz / 24;

        double jdn = round(jdd);

        double a = 4 * jdn - 6884477;
        double x3 = floor(a / 146097);
        double r3 = fmod(a, 146097);

        double b = 100 * floor(r3 / 4) + 99;
        double x2 = floor(b / 36525);
        double r2 = fmod(b, 36525);

        double c = 5 * floor(r2 / 100) + 2;
        double x1 = floor(c / 153);
        double r1 = fmod(c, 153);

        double cc = floor((x1 + 2) / 12);
        int year = static_cast<int>(100 * x3 + x2 + cc);
        int month = static_cast<int>(x1 - 12 * cc + 3);
        int day = static_cast<int>(floor(r1 / 5) + 1);

        return {year, month, day};
    }
    void FromJulian(const FunctionCallbackInfo<Value> &args)
    {
        Isolate *isolate = args.GetIsolate();
        if (args.Length() < 1 || args.Length() > 2)
        {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()));
            return;
        }

        double jd = args[0]->NumberValue(isolate->GetCurrentContext()).ToChecked();
        float tz_offset = (args.Length() > 1) ? args[1]->NumberValue(isolate->GetCurrentContext()).ToChecked() : 0.0;
        Jd2Dt dt = jd2dt(jd, tz_offset);
        int year = dt.year;
        int month = dt.month;
        int day = dt.day;
        Local<Array> result = Array::New(isolate, 3);
        result->Set(isolate->GetCurrentContext(), 0, Number::New(isolate, year)).Check();
        result->Set(isolate->GetCurrentContext(), 1, Number::New(isolate, month)).Check();
        result->Set(isolate->GetCurrentContext(), 2, Number::New(isolate, day)).Check();
        args.GetReturnValue().Set(result);
    }
    void ToJulian(const FunctionCallbackInfo<Value> &args)
    {
        Isolate *isolate = args.GetIsolate();
        if (args.Length() < 3 || args.Length() > 7)
        {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()));
            return;
        }

        int year = args[0]->Int32Value(isolate->GetCurrentContext()).ToChecked();
        int month = args[1]->Int32Value(isolate->GetCurrentContext()).ToChecked();
        int day = args[2]->Int32Value(isolate->GetCurrentContext()).ToChecked();
        // set as optional values for h:m:s and tz
        int hour = (args.Length() > 3) ? args[3]->Int32Value(isolate->GetCurrentContext()).ToChecked() : 12;
        int minute = (args.Length() > 4) ? args[4]->Int32Value(isolate->GetCurrentContext()).ToChecked() : 0;
        int second = (args.Length() > 5) ? args[5]->Int32Value(isolate->GetCurrentContext()).ToChecked() : 0;
        float tz = (args.Length() > 6) ? args[6]->NumberValue(isolate->GetCurrentContext()).ToChecked() : 0.00;
        double value = g2jd(year, month, day, hour, minute, second, tz);
        args.GetReturnValue().Set(Number::New(isolate, value));
    }
    void Init(Local<Object> exports, Local<Value> module, void *priv)
    {
        NODE_SET_METHOD(exports, "gregorianToJulian", ToJulian);
        NODE_SET_METHOD(exports, "julianToGregorian", FromJulian);
    }
}

NODE_MODULE(NODE_GYP_MODULE_NAME, G2J::Init)
