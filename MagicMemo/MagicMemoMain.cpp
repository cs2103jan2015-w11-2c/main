#include "MagicMemoGUI.h"

using namespace System; 
using namespace System::Windows::Forms;

[STAThread] 
int main(array<String^>^ argv) { 
	Application::EnableVisualStyles(); 
	Application::SetCompatibleTextRenderingDefault(false);

	MagicMemo::MagicMemoGUI mainWindow; 
	Application::Run(%mainWindow); 

	return 0;
}