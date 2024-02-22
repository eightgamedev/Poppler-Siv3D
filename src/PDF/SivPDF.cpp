# include "Siv3DEx/PDF.hpp"

namespace s3dex
{
	PDF::PDF(const FilePath& path, const double xdpi, const double ydpi)
	{
		load(path, xdpi, ydpi);
	}

	bool PDF::load(const FilePath& path, const double xdpi, const double ydpi)
	{
		std::unique_ptr<poppler::document> doc{ poppler::document::load_from_file(path.narrow()) };
		if (!doc)
		{
			return false;
		}

		m_pageCount = doc->pages();

		for (int i = 0; i < m_pageCount; ++i)
		{
			std::unique_ptr<poppler::page> page{ doc->create_page(i) };
			if (!page)
			{
				throw Error{ U"ERROR: Could not open page {}"_fmt(i) };
			}

			poppler::page_renderer renderer;
			poppler::image popplerImage = renderer.render_page(page.get(), xdpi, ydpi);
			Image sivImage = convertPopplerImageToSiv3DImage(popplerImage);
			sivImage.scale(0.25); // 画像を縮小
			m_renderedImages.push_back(sivImage);
			m_textures.push_back(Texture(sivImage));
			m_texts.push_back(extractTextsFromPage(doc.get(), i));
		}

		return true;
	}

	int PDF::getPageCount() const
	{
		return m_pageCount;
	}

	int PDF::getCurrentPageIndex() const
	{
		return m_currentPageIndex;
	}

	void PDF::setCurrentPageIndex(int index)
	{
		m_currentPageIndex = index;
	}

	void PDF::turnOverPrevious()
	{
		if (m_currentPageIndex > 0)
		{
			--m_currentPageIndex;
		}
	}

	void PDF::turnOverNext()
	{
		if (m_currentPageIndex < m_pageCount - 1)
		{
			++m_currentPageIndex;
		}
	}

	const Array<String>& PDF::getCurrentPageTexts() const
	{
		return m_texts[m_currentPageIndex];
	}

	void PDF::drawCurrentPage(double x, double y) const
	{
		m_textures[m_currentPageIndex].draw(x, y);
	}

	Array<String> PDF::extractTextsFromPage(poppler::document* doc, int pageIndex)
	{
		Array<String> texts;

		std::unique_ptr<poppler::page> page{ doc->create_page(pageIndex) };
		if (!page)
		{
			throw Error{ U"ERROR: Could not open page {}"_fmt(pageIndex) };
		}

		const poppler::ustring text = page->text();
		const std::vector<char> vec = text.to_utf8();
		const std::string str = { vec.begin(), vec.end() };
		const String sivString = Unicode::FromUTF8(str);
		texts = sivString.split(U'\n');
		texts.remove_if([](const String& s) { return s.isEmpty(); });
		texts.remove_if([](const String& s) {
			return s.all([](const char32 c) { return c == '\n' || c == '\r'; });
		});

		return texts;
	}

	Image PDF::convertPopplerImageToSiv3DImage(poppler::image& popplerImage)
	{
		const int32 width = popplerImage.width();
		const int32 height = popplerImage.height();
		const char* pixelData = popplerImage.data();

		s3d::Image image(width, height);

		for (int y = 0; y < image.height(); ++y) {
			for (int x = 0; x < image.width(); ++x) {
				const auto index = (y * image.width() + x) * 4;
				const auto b = pixelData[index + 0];
				const auto g = pixelData[index + 1];
				const auto r = pixelData[index + 2];
				image[y][x] = Color(r, g, b);
			}
		}

		return image;
	}


}
