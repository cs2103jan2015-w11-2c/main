#pragma once
//@author A0111951N
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include "Controller.h"
#include "MessageManager.h"

#pragma once
#include <Windows.h>
#pragma comment(lib, "user32.lib")

using namespace std;

namespace MagicMemo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MagicMemoGUI
	/// </summary>
	public ref class MagicMemoGUI : public System::Windows::Forms::Form {
	private:
		MessageManager^ magicManager;

	public:
		MagicMemoGUI(void) {
			InitializeComponent();
			magicManager = gcnew MessageManager();
			setGuiText("display");
			successMessageLabel->Text = L"Welcome to Magic Memo!";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MagicMemoGUI() {
			if (components) {
				delete components;
				delete magicManager;
			}
		}
	private: System::Windows::Forms::Label^  todayTaskBoxLabel;

	private: System::Windows::Forms::TextBox^  commandInputBox;
	private: System::Windows::Forms::Label^  allTaskBoxLabel;

	private: System::Windows::Forms::Label^  programHeading;
	private: System::Windows::Forms::RichTextBox^  todayTaskBox;
	private: System::Windows::Forms::RichTextBox^  allTaskBox;
	private: System::Windows::Forms::Label^  successMessageLabel;
	private: System::Windows::Forms::PictureBox^  pictureBox;


	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.

		/// </summary>


		//@author generated
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void) {
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MagicMemoGUI::typeid));
			this->todayTaskBoxLabel = (gcnew System::Windows::Forms::Label());
			this->commandInputBox = (gcnew System::Windows::Forms::TextBox());
			this->allTaskBoxLabel = (gcnew System::Windows::Forms::Label());
			this->programHeading = (gcnew System::Windows::Forms::Label());
			this->allTaskBox = (gcnew System::Windows::Forms::RichTextBox());
			this->successMessageLabel = (gcnew System::Windows::Forms::Label());
			this->todayTaskBox = (gcnew System::Windows::Forms::RichTextBox());
			this->pictureBox = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox))->BeginInit();
			this->SuspendLayout();
			// 
			// todayTaskBoxLabel
			// 
			this->todayTaskBoxLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->todayTaskBoxLabel->AutoSize = true;
			this->todayTaskBoxLabel->Font = (gcnew System::Drawing::Font(L"Arial Rounded MT Bold", 12.75F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
<<<<<<< HEAD
			this->todayTaskBoxLabel->Location = System::Drawing::Point(77, 45);
=======
			this->todayTaskBoxLabel->Location = System::Drawing::Point(76, 45);
>>>>>>> master
			this->todayTaskBoxLabel->Name = L"todayTaskBoxLabel";
			this->todayTaskBoxLabel->Size = System::Drawing::Size(147, 20);
			this->todayTaskBoxLabel->TabIndex = 0;
			this->todayTaskBoxLabel->Text = L"Upcoming Tasks";
			// 
			// commandInputBox
			// 
			this->commandInputBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->commandInputBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Append;
			this->commandInputBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
			this->commandInputBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->commandInputBox->Location = System::Drawing::Point(16, 358);
			this->commandInputBox->Name = L"commandInputBox";
			this->commandInputBox->Size = System::Drawing::Size(562, 22);
			this->commandInputBox->TabIndex = 1;
			this->commandInputBox->Text = L"Enter text here:";
			this->commandInputBox->UseWaitCursor = true;
			this->commandInputBox->TextChanged += gcnew System::EventHandler(this, &MagicMemoGUI::commandInputBox_TextChanged);
			this->commandInputBox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MagicMemoGUI::commandInputBox_KeyDown);
			// 
			// allTaskBoxLabel
			// 
			this->allTaskBoxLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->allTaskBoxLabel->Font = (gcnew System::Drawing::Font(L"Arial Rounded MT Bold", 12.75F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
<<<<<<< HEAD
			this->allTaskBoxLabel->Location = System::Drawing::Point(374, 45);
=======
			this->allTaskBoxLabel->Location = System::Drawing::Point(352, 45);
>>>>>>> master
			this->allTaskBoxLabel->Name = L"allTaskBoxLabel";
			this->allTaskBoxLabel->Size = System::Drawing::Size(168, 20);
			this->allTaskBoxLabel->TabIndex = 5;
			this->allTaskBoxLabel->Text = L"Other Tasks";
			this->allTaskBoxLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// programHeading
			// 
			this->programHeading->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->programHeading->AutoSize = true;
			this->programHeading->Font = (gcnew System::Drawing::Font(L"Cooper Black", 30, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->programHeading->ForeColor = System::Drawing::Color::DarkBlue;
			this->programHeading->Location = System::Drawing::Point(158, 4);
			this->programHeading->Name = L"programHeading";
			this->programHeading->Size = System::Drawing::Size(265, 46);
			this->programHeading->TabIndex = 6;
			this->programHeading->Text = L"Magic Memo";
			// 
			// allTaskBox
			// 
			this->allTaskBox->BackColor = System::Drawing::Color::White;
			this->allTaskBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->allTaskBox->Font = (gcnew System::Drawing::Font(L"Palatino Linotype", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->allTaskBox->Location = System::Drawing::Point(315, 75);
			this->allTaskBox->Margin = System::Windows::Forms::Padding(5);
			this->allTaskBox->Name = L"allTaskBox";
			this->allTaskBox->ReadOnly = true;
			this->allTaskBox->Size = System::Drawing::Size(260, 255);
			this->allTaskBox->TabIndex = 7;
			this->allTaskBox->Text = L"";
			this->allTaskBox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MagicMemoGUI::allTaskBox_KeyDown);
			// 
			// successMessageLabel
			// 
			this->successMessageLabel->Font = (gcnew System::Drawing::Font(L"Franklin Gothic Demi", 12, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->successMessageLabel->Location = System::Drawing::Point(18, 335);
			this->successMessageLabel->Name = L"successMessageLabel";
			this->successMessageLabel->Size = System::Drawing::Size(197, 21);
			this->successMessageLabel->TabIndex = 8;
			this->successMessageLabel->Text = L"Welcome to Magic Memo!";
			// 
			// todayTaskBox
			// 
			this->todayTaskBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->todayTaskBox->BackColor = System::Drawing::Color::White;
			this->todayTaskBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->todayTaskBox->Font = (gcnew System::Drawing::Font(L"Palatino Linotype", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->todayTaskBox->Location = System::Drawing::Point(16, 75);
			this->todayTaskBox->Margin = System::Windows::Forms::Padding(5);
			this->todayTaskBox->Name = L"todayTaskBox";
			this->todayTaskBox->ReadOnly = true;
			this->todayTaskBox->Size = System::Drawing::Size(260, 255);
			this->todayTaskBox->TabIndex = 7;
			this->todayTaskBox->Text = L"";
			this->todayTaskBox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MagicMemoGUI::lastActionBox_KeyDown);
			// 
			// pictureBox
			// 
			this->pictureBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox->BackColor = System::Drawing::Color::White;
			this->pictureBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->pictureBox->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox.Image")));
			this->pictureBox->ImageLocation = L"";
			this->pictureBox->Location = System::Drawing::Point(13, 67);
			this->pictureBox->Name = L"pictureBox";
			this->pictureBox->Size = System::Drawing::Size(565, 265);
			this->pictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox->TabIndex = 9;
			this->pictureBox->TabStop = false;
			// 
			// MagicMemoGUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(590, 391);
			this->Controls->Add(this->todayTaskBox);
			this->Controls->Add(this->successMessageLabel);
			this->Controls->Add(this->allTaskBox);
			this->Controls->Add(this->commandInputBox);
			this->Controls->Add(this->allTaskBoxLabel);
			this->Controls->Add(this->todayTaskBoxLabel);
			this->Controls->Add(this->pictureBox);
			this->Controls->Add(this->programHeading);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"MagicMemoGUI";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Magic Memo";
			this->Load += gcnew System::EventHandler(this, &MagicMemoGUI::MagicMemoGUI_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		//@author A0111951N
	private:
		//Get value while typing
		System::Void commandInputBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

		}
	private:
		System::Void commandInputBox_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			if(e->KeyCode == Keys::Enter) {
				String^ inputText = commandInputBox->Text;
				setGuiText(inputText);

				magicManager->updateAutoCompleteSource(commandInputBox);
			}

			// Minimize via Ctrl + W
			if (e->KeyData == (Keys::Control | Keys::W)) {
				WindowState = FormWindowState::Minimized;
			} 

			// Exit via Ctrl + Q
			if (e->KeyData == (Keys::Control | Keys::Q)) {
				MessageBox::Show("Good bye!");
				Application::Exit();
			}

			// Undo via Ctrl + Z
			if (e->KeyData == (Keys::Control | Keys::Z)) {
				setGuiText("undo");
			}   

			// Redo via Ctrl + R
			if (e->KeyData == (Keys::Control | Keys::R)) {
				setGuiText("redo");
			}

		}

	private: 
		// return focus to the intput textbox on keypress
		System::Void allTaskBox_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			if((e->KeyCode != Keys::Up) && (e->KeyCode != Keys::Down) && (e->KeyCode != Keys::Left) && (e->KeyCode != Keys::Right)) {
				commandInputBox->Focus(); 
			}
		}

	private: 
		// return focus to the intput textbox on keypress
		System::Void lastActionBox_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			if((e->KeyCode != Keys::Up) && (e->KeyCode != Keys::Down) && (e->KeyCode != Keys::Left) && (e->KeyCode != Keys::Right)) {
				commandInputBox->Focus(); 
			}
		}

	private: 
		System::Void MagicMemoGUI_Load(System::Object^  sender, System::EventArgs^  e) {
		}

	private:
		// Sets success message and fills task boxes
		System::Void setGuiText(String^ userInput) {

			magicManager->generateMessageOutputs(userInput);

			String^ successMessage = magicManager->getSuccessMessage();
			successMessageLabel->Text = successMessage;

			allTaskBoxLabel->Text = magicManager->getAllTaskBoxLabel();

			magicManager->toggleTaskBoxSize(allTaskBox, todayTaskBox, pictureBox);

			allTaskBox->Text = magicManager->getAllTaskBoxMessage();
			todayTaskBox->Text = magicManager->getTodayTaskBoxMessage();

			magicManager->colorAllTaskBox(allTaskBox);
			magicManager->colorTodayTaskBox(todayTaskBox);

			clearCommandBox();

			checkExit(successMessage);
		}

		System::Void clearCommandBox() {
			commandInputBox->Text = "";
			commandInputBox->SelectionStart = 100;
		}

		System::Void checkExit(String^ input) {
			if (input == "exit") {
				MessageBox::Show("Good bye!");
				Application::Exit();
			}
		}


	protected:
		// Restore window from minimized when Ctrl + M is pressed
		virtual void OnHandleCreated(EventArgs^ e) override {
			__super::OnHandleCreated(e);
			RegisterHotKey((HWND)this->Handle.ToPointer(), 1, 
				MOD_CONTROL, (UINT)Keys::M); 
		}

	protected:
		virtual void WndProc(Message% m) override {
			if (m.Msg == WM_HOTKEY && m.WParam.ToInt32() == 1) {
				this->WindowState = FormWindowState::Normal;
				this->BringToFront();
			}
			__super::WndProc(m);
		}
	};

	// hide the command prompt window
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") 

}
