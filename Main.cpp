# include <Siv3D.hpp> // Siv3D v0.6.14
# include <poppler/poppler-document.h>
# include <poppler/poppler-page.h>
# include <poppler/poppler-page-renderer.h>
# include <poppler/poppler-image.h>

Image convertPopplerImageToSiv3DImage(poppler::image& image)
{
	Image sivImage(image.width(), image.height());
	const auto* pixelData = reinterpret_cast<const unsigned char*>(image.data());

	for (int y = 0; y < image.height(); ++y) {
		for (int x = 0; x < image.width(); ++x) {
			const auto index = (y * image.width() + x) * 4;
			const auto b = pixelData[index + 0];
			const auto g = pixelData[index + 1];
			const auto r = pixelData[index + 2];
			sivImage[y][x] = Color(r, g, b);
		}
	}
	return sivImage;
}

Array<String> getTextsFromPage(const poppler::document* doc, const int pageIndex)
{
	std::unique_ptr<poppler::page> page{ doc->create_page(pageIndex) };
	if (!page)
	{
		throw Error{ U"ERROR: Could not open page {}"_fmt(pageIndex) };
	}
	const poppler::ustring text = page->text();
	const std::vector<char> vec = text.to_utf8();
	const std::string str = { vec.begin(), vec.end() };
	const String sivString = Unicode::FromUTF8(str);
	Array<String> texts = sivString.split(U'\n');
	texts.remove_if([](const String& s) { return s.isEmpty(); });
	texts.remove_if([](const String& s) {
		return s.all([](const char32 c) { return c == '\n' || c == '\r'; });
	});

	return texts;
}

Texture getTextureFromPage(poppler::document* doc, int pageIndex)
{
	std::unique_ptr<poppler::page> page{ doc->create_page(pageIndex) };
	if (!page)
	{
		throw Error{ U"ERROR: Could not open page {}"_fmt(pageIndex) };
	}

	poppler::page_renderer renderer;
	poppler::image popplerImage = renderer.render_page(page.get(), 300, 300); // 300dpi
	Image sivImage = convertPopplerImageToSiv3DImage(popplerImage);
	sivImage.scale(0.25); // 画像を縮小
	Texture texture(sivImage);

	return texture;
}

void loadPDF(const String& filename, Array<Array<String>>& textsList, Array<Texture>& pageTextures, int& lastPage)
{
	std::unique_ptr<poppler::document> doc{ poppler::document::load_from_file(filename.narrow()) };
	if (!doc)
	{
		throw Error{ U"ERROR: Could not open file "_fmt(filename) };
	}

	lastPage = doc->pages();
	textsList.resize(lastPage);
	pageTextures.resize(lastPage);

	for (int i = 0; i < lastPage; ++i)
	{
		textsList[i] = getTextsFromPage(doc.get(), i);
		pageTextures[i] = getTextureFromPage(doc.get(), i);
	}
}

void Main()
{
	Window::Resize(1920, 1009);
	Font font(30);

	// PDF ファイルを開く
	const String filename = U"test.pdf";

	Array<Array<String>> textsList;
	Array<Texture> pageTextures;
	int lastPage = 0;
	int currentPage = 0;

	loadPDF(filename, textsList, pageTextures, lastPage);

	while (System::Update())
	{
		// テクスチャを描く
		pageTextures[currentPage].draw(50, 50);

		// テキストを描く
		for (int i = 0; i < textsList[currentPage].size(); ++i)
		{
			font(textsList[currentPage][i]).draw(700, 200 + i * 30);
		}

		if (SimpleGUI::Button(U"Next Page", Vec2(1200, 60)))
		{
			if (currentPage < lastPage - 1)
			{
				++currentPage;
			}
		}
		if (SimpleGUI::Button(U"Previous Page", Vec2(1000, 60)))
		{
			if (currentPage > 0)
			{
				--currentPage;
			}
		}
	}
}
