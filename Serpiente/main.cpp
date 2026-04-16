#include "Form.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace MiProyecto;

[STAThreadAttribute]
int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew Form1());
    return 0;
}