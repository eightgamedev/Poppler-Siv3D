# pragma once
# include <Siv3D.hpp>
# include <poppler/poppler-document.h>
# include <poppler/poppler-page.h>
# include <poppler/poppler-page-renderer.h>
# include <poppler/poppler-image.h>

namespace s3dex
{
	/// @brief PDF ドキュメントの読み込み
	class PDF
	{
	public:

		PDF() = default;

		explicit PDF(const FilePath& path, const double xdpi = 300, const double ydpi = 300);

		bool load(const FilePath& path, const double xdpi, const double ydpi);

		/// @brief ページ数を取得
		/// @return ページ数
		int getPageCount() const;

		/// @brief 現在のページ番号を取得
		/// @return 現在のページ番号
		int getCurrentPageIndex() const;

		/// @brief 現在のページ番号を設定
		void setCurrentPageIndex(int index);

		/// @brief 前のページに移動
		void turnOverPrevious();

		/// @brief 次のページに移動
		void turnOverNext();

		/// @brief 現在のページのテキストを取得
		const Array<String>& getCurrentPageTexts() const;

		/// @brief 現在のページの画像を描画
		/// @param x 描画する位置の x 座標
		/// @param y 描画する位置の y 座標
		void drawCurrentPage(double x, double y) const;

	private:
		int m_pageCount;
		int m_currentPageIndex;
		Array<Array<String>> m_texts;
		Array<Image> m_renderedImages;
		Array<Texture> m_textures;
		Array<String> extractTextsFromPage(poppler::document* doc, int pageIndex);
		Image convertPopplerImageToSiv3DImage(poppler::image& popplerImage);
	};
}
