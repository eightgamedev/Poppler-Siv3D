# include <Siv3D.hpp> // Siv3D v0.6.14

# include "Siv3DEx/PDF.hpp"
# include "Siv3DEx/Page.hpp"

void Main()
{
	Window::Resize(1920, 1009);
	Font font(25);

	s3dex::PDF pdf;
	bool isLoaded = false;

	while (System::Update())
	{
		if (DragDrop::HasNewFilePaths())
		{
			for (const auto& dropped : DragDrop::GetDroppedFilePaths())
			{
				if (dropped.path.includes(U".pdf") || dropped.path.includes(U".PDF"))
				{
					pdf = s3dex::PDF(dropped.path, 300, 300);
					isLoaded = true;
				}
			}
		}

		if (isLoaded)
		{
			const s3dex::Page& currentPage = pdf.getCurrentPage();
			const Array<String>& texts = currentPage.getText();
			const Texture& texture = currentPage.getTexture();

			for (size_t i = 0; i < texts.size(); ++i)
			{
				font(texts[i]).draw(750, 50 + i * 30, Palette::White);
			}

			currentPage.drawTexture(50, 50);

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
}
