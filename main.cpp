#include <windows.h>
#include <gdiplus.h>
#include <commdlg.h>
#include <tchar.h>
#include <memory>
#include "main.h"
#include "rc.h"
#include <filesystem>



tstring findImageName(const tstring& path)
{
	tstring temp = std::filesystem::path(path).filename();
	return temp;
}




void main_window::on_paint(HDC hdc)
{
	RECT rc;
	GetClientRect(*this, &rc);

	using namespace Gdiplus;
	Graphics graphics(hdc);


	if (!img)
		return;


	graphics.DrawImage(img.get(), 0, 0, rc.right, rc.bottom);


	if (!fileName.empty()) {
		Gdiplus::Font font(L"Arial", 25, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		Gdiplus::SolidBrush textBrush(Gdiplus::Color::White);
		Gdiplus::SolidBrush shadowBrush(Gdiplus::Color(50, 0, 0, 0));

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentFar);

	

		Gdiplus::RectF layoutRect(0, 0, rc.right, rc.bottom); 

		
		Gdiplus::RectF shadowLayoutRect(2, 2, rc.right, rc.bottom);
		graphics.DrawString(fileName.c_str(), -1, &font, shadowLayoutRect, &format, &shadowBrush);

		
		graphics.DrawString(fileName.c_str(), -1, &font, layoutRect, &format, &textBrush);
	}



}




void main_window::on_command(int id)
{
	switch (id)
	{
	case ID_OPEN:
	{
		OPENFILENAME ofn;
		TCHAR file_name[MAX_PATH] = _T("");
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = *this;
		ofn.lpstrFile = file_name;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(file_name);
		ofn.lpstrFilter = _T("Image Files\0*.jpg;*.jpeg;*.bmp;*.gif;*.png;*.tiff;*.emf\0All Files\0*.*\0");
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if (GetOpenFileName(&ofn))
		{
			img.reset(Gdiplus::Image::FromFile(ofn.lpstrFile));
			fileName = findImageName(ofn.lpstrFile);
			InvalidateRect(*this, nullptr, true);
		}
	}
	break;
	case ID_EXIT:
		DestroyWindow(*this);
		break;
	}
}

void main_window::on_destroy()
{
	::PostQuitMessage(0);
}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
