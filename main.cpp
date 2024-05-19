#include <windows.h>
#include <gdiplus.h>
#include <commdlg.h>
#include <tchar.h>
#include <memory>
#include "main.h"
#include "rc.h"
#include <filesystem>

using tstring = std::basic_string<TCHAR>;

std::unique_ptr<Gdiplus::Image> img;
tstring fileName;

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

		Font font(L"Arial", 25, FontStyleBold, UnitPixel);
		SolidBrush textBrush(Color::White);

		SolidBrush shadowBrush(Color(50, 0, 0, 0));


		RectF boundingBox;
		graphics.MeasureString(fileName.c_str(), -1, &font, PointF(0, 0), &boundingBox);


		float x = (rc.right - boundingBox.Width) / 2;
		float y = rc.bottom - boundingBox.Height - 10;


		PointF shadowPosition(x + 2, y + 2);
		graphics.DrawString(fileName.c_str(), -1, &font, shadowPosition, nullptr, &shadowBrush);


		PointF textPosition(x, y);
		graphics.DrawString(fileName.c_str(), -1, &font, textPosition, nullptr, &textBrush);
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
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = nullptr;
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
