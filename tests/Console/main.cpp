
// ForkENGINE: Console Test
// 25/02/2014

#include <fengine/core.h>
#include <fengine/using.h>
#include <iostream>

using namespace std;
using namespace Fork;

class MyLogEventHandler : public IO::Console::LogEventHandler
{
    public:
        void OnPrintLn(const std::string& indent, const std::string& message, const IO::Log::EntryTypes type) override;
        //void OnStartLn(const std::string& indent);
};

/*void MyLogEventHandler::OnStartLn(const std::string& indent)
{
    OnPushColor(Platform::ConsoleManip::Colors::White);
    OnPrint("[" + IO::SystemClock::SecondsToTimePoint(IO::SystemClock::ElapsedTime()) + "] ");
    OnPopColor();
    IO::Console::LogEventHandler::OnStartLn(indent);
}*/

void MyLogEventHandler::OnPrintLn(const std::string& indent, const std::string& message, const IO::Log::EntryTypes type)
{
    auto start = message.find("\"");
    if (start != std::string::npos)
    {
        auto end = message.find("\"", start + 1);
        if (end != std::string::npos)
        {
            ++end;
            Platform::ConsoleManip::ScopedColor unused(Platform::ConsoleManip::Colors::Gray);
            IO::Console::Print(indent + message.substr(0, start));
            {
                Platform::ConsoleManip::ScopedColor unused2(Platform::ConsoleManip::Colors::Pink | Platform::ConsoleManip::Colors::Intens);
                IO::Console::Print(message.substr(start, end - start));
            }
            IO::Console::PrintLn(message.substr(end, message.size() - end));
            return;
        }
    }
    IO::Console::LogEventHandler::OnPrintLn(indent, message, type);
}

int main()
{
    IO::Log::AddEventHandler(std::make_shared<MyLogEventHandler>());
    IO::Log::AddEventHandler(std::make_shared<IO::LogFile::LogEventHandler>("LogFile.txt"));

    #if 1//!ENCRYPTION TEST!

    IO::CryptoBitKey bitKey("LoL Code");

    std::string encryptStr = "One Time Pad";

    IO::Log::Message("Original String: \"" + encryptStr + "\"");
    bitKey.EncodeContainer(encryptStr);

    IO::Log::Message("After Encryption: \"" + encryptStr + "\"");
    bitKey.DecodeContainer(encryptStr);

    IO::Log::Message("After Decryption: \"" + encryptStr + "\"");

    #endif

    #if 1//!COLLISION TEST!
    {

    Math::Triangle3f triA { { -1, -1, 0 }, { 0, 1, 0 }, { 1, -1, 0 } };
    Math::Triangle3f triB { { -1, 0, -1 }, { 0, 0, 1 }, { 0, 0, -1 } };

    Math::Line3f intersect;
    if (Math::CheckIntersectionWithTriangle(triA, triB, intersect))
        IO::Log::Message("Intersection: " + IO::Printer::Print(intersect.start) + ", " + IO::Printer::Print(intersect.end));
    else
        IO::Log::Message("No Intersection");

    }
    #endif

    #if 1//!MEMENTO TEST!

    MementoHierarchy<int> timeline(0);

    timeline.Commit(3);
    timeline.Commit(4);
    timeline.Prev();
    timeline.Commit(5);
    timeline.Commit(6);
    timeline.Prev();
    timeline.Prev();
    timeline.Commit(4);
    timeline.Commit(5);
    timeline.Commit(7);

    timeline.Traverse(
        [](MementoHierarchy<int>::TreeNode* node)
        {
            IO::Log::Message(std::string(node->Level()*2, ' ') + ToStr(node->data));
        }
    );

    #endif

    // Common math tests
    Matrix2f m2;
    Matrix3f m3;
    Matrix4f m4;

    AABB2f box1;
    box1.InsertPoint(Vector2f(5, 2));

    Angle<> angle = Degree<>(45.0f) * 2.0f + Radian<>(0.5f);
    angle += Degree<>(90.0f);
    angle += Radian<>(Math::pi/2);
    angle *= 2.5f;

    Angle<> angle2 = Degree<>(90.0f) + Radian<>(Math::pi/2);
    auto deg1 = angle2.Get<Degree<>>();
    Degree<> deg2(angle2);
    auto rad1 = angle2.Get<Radian<>>();
    Radian<> rad2(angle2);
    auto degRadEq = (rad2 == deg2);

    Recti rc(4, 2, 20, 19);

    Frustum<> frustum;
    frustum.Normalize();

    Line3f line(Vector3f(0, 0, 0), Vector3f(0, 2, 3));

    Ray3f ray(line);

    Cylinder<> cylinder(1.0f, 3.0f);
    IO::Log::Message("cylinder volume = " + ToStr(cylinder.Volume()));
    Cone<> cone(1.0f, 3.0f);
    IO::Log::Message("cone volume = " + ToStr(cone.Volume()));
    Capsule<> capsule(1.0f, 3.0f);
    IO::Log::Message("capsule volume = " + ToStr(capsule.Volume()));

    // Quaternion tests
    m3.RotateX(Degree<>(70).Get<Radian<>>());
    Quaternion<> quat(m3);

    Matrix3f m3b = quat.Mat3();

    IO::Console::PrintLn("m3:");
    IO::Console::PrintLn(IO::Printer::Print(m3));

    IO::Console::PrintLn("\nquat:");
    IO::Console::PrintLn(IO::Printer::Print(quat));

    IO::Console::PrintLn("\nm3b:");
    IO::Console::PrintLn(IO::Printer::Print(m3b));

    IO::Console::PrintLn("\nm4:");
    IO::Console::PrintLn(IO::Printer::Print(m4));

    Transform2Df trans2;
    Transform3Df trans3;

    quat.SetupEulerRotation(Vector3f(0.5f, 0.2f, -0.4f));
    auto eulerRot = quat.EulerRotation();
    IO::Console::PrintLn(IO::Printer::Print(eulerRot));

    // Container tests
    try
    {
        IO::Log::Message("Buffer Test");

        struct Test
        {
            Test(int X = 0, float* P = nullptr) : x(X), p(P) {}
            int x;
            float* p;
        };

        StrideBuffer buf;
        buf.SetStride(sizeof(Test));

        buf.PushBack<Test>(Test(5));
        buf.PushBack<Test>(Test(42));

        for (size_t i = 0; i < buf.Size(); ++i)
        {
            const auto entry = buf.Get<Test>(i);
            IO::Log::Message("x = " + ToStr(entry.x) + ", p = " + ToStr((long)entry.p));
        }
    }
    catch (const std::exception& err)
    {
        IO::Log::Error(err.what());
    }

    try
    {
        throw IndexOutOfBoundsException(__FUNCTION__, 5);
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
    }

    // File test
    IO::VirtualFile inFile("test.txt");
    inFile.WriteStringNL("/--------------\\");
    inFile.WriteStringNL("| Hello, World |");
    inFile.WriteStringNL("\\--------------/");
    inFile.WriteToHDD();

    // OS version tests
    const auto sysInfo = Platform::QuerySystemInfo();

    std::string version;

    version += "<0101>" + sysInfo.osName + "</>";
    version += " " + sysInfo.osProductType;
    version += " " + sysInfo.osExtensions;
    version += " <0110>(" + sysInfo.osBuild + ")</>";

    IO::Log::MessageColored(version);

    // Console tests
    IO::Log::Message("ForkENGINE Version " + EngineVersion());

    IO::Log::ScopedIndent indent;
    IO::Log::Message("Hello, World!");

    IO::Log::Message("angle = " + ToStr(angle.Get<Degree<>>()) + " degrees");
    IO::Log::Message("angle = " + ToStr(angle.Get<Radian<>>()) + " radians");
    IO::Log::Message("angle = " + ToStr(angle.Get<Angle<>>()) + " angle units");

    IO::Log::Error("Test 1");
    IO::Log::Message("Test 2");
    IO::Log::Warning("Test 3");
    IO::Log::Message("Test 4", Platform::ConsoleManip::Colors::Black, Platform::ConsoleManip::Colors::White);
    IO::Log::Success("Test Completed Successful");
    IO::Log::Message("Hello, \"User\"!");

    IO::Log::MessageColored("Foo <1101>Bar</> Blub &amp; <0000|1001>x &lt; 2</> End");
    IO::Log::MessageColored("<1001>L</><0101>U</><0011>K</><1101>A</><1011>S</>");

    // Polynom and printer test
    IO::Log::Message("Polynomial: " + IO::Printer::Print(Math::Polynomial<float, 3>({ 3, -4.34f, 0, 1 })));

    // Raster number tests
    const float rasterSize = 5.0f;
    const bool flushRaster = true;
    Math::RasterNumber<float> rasterNum = -8.5f;

    auto IncRaster = [&](float val)
    {
        rasterNum += val;

        auto num  = static_cast<float>(rasterNum);
        auto rnum = (flushRaster ? rasterNum.RasterFlush(rasterSize) : rasterNum.Raster(rasterSize));

        IO::Log::Message(
            "num = " + ToStr(num) +
            "\trasterNum(" + ToStr(rasterSize) +
            ") = " + ToStr(rnum)
        );
    };

    IncRaster(4.4f);
    IncRaster(2.6f);
    IncRaster(1.5f);
    IncRaster(0.4f);
    IncRaster(3.8f);
    IncRaster(5.7f);

    // Filename tests
    IO::Filename filename("/home/lh/Tests/TestFile1.txt");

    filename.ChangePath("C:/Users/Lukas Hermanns\\");
    //filename.PopPath();
    filename.AddPath("../../Program Files\\Test/../Foo-Bar");
    filename.RemovePath("c:/program files/Foo-bars/Bla");
    filename.ChangeExt("dump");
    filename.ChangeNameExt("test.obj");
    filename.ChangeName("LoL...");
    filename.ChangeName("FILE");

    IO::Log::Message("Filename tests:");
    IO::Log::Message("filename.Get      = " + filename.Get());
    IO::Log::Message("filename.Path     = " + filename.Path());
    IO::Log::Message("filename.Name     = " + filename.Name());
    IO::Log::Message("filename.Ext      = " + filename.Ext());
    IO::Log::Message("filename.NameExt  = " + filename.NameExt());

    IO::Console::Wait();

    return 0;
}

