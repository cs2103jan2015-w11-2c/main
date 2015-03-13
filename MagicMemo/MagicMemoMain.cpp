#include "MagicMemoGUI.h"

using namespace System; 
using namespace System::Windows::Forms;

[STAThread] 
void main(array<String^>^ args) { 
	Application::EnableVisualStyles(); 
	Application::SetCompatibleTextRenderingDefault(false);

	UI::MagicMemoGUI mainWindow; 
	Application::Run(%mainWindow); 
}