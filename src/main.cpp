#include <node.h>

namespace G2J
{
    using v8::Exception;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object; // Include Object header
    using v8::String;
    using v8::Value;

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
    void Init(Local<Object> exports)
    {
        NODE_SET_METHOD(exports, "gregorianToJulian", ToJulian);
    }
    NODE_MODULE(NODE_GYP_MODULE_NAME, [](Local<Object> exports, Local<Value> module, void* priv) {
        Init(exports);
    })
}
