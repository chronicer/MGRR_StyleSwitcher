#include "Hotkey.h"
#include "KeyBind.h"

#include "imgui/imgui.h"

Hotkey::Hotkey()
{
	this->m_nHkey = 0;
}

Hotkey::Hotkey(int hkey)
{
	this->m_nHkey = hkey;
}

bool Hotkey::IsPressed()
{
	return ImGui::IsKeyPressed(this->m_nHkey, false);
}

void Hotkey::Draw(const char *label)
{
	ImGui::PushID(label);
	ImGui::TextUnformatted(label);
	ImGui::SameLine();
	if (!this->bWaitsInput && ImGui::Button(KeyNames[this->m_nHkey], ImVec2(100.0f, 0.0f)))
		this->bWaitsInput = true;
	else if (this->bWaitsInput)
		ImGui::Button("...", ImVec2(100.0f, 0.0f));
	if (this->bWaitsInput)
		this->Rebind();
	ImGui::PopID();
}

void Hotkey::Rebind()
{
	if (KeyBind::SetToPressedKey(&this->m_nHkey) && this->bWaitsInput)
		this->bWaitsInput = false;
}