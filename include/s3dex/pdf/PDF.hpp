# pragma once
# include <Siv3D.hpp>
# include "s3dex/PDF/Page.hpp"
# include <poppler/poppler-document.h>
# include <poppler/poppler-page.h>
# include <poppler/poppler-page-renderer.h>
# include <poppler/poppler-image.h>

namespace s3dex
{
	namespace pdf
	{
		/// @brief PDF ドキュメントの読み込み
		class PDF
		{
		public:
			PDF() = default;

			explicit PDF(const FilePath& path, const double xdpi = 300, const double ydpi = 300);

			/// @brief PDF ドキュメントを読み込む
			/// @param path 
			/// @param xdpi 
			/// @param ydpi 
			/// @return 
			bool load(const FilePath& path, const double xdpi, const double ydpi);

			/// @brief ページ数を取得
			/// @return ページ数
			int getPageCount() const;

			/// @brief 現在のページ番号を取得
			/// @return 現在のページ番号
			int getCurrentPageIndex() const;

			/// @brief  
			/// @return 
			const Page& getCurrentPage() const;

			const Array<Page>& getPages() const;

			/// @brief 現在のページ番号を設定
			/// @param index
			void setCurrentPageIndex(int index);

			/// @brief 前のページに移動
			void turnOverPrevious();

			/// @brief 次のページに移動
			void turnOverNext();

		private:
			Page m_currentPage;
			Array<Page> m_pages;
			int m_pageCount;
			int m_currentPageIndex;
			Array<String> extractTextsFromPage(poppler::document* doc, int pageIndex);
			Image convertPopplerImageToSiv3DImage(poppler::image& popplerImage);
		};
	}
}
