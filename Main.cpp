# include <Siv3D.hpp> // Siv3D v0.6.14

# include "Siv3DEx/PDF.hpp"

void Main()
{
	Window::Resize(1920, 1009);
	Font font(25);

	const FilePath filename = U"test.pdf";

	s3dex::PDF pdf(filename);

	while (System::Update())
	{
		pdf.drawCurrentPage(50, 50);

		for (size_t i = 0; i < pdf.getCurrentPageTexts().size(); ++i)
		{
			font(pdf.getCurrentPageTexts()[i]).draw(750, 50 + 30 * i);
		}

		if (SimpleGUI::Button(U"<<", Vec2(100, 950)))
		{
			pdf.setCurrentPageIndex(0);
		}

		if (SimpleGUI::Button(U"<", Vec2(200, 950)))
		{
			pdf.turnOverPrevious();
		}

		if (SimpleGUI::Button(U">", Vec2(450, 950)))
		{
			pdf.turnOverNext();
		}

		if (SimpleGUI::Button(U">>", Vec2(550, 950)))
		{
			pdf.setCurrentPageIndex(pdf.getPageCount() - 1);
		}
	}
}
