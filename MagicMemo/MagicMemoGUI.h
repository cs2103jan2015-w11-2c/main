#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include "Controller.h"
#include "MessageManager.h"

using namespace std;

namespace UI {

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
		Controller* magicMemo;
		MessageManager^ outputMessage;
	public:
		MagicMemoGUI(void) {
			InitializeComponent();
			magicMemo = new Controller();
			outputMessage = gcnew MessageManager();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MagicMemoGUI() {
			if (components) {
				delete components;
				delete magicMemo;
				delete outputMessage;
			}
		}
	private: System::Windows::Forms::Label^  mainTaskBoxLabel;
	protected:
	private: System::Windows::Forms::TextBox^  commandInputBox;
	private: System::Windows::Forms::MonthCalendar^  monthCalendar;

	private: System::Windows::Forms::Label^  ResultLabel;

	private: System::Windows::Forms::Label^  programHeading;
	private: System::Windows::Forms::RichTextBox^  allTaskBox;
	private: System::Windows::Forms::Label^  textInputLabel;
	private: System::Windows::Forms::RichTextBox^  lastActionBox;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void) {
			this->mainTaskBoxLabel = (gcnew System::Windows::Forms::Label());
			this->commandInputBox = (gcnew System::Windows::Forms::TextBox());
			this->monthCalendar = (gcnew System::Windows::Forms::MonthCalendar());
			this->ResultLabel = (gcnew System::Windows::Forms::Label());
			this->programHeading = (gcnew System::Windows::Forms::Label());
			this->allTaskBox = (gcnew System::Windows::Forms::RichTextBox());
			this->textInputLabel = (gcnew System::Windows::Forms::Label());
			this->lastActionBox = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// mainTaskBoxLabel
			// 
			this->mainTaskBoxLabel->AutoSize = true;
			this->mainTaskBoxLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->mainTaskBoxLabel->Location = System::Drawing::Point(136, 54);
			this->mainTaskBoxLabel->Name = L"mainTaskBoxLabel";
			this->mainTaskBoxLabel->Size = System::Drawing::Size(73, 16);
			this->mainTaskBoxLabel->TabIndex = 0;
			this->mainTaskBoxLabel->Text = L"All Tasks";
			// 
			// commandInputBox
			// 
			this->commandInputBox->Location = System::Drawing::Point(23, 351);
			this->commandInputBox->Name = L"commandInputBox";
			this->commandInputBox->Size = System::Drawing::Size(536, 20);
			this->commandInputBox->TabIndex = 1;
			this->commandInputBox->UseWaitCursor = true;
			this->commandInputBox->TextChanged += gcnew System::EventHandler(this, &MagicMemoGUI::commandInputBox_TextChanged);
			this->commandInputBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MagicMemoGUI::commandInputBox_KeyPress);
			// 
			// monthCalendar
			// 
			this->monthCalendar->Enabled = false;
			this->monthCalendar->Location = System::Drawing::Point(333, 73);
			this->monthCalendar->Name = L"monthCalendar";
			this->monthCalendar->TabIndex = 2;
			this->monthCalendar->TitleBackColor = System::Drawing::SystemColors::MenuHighlight;
			// 
			// ResultLabel
			// 
			this->ResultLabel->AutoSize = true;
			this->ResultLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ResultLabel->Location = System::Drawing::Point(418, 244);
			this->ResultLabel->Name = L"ResultLabel";
			this->ResultLabel->Size = System::Drawing::Size(52, 16);
			this->ResultLabel->TabIndex = 5;
			this->ResultLabel->Text = L"Result";
			// 
			// programHeading
			// 
			this->programHeading->AutoSize = true;
			this->programHeading->Font = (gcnew System::Drawing::Font(L"AR DARLING", 30, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->programHeading->ForeColor = System::Drawing::Color::DarkBlue;
			this->programHeading->Location = System::Drawing::Point(168, -6);
			this->programHeading->Name = L"programHeading";
			this->programHeading->Size = System::Drawing::Size(249, 49);
			this->programHeading->TabIndex = 6;
			this->programHeading->Text = L"Magic Memo";
			// 
			// allTaskBox
			// 
			this->allTaskBox->Font = (gcnew System::Drawing::Font(L"Script MT Bold", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->allTaskBox->Location = System::Drawing::Point(23, 73);
			this->allTaskBox->Margin = System::Windows::Forms::Padding(5);
			this->allTaskBox->Name = L"allTaskBox";
			this->allTaskBox->ReadOnly = true;
			this->allTaskBox->Size = System::Drawing::Size(295, 244);
			this->allTaskBox->TabIndex = 7;
			this->allTaskBox->Text = L"";
			// 
			// textInputLabel
			// 
			this->textInputLabel->AutoSize = true;
			this->textInputLabel->Location = System::Drawing::Point(23, 332);
			this->textInputLabel->Name = L"textInputLabel";
			this->textInputLabel->Size = System::Drawing::Size(79, 13);
			this->textInputLabel->TabIndex = 8;
			this->textInputLabel->Text = L"Enter text here:";
			// 
			// lastActionBox
			// 
			this->lastActionBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lastActionBox->Location = System::Drawing::Point(333, 262);
			this->lastActionBox->Margin = System::Windows::Forms::Padding(4);
			this->lastActionBox->Name = L"lastActionBox";
			this->lastActionBox->ReadOnly = true;
			this->lastActionBox->Size = System::Drawing::Size(226, 55);
			this->lastActionBox->TabIndex = 9;
			this->lastActionBox->Text = L"";
			// 
			// MagicMemoGUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(586, 383);
			this->Controls->Add(this->lastActionBox);
			this->Controls->Add(this->textInputLabel);
			this->Controls->Add(this->allTaskBox);
			this->Controls->Add(this->programHeading);
			this->Controls->Add(this->ResultLabel);
			this->Controls->Add(this->monthCalendar);
			this->Controls->Add(this->commandInputBox);
			this->Controls->Add(this->mainTaskBoxLabel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"MagicMemoGUI";
			this->Text = L"MagicMemoGUI";
			this->Load += gcnew System::EventHandler(this, &MagicMemoGUI::MagicMemoGUI_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:

		//Get value while typing
		System::Void commandInputBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

		}
	private:
		System::Void commandInputBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
			if (e->KeyChar == 13) { //Enter key pressed
				String^ inputText = commandInputBox->Text;
				string successMessage = magicMemo->executeCommand(outputMessage->convertToStdString(inputText));
				lastActionBox->Text = outputMessage->convertToSystemString(successMessage);


				allTaskBox->Text = outputMessage->convertToSystemString(magicMemo->displayAll());
				commandInputBox->Text = outputMessage->convertToSystemString(magicMemo->getInputBoxMessage());
				commandInputBox->SelectionStart = 100;

				if (successMessage == "exit") {
					MessageBox::Show("Good bye!");
					Application::Exit();
				}
			}
		}

	private: System::Void MagicMemoGUI_Load(System::Object^  sender, System::EventArgs^  e) {
		    }
	};

}
