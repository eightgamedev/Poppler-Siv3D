# pragma once
# include <Siv3D.hpp>

namespace s3dex
{
	class Page
	{
	public:
		Page() = default;
		Page(const Array<String>& texts, const Image& image)
			: m_texts(texts)
			, m_image(image)
			, m_texture(image)
		{}
		const Array<String>& getText() const; 
		const Texture getTexture() const;

		/// @brief 画像を描画
		/// @param x 
		/// @param y 
		void drawTexture(double x, double y) const;
	private:
		Array<String> m_texts;
		Image m_image;
		Texture m_texture;
	};
}
